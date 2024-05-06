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

#include "viewport/window.h"
#include <SDL2/SDL_image.h>

Window::Window(const char * title, int w, int h, const char * icon_path, bool resizable, bool always_on_top, bool borderless, bool fullscreen) {
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 1);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
    window = SDL_CreateWindow(title, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, w, h, SDL_WINDOW_SHOWN | SDL_WINDOW_OPENGL);
    if(window == nullptr) {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Could not create the window! (%s)", SDL_GetError());
        return;
    }

    context = SDL_GL_CreateContext(window);
    if(context == nullptr) {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Could not create OpenGL context! (%s)", SDL_GetError());
        SDL_DestroyWindow(window);
        return;
    }

    if(icon_path != nullptr && icon_path != "") {
        SDL_Surface * surface = IMG_Load(icon_path);
        SDL_SetWindowIcon(window, surface);
        SDL_FreeSurface(surface);
    }

    SDL_SetWindowResizable(window, resizable ? SDL_TRUE : SDL_FALSE);
    SDL_SetWindowAlwaysOnTop(window, always_on_top ? SDL_TRUE : SDL_FALSE);
    SDL_SetWindowBordered(window, borderless ? SDL_FALSE : SDL_TRUE);
    SDL_SetWindowFullscreen(window, fullscreen ? SDL_WINDOW_FULLSCREEN_DESKTOP : 0);
}

Window::~Window() {
    SDL_GL_DeleteContext(context);
    SDL_DestroyWindow(window);
}

void Window::swap() {
    SDL_GL_SwapWindow(window);
}