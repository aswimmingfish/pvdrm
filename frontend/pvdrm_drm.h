/*
  Copyright (C) 2014 Yusuke Suzuki <yusuke.suzuki@sslab.ics.keio.ac.jp>

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
#ifndef PVDRM_DRM_H_
#define PVDRM_DRM_H_

#include <drmP.h>

#include <common/pvdrm_slot.h>
#include <common/pvdrm_utils.h>

#include "pvdrm_cache.h"
#include "pvdrm_ttm.h"

#define DRIVER_NAME		"pvdrm-front"
#define DRIVER_DESC		"PVDRM frontend driver"
#define DRIVER_DATE		"20141116"

struct pvdrm_host_table;

struct pvdrm_fpriv {
	struct drm_file* file;
	int32_t host;
	struct pvdrm_host_table* hosts;
};

struct pvdrm_device {
	struct drm_device* dev;
	struct pvdrm_slots* slots;
	struct pvdrm_ttm* ttm;
	struct drm_open_hash mh2obj;
	spinlock_t mh2obj_lock;
	struct idr channels_idr;
	spinlock_t channels_lock;

	struct kmem_cache* hosts_cache;

	bool gem_cache_enabled;
	struct pvdrm_cache* gem_cache;
	struct workqueue_struct* wq;

        struct pvdrm_fpriv global_fpriv;
        struct file* global_filp;
};

int pvdrm_drm_init(struct pvdrm_device* pvdrm, struct drm_device *dev);
int pvdrm_drm_load(struct drm_device *dev, unsigned long flags);
int pvdrm_drm_unload(struct drm_device *dev);
int pvdrm_drm_open(struct drm_device *dev, struct drm_file *file);
void pvdrm_drm_preclose(struct drm_device *dev, struct drm_file *file);
void pvdrm_drm_postclose(struct drm_device *dev, struct drm_file *file);

#endif  /* PVDRM_DRM_H_ */
