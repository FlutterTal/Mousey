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

#include "mouse_wrapper.h"
#include "modules/math/vector2.h"
#include "modules/math/math_wrapper.h"
#include "core.h"
#include <SDL2/SDL.h>

enum MouseButton {
    LEFT = SDL_BUTTON_LEFT,
    MIDDLE = SDL_BUTTON_MIDDLE,
    RIGHT = SDL_BUTTON_RIGHT,
    X1 = SDL_BUTTON_X1,
    X2 = SDL_BUTTON_X2
};

static SQInteger squirrel_mouse_getposition(HSQUIRRELVM v) {
    int x, y;
    SDL_GetMouseState(&x, &y);
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

static SQInteger squirrel_mouse_isdown(HSQUIRRELVM v) {
    MouseButton button;
    if(SQ_FAILED(sq_getinteger(v, 2, (SQInteger *)&button)))
        return sq_throwerror(v, _SC("Argument 1 not a mouse button"));

    sq_pushbool(v, (current_mouse_state & SDL_BUTTON(button)));
    return 1;
}

static SQInteger squirrel_mouse_ispressed(HSQUIRRELVM v) {
    MouseButton button;
    if(SQ_FAILED(sq_getinteger(v, 2, (SQInteger *)&button)))
        return sq_throwerror(v, _SC("Argument 1 not a mouse button"));

    sq_pushbool(v, (current_mouse_state & SDL_BUTTON(button)) && !(previous_mouse_state & SDL_BUTTON(button)));
    return 1;
}

static SQInteger squirrel_mouse_isreleased(HSQUIRRELVM v) {
    MouseButton button;
    if(SQ_FAILED(sq_getinteger(v, 2, (SQInteger *)&button)))
        return sq_throwerror(v, _SC("Argument 1 not a mouse button"));

    sq_pushbool(v, !(current_mouse_state & SDL_BUTTON(button)) && (previous_mouse_state & SDL_BUTTON(button)));
    return 1;
}

void register_mouse_wrapper(HSQUIRRELVM v) {
    sq_pushstring(v, _SC("MouseButton"), -1);
    sq_newtable(v);
    sq_pushstring(v, _SC("Left"), -1);
    sq_pushinteger(v, MouseButton::LEFT);
    sq_newslot(v, -3, SQFalse);
    sq_pushstring(v, _SC("Middle"), -1);
    sq_pushinteger(v, MouseButton::MIDDLE);
    sq_newslot(v, -3, SQFalse);
    sq_pushstring(v, _SC("Right"), -1);
    sq_pushinteger(v, MouseButton::RIGHT);
    sq_newslot(v, -3, SQFalse);
    sq_pushstring(v, _SC("X1"), -1);
    sq_pushinteger(v, MouseButton::X1);
    sq_newslot(v, -3, SQFalse);
    sq_pushstring(v, _SC("X2"), -1);
    sq_pushinteger(v, MouseButton::X2);
    sq_newslot(v, -3, SQFalse);
    sq_newslot(v, -3, SQFalse);

    sq_pushstring(v, _SC("Mouse"), -1);
    sq_newtable(v);

    sq_pushstring(v, _SC("get_position"), -1);
    sq_newclosure(v, squirrel_mouse_getposition, 0);
    sq_setparamscheck(v, 1, _SC("."));
    sq_newslot(v, -3, SQFalse);

    sq_pushstring(v, _SC("is_down"), -1);
    sq_newclosure(v, squirrel_mouse_isdown, 0);
    sq_setparamscheck(v, 2, _SC(".i"));
    sq_newslot(v, -3, SQFalse);

    sq_pushstring(v, _SC("is_pressed"), -1);
    sq_newclosure(v, squirrel_mouse_ispressed, 0);
    sq_setparamscheck(v, 2, _SC(".i"));
    sq_newslot(v, -3, SQFalse);

    sq_pushstring(v, _SC("is_released"), -1);
    sq_newclosure(v, squirrel_mouse_isreleased, 0);
    sq_setparamscheck(v, 2, _SC(".i"));
    sq_newslot(v, -3, SQFalse);

    sq_newslot(v, -3, SQFalse);
}