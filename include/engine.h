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

#ifndef ENGINE_H
#define ENGINE_H

#include "events.h"
#include "thirdparty/squirrel/scriptvm.h"
#include "modules/viewport/window.h"

class Engine {
    static Engine singleton;
    ScriptVM vm;
    Window * window;
    Event event;
    SQFloat accumulator = 0;
    const SQFloat fixed_dt = 1.0 / 60.0;

    Engine();
    ~Engine();

public:
    Engine(const Engine &) = delete;
    void operator=(const Engine &) = delete;

    static Engine * get_singleton();

    void run();
    Window * get_window() const { return window; }
    ScriptVM get_vm() const { return vm; }
};

#endif