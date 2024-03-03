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

#include "math_wrapper.h"
#include "vector2.h"
#include "rect2.h"
#include <stdio.h>
#include <math.h>

inline SQInteger sqVarGet(HSQUIRRELVM vm) {
    sq_push(vm, 2);
    if (SQ_FAILED(sq_rawget(vm, -2))) {
        sq_pushnull(vm);
        return sq_throwobject(vm);
    }

    sq_push(vm, 1);
    SQRESULT result = sq_call(vm, 1, SQTrue, SQTrue);
    return SQ_SUCCEEDED(result) ? 1 : SQ_ERROR;
}

inline SQInteger sqVarSet(HSQUIRRELVM vm) {
    sq_push(vm, 2);
    if (SQ_FAILED(sq_rawget(vm, -2))) {
        sq_pushnull(vm);
        return sq_throwobject(vm);
    }

    sq_push(vm, 1);
    sq_push(vm, 3);
    SQRESULT result = sq_call(vm, 2, SQFalse, SQTrue);
    return SQ_SUCCEEDED(result) ? 0 : SQ_ERROR;
}

SQInteger squirrel_vector2_destructor(SQUserPointer p, SQInteger SQ_UNUSED_ARG(size)) {
    Vector2 * instance = reinterpret_cast<Vector2 *>(p);
    delete instance;
    return 0;
}

static SQInteger squirrel_vector2_constructor(HSQUIRRELVM v) {
    Vector2 * instance;
    if(sq_gettop(v) == 1)
        instance = new Vector2();
    else if(sq_gettop(v) == 2) {
        Vector2 * other;
        sq_getinstanceup(v, 2, (SQUserPointer *)&other, (SQUserPointer)"Vector2Tag", SQTrue);
        instance = new Vector2(*other);
    } else if(sq_gettop(v) == 3) {
        SQFloat x;
        SQFloat y;
        if(SQ_FAILED(sq_getfloat(v, 2, &x)))
            return sq_throwerror(v, _SC("Argument 1 not a float"));

        if(SQ_FAILED(sq_getfloat(v, 3, &y)))
            return sq_throwerror(v, _SC("Argument 2 not a float"));

        instance = new Vector2(x, y);
    } else {
        char buffer[1024];
        sprintf(buffer, "Too many arguments (expected 2, got %d)", sq_gettop(v) - 1);
        return sq_throwerror(v, _SC(buffer));
    }

    sq_setinstanceup(v, 1, instance);
    sq_setreleasehook(v, 1, squirrel_vector2_destructor);
    return 0;
}

static SQInteger squirrel_vector2_getx(HSQUIRRELVM v) {
    Vector2 * instance;
    sq_getinstanceup(v, 1, (SQUserPointer *)&instance, (SQUserPointer)"Vector2Tag", SQTrue);
    sq_pushfloat(v, instance->x);
    return 1;
}

static SQInteger squirrel_vector2_setx(HSQUIRRELVM v) {
    Vector2 * instance;
    sq_getinstanceup(v, 1, (SQUserPointer *)&instance, (SQUserPointer)"Vector2Tag", SQTrue);
    SQFloat x;
    if(SQ_FAILED(sq_getfloat(v, 2, &x)))
        return sq_throwerror(v, _SC("Argument 1 not a float"));

    instance->x = x;
    return 0;
}

static SQInteger squirrel_vector2_gety(HSQUIRRELVM v) {
    Vector2 * instance;
    sq_getinstanceup(v, 1, (SQUserPointer *)&instance, (SQUserPointer)"Vector2Tag", SQTrue);
    sq_pushfloat(v, instance->y);
    return 1;
}

static SQInteger squirrel_vector2_sety(HSQUIRRELVM v) {
    Vector2 * instance;
    sq_getinstanceup(v, 1, (SQUserPointer *)&instance, (SQUserPointer)"Vector2Tag", SQTrue);
    SQFloat y;
    if(SQ_FAILED(sq_getfloat(v, 2, &y)))
        return sq_throwerror(v, _SC("Argument 1 not a float"));

    instance->y = y;
    return 0;
}

