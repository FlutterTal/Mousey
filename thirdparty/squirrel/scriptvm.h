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
#include <stdint.h>

#define SQUSEDOUBLE

class ScriptVM {
    HSQUIRRELVM v;

    void push_arg(SQInteger i) { sq_pushinteger(v, i); }
    void push_arg(int64_t i) { sq_pushinteger(v, i); }
    void push_arg(SQFloat f) { sq_pushfloat(v, f); }
    void push_arg(double f) { sq_pushfloat(v, f); }
    void push_arg(SQBool b) { sq_pushbool(v, b); }
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
};

inline SQInteger sqVarGet(HSQUIRRELVM v) {
    sq_push(v, 2);
    if(SQ_FAILED(sq_rawget(v, -2))) {
        sq_pushnull(v);
        return sq_throwobject(v);
    }

    sq_push(v, 1);
    SQRESULT result = sq_call(v, 1, SQTrue, SQTrue);
    return SQ_SUCCEEDED(result) ? 1 : SQ_ERROR;
}

inline SQInteger sqVarSet(HSQUIRRELVM v) {
    sq_push(v, 2);
    if(SQ_FAILED(sq_rawget(v, -2))) {
        sq_pushnull(v);
        return sq_throwobject(v);
    }

    sq_push(v, 1);
    sq_push(v, 3);
    SQRESULT result = sq_call(v, 2, SQFalse, SQTrue);
    return SQ_SUCCEEDED(result) ? 0 : SQ_ERROR;
}

#endif