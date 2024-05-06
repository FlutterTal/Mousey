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

#include "graphics_wrapper.h"
#include "graphics/font.h"
#include "math/rect2.h"
#include "math/vector2.h"
#include <GL/glew.h>
#include <math.h>

static Font * default_font = new Font("/usr/share/fonts/noto/NotoSans-Condensed.ttf", 12);

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

static SQInteger squirrel_graphics_fillrectangle(HSQUIRRELVM v) {
    Rect2 * rectangle;
    sq_getinstanceup(v, 2, (SQUserPointer *)&rectangle, (SQUserPointer)"Rect2Tag", SQTrue);
    glBegin(GL_QUADS);
    glVertex2d(rectangle->position.x, rectangle->position.y);
    glVertex2d(rectangle->position.x + rectangle->size.x, rectangle->position.y);
    glVertex2d(rectangle->position.x + rectangle->size.x, rectangle->position.y + rectangle->size.y);
    glVertex2d(rectangle->position.x, rectangle->position.y + rectangle->size.y);
    glEnd();
    glFlush();
    return 0;
}

static SQInteger squirrel_graphics_drawrectangle(HSQUIRRELVM v) {
    Rect2 * rectangle;
    sq_getinstanceup(v, 2, (SQUserPointer *)&rectangle, (SQUserPointer)"Rect2Tag", SQTrue);
    glBegin(GL_LINE_LOOP);
    glVertex2d(rectangle->position.x, rectangle->position.y);
    glVertex2d(rectangle->position.x + rectangle->size.x, rectangle->position.y);
    glVertex2d(rectangle->position.x + rectangle->size.x, rectangle->position.y + rectangle->size.y);
    glVertex2d(rectangle->position.x, rectangle->position.y + rectangle->size.y);
    glEnd();
    glFlush();
    return 0;
}

static SQInteger squirrel_graphics_fillcircle(HSQUIRRELVM v) {
    Vector2 * position;
    SQFloat radius;
    sq_getinstanceup(v, 2, (SQUserPointer *)&position, (SQUserPointer)"Vector2Tag", SQTrue);
    if(SQ_FAILED(sq_getfloat(v, 3, &radius)))
        return sq_throwerror(v, _SC("Argument 2 not a float"));
    
    glBegin(GL_TRIANGLE_FAN);
    for(int i = 0; i <= 500; i++) {
        double angle = 2 * M_PI * i / 500;
        glVertex2d(position->x + cos(angle) * radius, position->y + sin(angle) * radius);
    }

    glEnd();
    glFlush();
    return 0;
}

static SQInteger squirrel_graphics_drawcircle(HSQUIRRELVM v) {
    Vector2 * position;
    SQFloat radius;
    sq_getinstanceup(v, 2, (SQUserPointer *)&position, (SQUserPointer)"Vector2Tag", SQTrue);
    if(SQ_FAILED(sq_getfloat(v, 3, &radius)))
        return sq_throwerror(v, _SC("Argument 2 not a float"));
    
    glBegin(GL_LINE_LOOP);
    for(int i = 0; i <= 500; i++) {
        double angle = 2 * M_PI * i / 500;
        glVertex2d(position->x + cos(angle) * radius, position->y + sin(angle) * radius);
    }

    glEnd();
    glFlush();
    return 0;
}

static SQInteger squirrel_graphics_drawline(HSQUIRRELVM v) {
    Vector2 * start;
    Vector2 * end;
    sq_getinstanceup(v, 2, (SQUserPointer *)&start, (SQUserPointer)"Vector2Tag", SQTrue);
    sq_getinstanceup(v, 3, (SQUserPointer *)&end, (SQUserPointer)"Vector2Tag", SQTrue);
    glBegin(GL_LINES);
    glVertex2d(start->x, start->y);
    glVertex2d(end->x, end->y);
    glEnd();
    glFlush();
    return 0;
}

static SQInteger squirrel_graphics_drawtext(HSQUIRRELVM v) {
    const SQChar * text;
    Vector2 * position;
    Font * font = default_font;
    if(SQ_FAILED(sq_getstring(v, 2, &text)))
        return sq_throwerror(v, _SC("Argument 1 not a string"));
    
    sq_getinstanceup(v, 3, (SQUserPointer *)&position, (SQUserPointer)"Vector2Tag", SQTrue);
    if(sq_gettop(v) > 3)
        sq_getinstanceup(v, 4, (SQUserPointer *)&font, (SQUserPointer)"FontTag", SQTrue);
    
    SDL_Surface * surface = TTF_RenderUTF8_Blended(font->get_font(), text, { 255, 255, 255 });
    SDL_FRect rect = { (float)position->x, (float)position->y, (float)(position->x + surface->w), (float)(position->y + surface->h) };
    GLuint texture = load_texture(surface);
    SDL_FreeSurface(surface);
    glBindTexture(GL_TEXTURE_2D, texture);
    glBegin(GL_QUADS);
    glTexCoord2d(0, 0); glVertex2d(rect.x, rect.y);
    glTexCoord2d(0, 1); glVertex2d(rect.x, rect.h);
    glTexCoord2d(1, 1); glVertex2d(rect.w, rect.h);
    glTexCoord2d(1, 0); glVertex2d(rect.w, rect.y);
    glEnd();
    glFlush();
    glBindTexture(GL_TEXTURE_2D, 0);
    glDeleteTextures(1, &texture);
    return 0;
}

void register_graphics_wrapper(HSQUIRRELVM v) {
    sq_pushstring(v, _SC("fill_rectangle"), -1);
    sq_newclosure(v, squirrel_graphics_fillrectangle, 0);
    sq_setparamscheck(v, 2, _SC(".x"));
    sq_newslot(v, -3, SQFalse);

    sq_pushstring(v, _SC("draw_rectangle"), -1);
    sq_newclosure(v, squirrel_graphics_drawrectangle, 0);
    sq_setparamscheck(v, 2, _SC(".x"));
    sq_newslot(v, -3, SQFalse);

    sq_pushstring(v, _SC("fill_circle"), -1);
    sq_newclosure(v, squirrel_graphics_fillcircle, 0);
    sq_setparamscheck(v, 3, _SC(".xn"));
    sq_newslot(v, -3, SQFalse);

    sq_pushstring(v, _SC("draw_circle"), -1);
    sq_newclosure(v, squirrel_graphics_drawcircle, 0);
    sq_setparamscheck(v, 3, _SC(".xn"));
    sq_newslot(v, -3, SQFalse);

    sq_pushstring(v, _SC("draw_line"), -1);
    sq_newclosure(v, squirrel_graphics_drawline, 0);
    sq_setparamscheck(v, 3, _SC(".xx"));
    sq_newslot(v, -3, SQFalse);

    sq_pushstring(v, _SC("draw_text"), -1);
    sq_newclosure(v, squirrel_graphics_drawtext, 0);
    sq_setparamscheck(v, -3, _SC(".sxx"));
    sq_newslot(v, -3, SQFalse);
}