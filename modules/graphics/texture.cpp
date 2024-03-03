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

#include "texture.h"
#include "core.h"
#include <SDL2/SDL_image.h>

Texture::Texture(const char * path) {
    surface = IMG_Load(path);
    if(surface == nullptr)
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Unable to load image %s! (%s)", path, IMG_GetError());
}

Texture::~Texture() {
    SDL_FreeSurface(surface);
}

Vector2 Texture::get_size() {
    return Vector2(surface->w, surface->h);
}

void Texture::draw(const Vector2 & position, double angle, const Vector2 & origin, FlipMode flip) {
    SDL_FRect rect = { (float)origin.x, (float)origin.y, (float)(origin.x + get_size().x), (float)(origin.y + get_size().y) };
    GLuint gl_texture = load_texture(surface);
    glBindTexture(GL_TEXTURE_2D, gl_texture);
    glPushMatrix();
    glTranslated(position.x, position.y, 0.0);
    glRotated(angle * (180 / M_PI), 0.0, 0.0, 1.0);
    glTranslated(-2 * origin.x, -2 * origin.y, 0.0);
    glBegin(GL_QUADS);
    if(flip == NONE) {
        glTexCoord2d(0, 0); glVertex2d(rect.x, rect.y);
        glTexCoord2d(0, 1); glVertex2d(rect.x, rect.h);
        glTexCoord2d(1, 1); glVertex2d(rect.w, rect.h);
        glTexCoord2d(1, 0); glVertex2d(rect.w, rect.y);
    } else if(flip == HORIZONTAL) {
        glTexCoord2d(0, 0); glVertex2d(rect.w, rect.y);
        glTexCoord2d(0, 1); glVertex2d(rect.w, rect.h);
        glTexCoord2d(1, 1); glVertex2d(rect.x, rect.h);
        glTexCoord2d(1, 0); glVertex2d(rect.x, rect.y);
    } else if(flip == VERTICAL) {
        glTexCoord2d(0, 0); glVertex2d(rect.x, rect.h);
        glTexCoord2d(0, 1); glVertex2d(rect.x, rect.y);
        glTexCoord2d(1, 1); glVertex2d(rect.w, rect.y);
        glTexCoord2d(1, 0); glVertex2d(rect.w, rect.h);
    } else if(flip == BOTH) {
        glTexCoord2d(0, 0); glVertex2d(rect.w, rect.h);
        glTexCoord2d(0, 1); glVertex2d(rect.w, rect.y);
        glTexCoord2d(1, 1); glVertex2d(rect.x, rect.y);
        glTexCoord2d(1, 0); glVertex2d(rect.x, rect.h);
    }

    glEnd();
    glFlush();
    glPopMatrix();
    glBindTexture(GL_TEXTURE_2D, 0);
    glDeleteTextures(1, &gl_texture);
}