static SQInteger squirrel_vector2_normalized(HSQUIRRELVM v) {
    Vector2 * instance;
    sq_getinstanceup(v, 1, (SQUserPointer *)&instance, (SQUserPointer)"Vector2Tag", SQTrue);
    sq_pushroottable(v);
    sq_pushstring(v, _SC("Mousey"), -1);
    sq_get(v, -2);
    sq_pushstring(v, _SC("Vector2"), -1);
    sq_get(v, -2);
    sq_createinstance(v, -1);
    Vector2 * result = new Vector2(instance->normalized());
    sq_setinstanceup(v, -1, result);
    sq_setreleasehook(v, -1, squirrel_vector2_destructor);
    sq_remove(v, -2);
    sq_remove(v, -2);
    sq_remove(v, -2);
    return 1;
}

static SQInteger squirrel_vector2_dot(HSQUIRRELVM v) {
    Vector2 * instance;
    sq_getinstanceup(v, 1, (SQUserPointer *)&instance, (SQUserPointer)"Vector2Tag", SQTrue);
    Vector2 * with;
    sq_getinstanceup(v, 2, (SQUserPointer *)&with, (SQUserPointer)"Vector2Tag", SQTrue);
    sq_pushfloat(v, instance->dot(*with));
    return 1;
}

static SQInteger squirrel_vector2_cross(HSQUIRRELVM v) {
    Vector2 * instance;
    sq_getinstanceup(v, 1, (SQUserPointer *)&instance, (SQUserPointer)"Vector2Tag", SQTrue);
    Vector2 * with;
    sq_getinstanceup(v, 2, (SQUserPointer *)&with, (SQUserPointer)"Vector2Tag", SQTrue);
    sq_pushfloat(v, instance->cross(*with));
    return 1;
}

static SQInteger squirrel_vector2_project(HSQUIRRELVM v) {
    Vector2 * instance;
    sq_getinstanceup(v, 1, (SQUserPointer *)&instance, (SQUserPointer)"Vector2Tag", SQTrue);
    Vector2 * to;
    sq_getinstanceup(v, 2, (SQUserPointer *)&to, (SQUserPointer)"Vector2Tag", SQTrue);
    sq_pushroottable(v);
    sq_pushstring(v, _SC("Mousey"), -1);
    sq_get(v, -2);
    sq_pushstring(v, _SC("Vector2"), -1);
    sq_get(v, -2);
    sq_createinstance(v, -1);
    Vector2 * result = new Vector2(instance->project(*to));
    sq_setinstanceup(v, -1, result);
    sq_setreleasehook(v, -1, squirrel_vector2_destructor);
    sq_remove(v, -2);
    sq_remove(v, -2);
    sq_remove(v, -2);
    return 1;
}

static SQInteger squirrel_vector2_add(HSQUIRRELVM v) {
    Vector2 * instance;
    sq_getinstanceup(v, 1, (SQUserPointer *)&instance, (SQUserPointer)"Vector2Tag", SQTrue);
    Vector2 * with;
    sq_getinstanceup(v, 2, (SQUserPointer *)&with, (SQUserPointer)"Vector2Tag", SQTrue);
    sq_pushroottable(v);
    sq_pushstring(v, _SC("Mousey"), -1);
    sq_get(v, -2);
    sq_pushstring(v, _SC("Vector2"), -1);
    sq_get(v, -2);
    sq_createinstance(v, -1);
    Vector2 * result = new Vector2(instance->x + with->x, instance->y + with->y);
    sq_setinstanceup(v, -1, result);
    sq_setreleasehook(v, -1, squirrel_vector2_destructor);
    sq_remove(v, -2);
    sq_remove(v, -2);
    sq_remove(v, -2);
    return 1;
}

static SQInteger squirrel_vector2_sub(HSQUIRRELVM v) {
    Vector2 * instance;
    sq_getinstanceup(v, 1, (SQUserPointer *)&instance, (SQUserPointer)"Vector2Tag", SQTrue);
    Vector2 * with;
    sq_getinstanceup(v, 2, (SQUserPointer *)&with, (SQUserPointer)"Vector2Tag", SQTrue);
    sq_pushroottable(v);
    sq_pushstring(v, _SC("Mousey"), -1);
    sq_get(v, -2);
    sq_pushstring(v, _SC("Vector2"), -1);
    sq_get(v, -2);
    sq_createinstance(v, -1);
    Vector2 * result = new Vector2(instance->x - with->x, instance->y - with->y);
    sq_setinstanceup(v, -1, result);
    sq_setreleasehook(v, -1, squirrel_vector2_destructor);
    sq_remove(v, -2);
    sq_remove(v, -2);
    sq_remove(v, -2);
    return 1;
}

