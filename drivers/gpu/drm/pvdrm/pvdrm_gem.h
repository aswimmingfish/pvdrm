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
#ifndef PVDRM_GEM_H_
#define PVDRM_GEM_H_

#include "drmP.h"
#include "nouveau_drm.h"

struct drm_pvdrm_gem_object {
	struct drm_gem_object base;
	struct drm_hash_item hash;
	uint32_t handle;
	uint32_t host;
	uint32_t domain;
	uint64_t map_handle;
	unsigned long backing;  /* Backing stone for VRAM mapping. */
};

int pvdrm_gem_fault(struct vm_area_struct *vma, struct vm_fault *vmf);
int pvdrm_gem_object_init(struct drm_gem_object *obj);
void pvdrm_gem_object_free(struct drm_gem_object *gobj);
int pvdrm_gem_object_open(struct drm_gem_object *obj, struct drm_file *file);
void pvdrm_gem_object_close(struct drm_gem_object *obj, struct drm_file *file);
int pvdrm_gem_object_new(struct drm_device *dev, struct drm_file *file, struct drm_nouveau_gem_new *req_out, struct drm_pvdrm_gem_object** result);
struct drm_pvdrm_gem_object* pvdrm_gem_object_lookup(struct drm_device *dev, struct drm_file *file, uint32_t handle);
struct drm_pvdrm_gem_object* pvdrm_gem_alloc_object(struct drm_device *dev, struct drm_file *file, uint32_t host, uint32_t size);
void pvdrm_gem_register_host_info(struct drm_device* dev, struct drm_file *file, struct drm_pvdrm_gem_object* obj, struct drm_nouveau_gem_info* info);

int pvdrm_gem_mmap(struct file *filp, struct vm_area_struct *vma);

#endif  /* PVDRM_GEM_H_ */
/* vim: set sw=8 ts=8 et tw=80 : */
