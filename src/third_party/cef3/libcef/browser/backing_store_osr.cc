// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "libcef/browser/backing_store_osr.h"

#include "ui/gfx/rect.h"

BackingStoreOSR::BackingStoreOSR(content::RenderWidgetHost* widget,
                                 const gfx::Size& size)
    : content::BackingStore(widget, size) {
#ifdef BACKING_STORE_OSR_ALPHA
  bitmap_.setConfig(SkBitmap::kARGB_8888_Config, size.width(), size.height());
  bitmap_.allocPixels();
  canvas_.reset(new SkCanvas(bitmap_));
#else
  bitmap_.Allocate(size.width(), size.height(), true);
#endif
}

void BackingStoreOSR::ScrollBackingStore(const gfx::Vector2d& delta,
                                         const gfx::Rect& clip_rect,
                                         const gfx::Size& view_size) {
  SkIRect subset_rect = SkIRect::MakeXYWH(clip_rect.x(), clip_rect.y(),
      clip_rect.width(), clip_rect.height());
#ifdef BACKING_STORE_OSR_ALPHA
  bitmap_.scrollRect(&subset_rect, delta.x(), delta.y());
#else
  bitmap_.GetBitmap().scrollRect(&subset_rect, delta.x(), delta.y());
#endif
}

const void* BackingStoreOSR::getPixels() const {
#ifdef BACKING_STORE_OSR_ALPHA
  return const_cast<BackingStoreOSR*>(this)->bitmap_.getPixels();
#else
  return const_cast<BackingStoreOSR*>(this)->bitmap_.GetBitmap().getPixels();
#endif
}
