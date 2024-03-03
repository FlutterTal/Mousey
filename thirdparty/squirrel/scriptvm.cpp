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

#include "scriptvm.h"
#include "modules/math/math_wrapper.h"
#include "modules/graphics/graphics_wrapper.h"
#include "modules/viewport/viewport_wrapper.h"
#include "modules/keyboard/keyboard_wrapper.h"
#include "modules/mouse/mouse_wrapper.h"
#include "modules/audio/audio_wrapper.h"
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <sqstdio.h>
#include <sqstdblob.h>
#include <sqstdmath.h>
#include <sqstdsystem.h>
#include <sqstdstring.h>
#include <sqstdaux.h>

static void printfunc(HSQUIRRELVM v, const SQChar * s, ...) {
    va_list argv;
    va_start(argv, s);
    vfprintf(stdout, s, argv);
    va_end(argv);
}

static void errorfunc(HSQUIRRELVM v, const SQChar * s, ...) {
    va_list argv;
    va_start(argv, s);
    vfprintf(stderr, s, argv);
    va_end(argv);
}

ScriptVM::ScriptVM() {
    v = sq_open(1024);
    sq_setprintfunc(v, printfunc, errorfunc);
    sqstd_seterrorhandlers(v);
    sq_pushroottable(v);
    {
        sq_pushroottable(v);
        sq_pushstring(v, _SC("Mousey"), -1);
        sq_newtable(v);
        register_math_wrapper(v);
        register_graphics_wrapper(v);
        register_viewport_wrapper(v);
        register_keyboard_wrapper(v);
        register_mouse_wrapper(v);
        register_audio_wrapper(v);
        sq_newslot(v, -3, SQFalse);
        sq_pop(v, 1);
    }

    sqstd_register_iolib(v);
    sqstd_register_bloblib(v);
    sqstd_register_mathlib(v);
    sqstd_register_systemlib(v);
    sqstd_register_stringlib(v);
    sqstd_dofile(v, "main.nut", SQFalse, SQTrue);
}

ScriptVM::~ScriptVM(){
    sq_collectgarbage(v);
    sq_pop(v, 1);
    sq_close(v);
}