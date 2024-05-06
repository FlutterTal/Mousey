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

#include "audio/sound.h"
#include <SDL2/SDL_sound.h>

Sound::Sound(const char * path) {
    alGenBuffers(1, &buffer);
    Sound_Sample * sample = Sound_NewSampleFromFile(path, nullptr, 65536);
    ALenum format = (sample->actual.channels == 1) ? AL_FORMAT_MONO16 : AL_FORMAT_STEREO16;
    uint32_t size = Sound_DecodeAll(sample);
    alBufferData(buffer, format, sample->buffer, size, sample->actual.rate);
    alSourcei(source, AL_BUFFER, buffer);
    Sound_FreeSample(sample);
}

Sound::~Sound() {
    stop();
    alDeleteBuffers(1, &buffer);
}

void Sound::play() {
    alSourcePlay(source);
}

void Sound::pause() {
    alSourcePause(source);
}

void Sound::stop() {
    alSourceStop(source);
}