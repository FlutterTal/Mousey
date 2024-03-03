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

#include "core.h"

uint8_t * current_keyboard_state = nullptr;
uint8_t * previous_keyboard_state = nullptr;
uint32_t current_mouse_state = 0;
uint32_t previous_mouse_state = 0;

GLuint load_texture(SDL_Surface * surface) {
    SDL_Surface * image = SDL_CreateRGBSurface(0, surface->w, surface->h, 32, 0x000000FF, 0x0000FF00, 0x00FF0000, 0xFF000000);
    Uint8 alpha;
    SDL_BlendMode blend;
    SDL_GetSurfaceAlphaMod(surface, &alpha);
    SDL_SetSurfaceAlphaMod(surface, 0xFF);
    SDL_GetSurfaceBlendMode(surface, &blend);
    SDL_SetSurfaceBlendMode(surface, SDL_BLENDMODE_NONE);
    SDL_Rect area = { 0, 0, surface->w, surface->h };
    SDL_BlitSurface(surface, &area, image, &area);
    SDL_SetSurfaceAlphaMod(surface, alpha);
    SDL_SetSurfaceBlendMode(surface, blend);
    GLuint texture;
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, surface->w, surface->h, 0, GL_RGBA, GL_UNSIGNED_BYTE, image->pixels);
    SDL_FreeSurface(image);
    return texture;
}