static SQInteger squirrel_vector2_mul(HSQUIRRELVM v) {
    Vector2 * instance;
    sq_getinstanceup(v, 1, (SQUserPointer *)&instance, (SQUserPointer)"Vector2Tag", SQTrue);
    SQFloat with;
    if(SQ_FAILED(sq_getfloat(v, 2, &with)))
        return sq_throwerror(v, _SC("Argument 1 not a float"));

    sq_pushroottable(v);
    sq_pushstring(v, _SC("Mousey"), -1);
    sq_get(v, -2);
    sq_pushstring(v, _SC("Vector2"), -1);
    sq_get(v, -2);
    sq_createinstance(v, -1);
    Vector2 * result = new Vector2(instance->x * with, instance->y * with);
    sq_setinstanceup(v, -1, result);
    sq_setreleasehook(v, -1, squirrel_vector2_destructor);
    sq_remove(v, -2);
    sq_remove(v, -2);
    sq_remove(v, -2);
    return 1;
}

static SQInteger squirrel_vector2_div(HSQUIRRELVM v) {
    Vector2 * instance;
    sq_getinstanceup(v, 1, (SQUserPointer *)&instance, (SQUserPointer)"Vector2Tag", SQTrue);
    SQFloat with;
    if(SQ_FAILED(sq_getfloat(v, 2, &with)))
        return sq_throwerror(v, _SC("Argument 1 not a float"));

    sq_pushroottable(v);
    sq_pushstring(v, _SC("Mousey"), -1);
    sq_get(v, -2);
    sq_pushstring(v, _SC("Vector2"), -1);
    sq_get(v, -2);
    sq_createinstance(v, -1);
    Vector2 * result = new Vector2(instance->x / with, instance->y / with);
    sq_setinstanceup(v, -1, result);
    sq_setreleasehook(v, -1, squirrel_vector2_destructor);
    sq_remove(v, -2);
    sq_remove(v, -2);
    sq_remove(v, -2);
    return 1;
}

static SQInteger squirrel_vector2_unm(HSQUIRRELVM v) {
    Vector2 * instance;
    sq_getinstanceup(v, 1, (SQUserPointer *)&instance, (SQUserPointer)"Vector2Tag", SQTrue);
    sq_pushroottable(v);
    sq_pushstring(v, _SC("Mousey"), -1);
    sq_get(v, -2);
    sq_pushstring(v, _SC("Vector2"), -1);
    sq_get(v, -2);
    sq_createinstance(v, -1);
    Vector2 * result = new Vector2(-instance->x, -instance->y);
    sq_setinstanceup(v, -1, result);
    sq_setreleasehook(v, -1, squirrel_vector2_destructor);
    sq_remove(v, -2);
    sq_remove(v, -2);
    sq_remove(v, -2);
    return 1;
}

SQInteger squirrel_rect2_destructor(SQUserPointer p, SQInteger SQ_UNUSED_ARG(size)) {
    Rect2 * instance = reinterpret_cast<Rect2 *>(p);
    delete instance;
    return 0;
}

