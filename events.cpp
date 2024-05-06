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

#include "events.h"
#include "engine.h"
#include "core.h"

void Event::poll() {
    while(SDL_PollEvent(&event)) {
        switch(event.type) {
        case SDL_QUIT:
            Engine::get_singleton()->get_window()->close_window();
            break;
        case SDL_KEYDOWN:
            current_keyboard_state[event.key.keysym.scancode] = 1;
            break;
        case SDL_KEYUP:
            current_keyboard_state[event.key.keysym.scancode] = 0;
            break;
        case SDL_MOUSEBUTTONDOWN:
            current_mouse_state |= SDL_BUTTON(event.button.button);
            break;
        case SDL_MOUSEBUTTONUP:
            current_mouse_state &= ~SDL_BUTTON(event.button.button);
            break;
        }
    }
}