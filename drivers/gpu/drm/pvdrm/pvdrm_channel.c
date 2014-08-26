/*
  Copyright (C) 2014 Yusuke Suzuki <utatane.tea@gmail.com>

  Redistribution and use in source and binary forms, with or without
  modification, are permitted provided that the following conditions are met:

    * Redistributions of source code must retain the above copyright
      notice, this list of conditions and the following disclaimer.
    * Redistributions in binary form must reproduce the above copyright
      notice, this list of conditions and the following disclaimer in the
      documentation and/or other materials provided with the distribution.

  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
  AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
  IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
  ARE DISCLAIMED. IN NO EVENT SHALL <COPYRIGHT HOLDER> BE LIABLE FOR ANY
  DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
  (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
  LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
  ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
  (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF
  THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/

#include "drmP.h"
#include "drm.h"

/* Include nouveau's abi16 header directly. */
#include "../nouveau/nouveau_abi16.h"

#include "pvdrm.h"
#include "pvdrm_channel.h"
#include "pvdrm_gem.h"
#include "pvdrm_nouveau_abi16.h"
#include "pvdrm_slot.h"

int pvdrm_channel_alloc(struct drm_device *dev, struct drm_file *file, struct drm_nouveau_channel_alloc *req_out, struct drm_pvdrm_gem_object** result)
{
	struct drm_pvdrm_gem_object *obj;
	int ret;

	ret = pvdrm_nouveau_abi16_ioctl(dev, PVDRM_IOCTL_NOUVEAU_CHANNEL_ALLOC, req_out, sizeof(struct drm_nouveau_channel_alloc));
	if (ret) {
		return ret;
	}

	obj = pvdrm_gem_alloc_object(dev, file, req_out->channel, /* FIXME */  PAGE_SIZE);
	if (obj == NULL) {
		return -ENOMEM;
	}

	/* Adjust gem information for guest environment. */
	printk(KERN_INFO "PVDRM: Allocating guest channel %d with host %d.\n", obj->handle, obj->host);
	req_out->channel = obj->handle;

	*result = obj;
	return 0;
}

int pvdrm_channel_free(struct drm_device *dev, struct drm_file *file, struct drm_nouveau_channel_free *req_out)
{
	int ret = 0;
	struct drm_pvdrm_gem_object *obj;
	obj = pvdrm_gem_object_lookup(dev, file, req_out->channel);
	if (!obj) {
		printk(KERN_INFO "PVDRM: Freeing invalid channel %d.\n", req_out->channel);
		return -EINVAL;
	}
	printk(KERN_INFO "PVDRM: Freeing guest channel %d with host %d.\n", obj->handle, obj->host);
	req_out->channel = obj->host;
	ret = pvdrm_nouveau_abi16_ioctl(dev, PVDRM_IOCTL_NOUVEAU_CHANNEL_FREE, req_out, sizeof(struct drm_nouveau_channel_free));
	drm_gem_object_unreference(&obj->base);
	return ret;
}

/* vim: set sw=8 ts=8 et tw=80 : */