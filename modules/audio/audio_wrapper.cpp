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

#include "audio_wrapper.h"
#include "audio/sound.h"
#include "audio/music.h"

SQInteger squirrel_sound_destructor(SQUserPointer p, SQInteger SQ_UNUSED_ARG(size)) {
    Sound * instance = reinterpret_cast<Sound *>(p);
    delete instance;
    return 0;
}

static SQInteger squirrel_sound_constructor(HSQUIRRELVM v) {
    const SQChar * path;
    if(SQ_FAILED(sq_getstring(v, 2, &path)))
        return sq_throwerror(v, _SC("Argument 1 not a string"));
    
    Sound * instance = new Sound(path);
    sq_setinstanceup(v, 1, instance);
    sq_setreleasehook(v, 1, squirrel_sound_destructor);
    return 0;
}

static SQInteger squirrel_sound_play(HSQUIRRELVM v) {
    Sound * instance;
    sq_getinstanceup(v, 1, (SQUserPointer *)&instance, (SQUserPointer)"SoundTag", SQTrue);
    instance->play();
    return 0;
}

SQInteger squirrel_music_destructor(SQUserPointer p, SQInteger SQ_UNUSED_ARG(size)) {
    Music * instance = reinterpret_cast<Music *>(p);
    delete instance;
    return 0;
}

static SQInteger squirrel_music_constructor(HSQUIRRELVM v) {
    const SQChar * path;
    if(SQ_FAILED(sq_getstring(v, 2, &path)))
        return sq_throwerror(v, _SC("Argument 1 not a string"));
    
    Music * instance = new Music(path);
    sq_setinstanceup(v, 1, instance);
    sq_setreleasehook(v, 1, squirrel_music_destructor);
    return 0;
}

static SQInteger squirrel_music_play(HSQUIRRELVM v) {
    Music * instance;
    sq_getinstanceup(v, 1, (SQUserPointer *)&instance, (SQUserPointer)"MusicTag", SQTrue);
    instance->play();
    return 0;
}

static SQInteger squirrel_music_setlooping(HSQUIRRELVM v) {
    Music * instance;
    sq_getinstanceup(v, 1, (SQUserPointer *)&instance, (SQUserPointer)"MusicTag", SQTrue);
    SQBool loop;
    if(SQ_FAILED(sq_getbool(v, 2, &loop)))
        return sq_throwerror(v, _SC("Argument 1 not a bool"));
    
    instance->set_looping(loop);
    return 0;
}

void register_audio_wrapper(HSQUIRRELVM v) {
    sq_pushstring(v, _SC("Sound"), -1);
    sq_newclass(v, SQFalse);
    sq_settypetag(v, -1, (SQUserPointer)"SoundTag");

    sq_pushstring(v, _SC("constructor"), -1);
    sq_newclosure(v, squirrel_sound_constructor, 0);
    sq_setparamscheck(v, 2, _SC(".s"));
    sq_newslot(v, -3, SQFalse);

    sq_pushstring(v, _SC("play"), -1);
    sq_newclosure(v, squirrel_sound_play, 0);
    sq_setparamscheck(v, 1, _SC("x"));
    sq_newslot(v, -3, SQFalse);

    sq_newslot(v, -3, SQFalse);

    sq_pushstring(v, _SC("Music"), -1);
    sq_newclass(v, SQFalse);
    sq_settypetag(v, 1, (SQUserPointer)"MusicTag");

    sq_pushstring(v, _SC("constructor"), -1);
    sq_newclosure(v, squirrel_music_constructor, 0);
    sq_setparamscheck(v, 2, _SC(".s"));
    sq_newslot(v, -3, SQFalse);

    sq_pushstring(v, _SC("play"), -1);
    sq_newclosure(v, squirrel_music_play, 0);
    sq_setparamscheck(v, 1, _SC("x"));
    sq_newslot(v, -3, SQFalse);

    sq_pushstring(v, _SC("set_looping"), -1);
    sq_newclosure(v, squirrel_music_setlooping, 0);
    sq_setparamscheck(v, 2, _SC("xb"));
    sq_newslot(v, -3, SQFalse);

    sq_newslot(v, -3, SQFalse);
}