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

#include "source.h"

Source::Source() {
    alGenSources(1, &source);
    alSourcei(source, AL_BUFFER, 0);
}

Source::~Source() {
    alSourcei(source, AL_BUFFER, 0);
    alDeleteSources(1, &source);
}

Source::Status Source::get_status() const {
    ALint status;
    alGetSourcei(source, AL_SOURCE_STATE, &status);
    switch(status) {
    case AL_INITIAL:
    case AL_STOPPED:
        return Stopped;
    case AL_PAUSED:
        return Paused;
    case AL_PLAYING:
        return Playing;
    }

    return Stopped;
}