static SQInteger squirrel_rect2_constructor(HSQUIRRELVM v) {
    Rect2 * instance;
    if(sq_gettop(v) == 1)
        instance = new Rect2();
    else if(sq_gettop(v) == 2) {
        Rect2 * other;
        sq_getinstanceup(v, 2, (SQUserPointer *)&other, (SQUserPointer)"Rect2Tag", SQTrue);
        instance = new Rect2(*other);
    } else if(sq_gettop(v) == 3) {
        Vector2 * position;
        Vector2 * size;
        sq_getinstanceup(v, 2, (SQUserPointer *)&position, (SQUserPointer)"Vector2Tag", SQTrue);
        sq_getinstanceup(v, 3, (SQUserPointer *)&size, (SQUserPointer)"Vector2Tag", SQTrue);
        instance = new Rect2(*position, *size);
    } else {
        if(sq_gettop(v) < 5) {
            char buffer[1024];
            sprintf(buffer, "Too few arguments (expected 4, got %d)", sq_gettop(v) - 1);
            return sq_throwerror(v, _SC(buffer));
        } else if(sq_gettop(v) > 5) {
            char buffer[1024];
            sprintf(buffer, "Too many arguments (expected 4, got %d)", sq_gettop(v) - 1);
            return sq_throwerror(v, _SC(buffer));
        } else {
            SQFloat x;
            SQFloat y;
            SQFloat width;
            SQFloat height;
            if(SQ_FAILED(sq_getfloat(v, 2, &x)))
                return sq_throwerror(v, _SC("Argument 1 not a float"));

            if(SQ_FAILED(sq_getfloat(v, 3, &y)))
                return sq_throwerror(v, _SC("Argument 2 not a float"));

            if(SQ_FAILED(sq_getfloat(v, 4, &width)))
                return sq_throwerror(v, _SC("Argument 3 not a float"));

            if(SQ_FAILED(sq_getfloat(v, 5, &height)))
                return sq_throwerror(v, _SC("Argument 4 not a float"));

            instance = new Rect2(x, y, width, height);
        }
    }

    sq_setinstanceup(v, 1, instance);
    sq_setreleasehook(v, 1, squirrel_rect2_destructor);
    return 0;
}

static SQInteger squirrel_rect2_getposition(HSQUIRRELVM v) {
    Rect2 * instance;
    sq_getinstanceup(v, 1, (SQUserPointer *)&instance, (SQUserPointer)"Rect2Tag", SQTrue);
    sq_pushroottable(v);
    sq_pushstring(v, _SC("Mousey"), -1);
    sq_get(v, -2);
    sq_pushstring(v, _SC("Vector2"), -1);
    sq_get(v, -2);
    sq_createinstance(v, -1);
    Vector2 * position = new Vector2(instance->position);
    sq_setinstanceup(v, -1, position);
    sq_setreleasehook(v, -1, squirrel_vector2_destructor);
    sq_remove(v, -2);
    sq_remove(v, -2);
    sq_remove(v, -2);
    return 1;
}

static SQInteger squirrel_rect2_setposition(HSQUIRRELVM v) {
    Rect2 * instance;
    sq_getinstanceup(v, 1, (SQUserPointer *)&instance, (SQUserPointer)"Rect2Tag", SQTrue);
    Vector2 * position;
    sq_getinstanceup(v, 2, (SQUserPointer *)&position, (SQUserPointer)"Vector2Tag", SQTrue);
    instance->position = *position;
    return 0;
}

static SQInteger squirrel_rect2_getsize(HSQUIRRELVM v) {
    Rect2 * instance;
    sq_getinstanceup(v, 1, (SQUserPointer *)&instance, (SQUserPointer)"Rect2Tag", SQTrue);
    sq_pushroottable(v);
    sq_pushstring(v, _SC("Mousey"), -1);
    sq_get(v, -2);
    sq_pushstring(v, _SC("Vector2"), -1);
    sq_get(v, -2);
    sq_createinstance(v, -1);
    Vector2 * size = new Vector2(instance->size);
    sq_setinstanceup(v, -1, size);
    sq_setreleasehook(v, -1, squirrel_vector2_destructor);
    sq_remove(v, -2);
    sq_remove(v, -2);
    sq_remove(v, -2);
    return 1;
}

static SQInteger squirrel_rect2_setsize(HSQUIRRELVM v) {
    Rect2 * instance;
    sq_getinstanceup(v, 1, (SQUserPointer *)&instance, (SQUserPointer)"Rect2Tag", SQTrue);
    Vector2 * size;
    sq_getinstanceup(v, 2, (SQUserPointer *)&size, (SQUserPointer)"Vector2Tag", SQTrue);
    instance->size = *size;
    return 0;
}

static SQInteger squirrel_math_radtodeg(HSQUIRRELVM v) {
    SQFloat rad;
    if(SQ_FAILED(sq_getfloat(v, 2, &rad)))
        return sq_throwerror(v, _SC("Argument 1 not a float"));

    sq_pushfloat(v, rad * 180 / M_PI);
    return 1;
}

