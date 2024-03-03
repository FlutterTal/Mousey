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

#include "device.h"
#include <AL/al.h>
#include <stdio.h>

Device::Device() {
    device = alcOpenDevice(nullptr);
    if(device == nullptr) {
        fprintf(stderr, "Failed to open the audio device\n");
        return;
    }

    context = alcCreateContext(device, nullptr);
    if(context == nullptr) {
        fprintf(stderr, "Failed to create the audio context\n");
        return;
    }

    alcMakeContextCurrent(context);
    alListenerf(AL_GAIN, 1.0);
}

Device::~Device() {
    alcMakeContextCurrent(nullptr);
    if(context)
        alcDestroyContext(context);

    if(device)
        alcCloseDevice(device);
}