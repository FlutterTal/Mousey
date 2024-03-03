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

#ifndef SQUIRREL_SCRIPTVM_H
#define SQUIRREL_SCRIPTVM_H

#include <squirrel.h>

#define SQUSEDOUBLE

class ScriptVM {
    HSQUIRRELVM v;

    void push_arg(SQInteger i) { sq_pushinteger(v, i); }
    void push_arg(SQFloat f) { sq_pushfloat(v, f); }
    void push_arg(const SQChar * s) { sq_pushstring(v, s, -1); }

public:
    ScriptVM();
    ~ScriptVM();

    template<typename... Args>
    void call_func_without_return(const SQChar * name, Args... args) {
        sq_pushroottable(v);
        sq_pushstring(v, _SC(name), -1);
        if(SQ_SUCCEEDED(sq_get(v, -2))) {
            sq_pushroottable(v);
            (push_arg(args), ...);
            if(SQ_FAILED(sq_call(v, sizeof...(args) + 1, SQFalse, SQTrue))) {
                sq_pop(v, 2);
                return;
            }

            sq_pop(v, 1);
        }

        sq_pop(v, 1);
    }

    template<typename R, typename... Args>
    R call_func_with_return(const SQChar * name, R default_return, Args... args) {
        sq_pushroottable(v);
        sq_pushstring(v, _SC(name), -1);
        if(SQ_SUCCEEDED(sq_get(v, -2))) {
            sq_pushroottable(v);
            (push_arg(args), ...);
            if(SQ_FAILED(sq_call(v, sizeof...(args) + 1, SQTrue, SQTrue))) {
                sq_pop(v, 2);
                return default_return;
            }

            switch(sq_gettype(v, -1)) {
            case OT_INTEGER: {
                SQInteger i;
                sq_getinteger(v, -1, &i);
                sq_pop(v, 3);
                return i;
            }
            case OT_FLOAT: {
                SQFloat f;
                sq_getfloat(v, -1, &f);
                sq_pop(v, 3);
                return f;
            }
            case OT_BOOL: {
                SQBool b;
                sq_getbool(v, -1, &b);
                sq_pop(v, 3);
                return b;
            }
            default:
                sq_pop(v, 3);
                return default_return;
            }
        }

        sq_pop(v, 1);
        return default_return;
    }
};

#endif