static SQInteger squirrel_math_degtorad(HSQUIRRELVM v) {
    SQFloat deg;
    if(SQ_FAILED(sq_getfloat(v, 2, &deg)))
        return sq_throwerror(v, _SC("Argument 1 not a float"));

    sq_pushfloat(v, deg * M_PI / 180);
    return 1;
}

void register_math_wrapper(HSQUIRRELVM v) {
    HSQOBJECT get_table;
    HSQOBJECT set_table;

    sq_pushstring(v, _SC("Vector2"), -1);
    sq_newclass(v, SQFalse);
    sq_settypetag(v, -1, (SQUserPointer)"Vector2Tag");

    sq_resetobject(&set_table);
    sq_pushstring(v, _SC("__setTable"), -1);
    sq_newtable(v);
    sq_getstackobj(v, -1, &set_table);
    sq_addref(v, &set_table);
    sq_newslot(v, -3, SQTrue);

    sq_resetobject(&get_table);
    sq_pushstring(v, _SC("__getTable"), -1);
    sq_newtable(v);
    sq_getstackobj(v, -1, &get_table);
    sq_addref(v, &get_table);
    sq_newslot(v, -3, SQTrue);

    sq_pushstring(v, _SC("_set"), -1);
    sq_pushobject(v, set_table);
    sq_newclosure(v, &sqVarSet, 1);
    sq_newslot(v, -3, SQFalse);

    sq_pushstring(v, _SC("_get"), -1);
    sq_pushobject(v, get_table);
    sq_newclosure(v, &sqVarGet, 1);
    sq_newslot(v, -3, SQFalse);

    sq_pushobject(v, get_table);
    sq_pushstring(v, _SC("x"), -1);
    sq_newclosure(v, squirrel_vector2_getx, 0);
    sq_setparamscheck(v, 1, _SC("x"));
    sq_newslot(v, -3, SQFalse);
    sq_pop(v, 1);

    sq_pushobject(v, set_table);
    sq_pushstring(v, _SC("x"), -1);
    sq_newclosure(v, squirrel_vector2_setx, 0);
    sq_setparamscheck(v, 2, _SC("x."));
    sq_newslot(v, -3, SQFalse);
    sq_pop(v, 1);

    sq_pushobject(v, get_table);
    sq_pushstring(v, _SC("y"), -1);
    sq_newclosure(v, squirrel_vector2_gety, 0);
    sq_setparamscheck(v, 1, _SC("x"));
    sq_newslot(v, -3, SQFalse);
    sq_pop(v, 1);

    sq_pushobject(v, set_table);
    sq_pushstring(v, _SC("y"), -1);
    sq_newclosure(v, squirrel_vector2_sety, 0);
    sq_setparamscheck(v, 2, _SC("x."));
    sq_newslot(v, -3, SQFalse);
    sq_pop(v, 1);

    sq_pushstring(v, _SC("constructor"), -1);
    sq_newclosure(v, squirrel_vector2_constructor, 0);
    sq_newslot(v, -3, SQFalse);

    sq_pushstring(v, _SC("normalized"), -1);
    sq_newclosure(v, squirrel_vector2_normalized, 0);
    sq_setparamscheck(v, 1, _SC("x"));
    sq_newslot(v, -3, SQFalse);

    sq_pushstring(v, _SC("dot"), -1);
    sq_newclosure(v, squirrel_vector2_dot, 0);
    sq_setparamscheck(v, 2, _SC("xx"));
    sq_newslot(v, -3, SQFalse);

    sq_pushstring(v, _SC("cross"), -1);
    sq_newclosure(v, squirrel_vector2_cross, 0);
    sq_setparamscheck(v, 2, _SC("xx"));
    sq_newslot(v, -3, SQFalse);

    sq_pushstring(v, _SC("project"), -1);
    sq_newclosure(v, squirrel_vector2_project, 0);
    sq_setparamscheck(v, 2, _SC("xx"));
    sq_newslot(v, -3, SQFalse);

    sq_pushstring(v, _SC("_add"), -1);
    sq_newclosure(v, squirrel_vector2_add, 0);
    sq_setparamscheck(v, 2, _SC("xx"));
    sq_newslot(v, -3, SQFalse);

    sq_pushstring(v, _SC("_sub"), -1);
    sq_newclosure(v, squirrel_vector2_sub, 0);
    sq_setparamscheck(v, 2, _SC("xx"));
    sq_newslot(v, -3, SQFalse);

    sq_pushstring(v, _SC("_mul"), -1);
    sq_newclosure(v, squirrel_vector2_mul, 0);
    sq_setparamscheck(v, 2, _SC("xn"));
    sq_newslot(v, -3, SQFalse);

    sq_pushstring(v, _SC("_div"), -1);
    sq_newclosure(v, squirrel_vector2_div, 0);
    sq_setparamscheck(v, 2, _SC("xn"));
    sq_newslot(v, -3, SQFalse);

    sq_pushstring(v, _SC("_unm"), -1);
    sq_newclosure(v, squirrel_vector2_unm, 0);
    sq_setparamscheck(v, 1, _SC("x"));
    sq_newslot(v, -3, SQFalse);

    sq_newslot(v, -3, SQFalse);

    sq_pushstring(v, _SC("Rect2"), -1);
    sq_newclass(v, SQFalse);
    sq_settypetag(v, -1, (SQUserPointer)"Rect2Tag");

    sq_resetobject(&set_table);
    sq_pushstring(v, _SC("__setTable"), -1);
    sq_newtable(v);
    sq_getstackobj(v, -1, &set_table);
    sq_addref(v, &set_table);
    sq_newslot(v, -3, SQTrue);

    sq_resetobject(&get_table);
    sq_pushstring(v, _SC("__getTable"), -1);
    sq_newtable(v);
    sq_getstackobj(v, -1, &get_table);
    sq_addref(v, &get_table);
    sq_newslot(v, -3, SQTrue);

    sq_pushstring(v, _SC("_set"), -1);
    sq_pushobject(v, set_table);
    sq_newclosure(v, &sqVarSet, 1);
    sq_newslot(v, -3, SQFalse);

    sq_pushstring(v, _SC("_get"), -1);
    sq_pushobject(v, get_table);
    sq_newclosure(v, &sqVarGet, 1);
    sq_newslot(v, -3, SQFalse);

    sq_pushobject(v, get_table);
    sq_pushstring(v, _SC("position"), -1);
    sq_newclosure(v, squirrel_rect2_getposition, 0);
    sq_setparamscheck(v, 1, _SC("x"));
    sq_newslot(v, -3, SQFalse);
    sq_pop(v, 1);

    sq_pushobject(v, set_table);
    sq_pushstring(v, _SC("position"), -1);
    sq_newclosure(v, squirrel_rect2_setposition, 0);
    sq_setparamscheck(v, 2, _SC("x."));
    sq_newslot(v, -3, SQFalse);
    sq_pop(v, 1);

    sq_pushobject(v, get_table);
    sq_pushstring(v, _SC("size"), -1);
    sq_newclosure(v, squirrel_rect2_getsize, 0);
    sq_setparamscheck(v, 1, _SC("x"));
    sq_newslot(v, -3, SQFalse);
    sq_pop(v, 1);

    sq_pushobject(v, set_table);
    sq_pushstring(v, _SC("size"), -1);
    sq_newclosure(v, squirrel_rect2_setsize, 0);
    sq_setparamscheck(v, 2, _SC("x."));
    sq_newslot(v, -3, SQFalse);
    sq_pop(v, 1);

    sq_pushstring(v, _SC("constructor"), -1);
    sq_newclosure(v, squirrel_rect2_constructor, 0);
    sq_newslot(v, -3, SQFalse);

    sq_newslot(v, -3, SQFalse);

    sq_pushstring(v, _SC("deg_to_rad"), -1);
    sq_newclosure(v, squirrel_math_degtorad, 0);
    sq_setparamscheck(v, 2, _SC(".n"));
    sq_newslot(v, -3, SQFalse);

    sq_pushstring(v, _SC("rad_to_deg"), -1);
    sq_newclosure(v, squirrel_math_radtodeg, 0);
    sq_setparamscheck(v, 2, _SC(".n"));
    sq_newslot(v, -3, SQFalse);
}