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

#include "viewport_wrapper.h"
#include "modules/math/vector2.h"
#include "modules/math/math_wrapper.h"
#include "engine.h"

static SQInteger squirrel_viewport_getsize(HSQUIRRELVM v) {
    int w, h;
    SDL_GetWindowSize(Engine::get_singleton()->get_window()->get_window(), &w, &h);
    sq_pushroottable(v);
    sq_pushstring(v, _SC("Mousey"), -1);
    sq_get(v, -2);
    sq_pushstring(v, _SC("Vector2"), -1);
    sq_get(v, -2);
    sq_createinstance(v, -1);
    Vector2 * vector = new Vector2(w, h);
    sq_setinstanceup(v, -1, vector);
    sq_setreleasehook(v, -1, squirrel_vector2_destructor);
    sq_remove(v, -2);
    sq_remove(v, -2);
    sq_remove(v, -2);
    return 1;
}

static SQInteger squirrel_viewport_getposition(HSQUIRRELVM v) {
    int x, y;
    SDL_GetWindowPosition(Engine::get_singleton()->get_window()->get_window(), &x, &y);
    sq_pushroottable(v);
    sq_pushstring(v, _SC("Mousey"), -1);
    sq_get(v, -2);
    sq_pushstring(v, _SC("Vector2"), -1);
    sq_get(v, -2);
    sq_createinstance(v, -1);
    Vector2 * vector = new Vector2(x, y);
    sq_setinstanceup(v, -1, vector);
    sq_setreleasehook(v, -1, squirrel_vector2_destructor);
    sq_remove(v, -2);
    sq_remove(v, -2);
    sq_remove(v, -2);
    return 1;
}

static SQInteger squirrel_viewport_getdisplaymodes(HSQUIRRELVM v) {
    SQInteger display_index = 0;
    if(sq_gettop(v) > 1) {
        if(SQ_FAILED(sq_getinteger(v, 2, &display_index)))
            return sq_throwerror(v, _SC("Argument 1 not an integer"));
    }

    sq_newarray(v, 0);
    int mode_count = SDL_GetNumDisplayModes(display_index);
    for(int i = 0; i < mode_count; i++) {
        SDL_DisplayMode mode;
        if(SDL_GetDisplayMode(display_index, i, &mode) != 0)
            return sq_throwerror(v, _SC("Error while recovering the display mode"));

        sq_newtable(v);
        sq_pushstring(v, _SC("format"), -1);
        sq_pushinteger(v, mode.format);
        sq_newslot(v, -3, SQFalse);
        sq_pushstring(v, _SC("width"), -1);
        sq_pushinteger(v, mode.w);
        sq_newslot(v, -3, SQFalse);
        sq_pushstring(v, _SC("height"), -1);
        sq_pushinteger(v, mode.h);
        sq_newslot(v, -3, SQFalse);
        sq_pushstring(v, _SC("refresh_rate"), -1);
        sq_pushinteger(v, mode.refresh_rate);
        sq_newslot(v, -3, SQFalse);
        sq_arrayappend(v, -2);
    }

    return 1;
}

void register_viewport_wrapper(HSQUIRRELVM v) {
    sq_pushstring(v, _SC("Viewport"), -1);
    sq_newtable(v);

    sq_pushstring(v, _SC("get_size"), -1);
    sq_newclosure(v, squirrel_viewport_getsize, 0);
    sq_setparamscheck(v, 1, _SC("."));
    sq_newslot(v, -3, SQFalse);

    sq_pushstring(v, _SC("get_position"), -1);
    sq_newclosure(v, squirrel_viewport_getposition, 0);
    sq_setparamscheck(v, 1, _SC("."));
    sq_newslot(v, -3, SQFalse);

    sq_pushstring(v, _SC("get_display_modes"), -1);
    sq_newclosure(v, squirrel_viewport_getdisplaymodes, 0);
    sq_setparamscheck(v, -1, _SC(".i"));
    sq_newslot(v, -3, SQFalse);

    sq_newslot(v, -3, SQFalse);
}