/******************************************************************************/
/* Copyright (c) 2024 Taliesin Perscilla "FlutterTal" Ambroise                */
/******************************************************************************/
/* This software is provided ‘as-is’, without any express or implied          */
/* warranty. In no event will the authors be held liable for any damages      */
/* arising from the use of this software.                                     */
/*                                                                            */
/* Permission is granted to anyone to use this software for any purpose,      */
/* including commercial applications, and to alter it and redistribute it     */
/* freely, subject to the following restrictions:                             */
/*                                                                            */
/* 1. The origin of this software must not be misrepresented; you must not    */
/* claim that you wrote the original software. If you use this software       */
/* in a product, an acknowledgment in the product documentation would be      */
/* appreciated but is not required.                                           */
/*                                                                            */
/* 2. Altered source versions must be plainly marked as such, and must not be */
/* misrepresented as being the original software.                             */
/*                                                                            */
/* 3. This notice may not be removed or altered from any source               */
/* distribution.                                                              */
/******************************************************************************/

#include "resource.h"
#include "device.h"
#include <memory>
#include <mutex>

namespace {
    unsigned int count = 0;
    std::recursive_mutex mutex;
    std::unique_ptr<Device> global_device;
}

AudioResource::AudioResource() {
    const std::lock_guard lock(mutex);
    if(count == 0)
        global_device = std::make_unique<Device>();

    ++count;
}

AudioResource::~AudioResource() {
    const std::lock_guard lock(mutex);
    --count;
    if(count == 0)
        global_device.reset();
}