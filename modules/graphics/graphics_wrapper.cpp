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
#include "core.h"
#include "font.h"
#include "texture.h"
#include "modules/math/rect2.h"
#include "modules/math/vector2.h"
#include <GL/glew.h>
#include <math.h>

static Font * default_font = new Font("/usr/share/fonts/google-noto-vf/NotoSans[wght].ttf", 12);

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

static SQInteger squirrel_font_destructor(SQUserPointer p, SQInteger SQ_UNUSED_ARG(size)) {
    Font * instance = reinterpret_cast<Font *>(p);
    delete instance;
    return 0;
}

static SQInteger squirrel_font_constructor(HSQUIRRELVM v) {
    const SQChar * path;
    SQInteger size;
    if(SQ_FAILED(sq_getstring(v, 2, &path)))
        return sq_throwerror(v, _SC("Argument 1 not a string"));

    if(SQ_FAILED(sq_getinteger(v, 3, &size)))
        return sq_throwerror(v, _SC("Argument 1 not an integer"));

    Font * instance = new Font(path, size);
    sq_setinstanceup(v, 1, instance);
    sq_setreleasehook(v, 1, squirrel_font_destructor);
    return 0;
}

static SQInteger squirrel_texture_destructor(SQUserPointer p, SQInteger SQ_UNUSED_ARG(size)) {
    Texture * instance = reinterpret_cast<Texture *>(p);
    delete instance;
    return 0;
}

static SQInteger squirrel_texture_constructor(HSQUIRRELVM v) {
    const SQChar * path;
    if(SQ_FAILED(sq_getstring(v, 2, &path)))
        return sq_throwerror(v, _SC("Argument 1 not a string"));

    Texture * instance = new Texture(path);
    sq_setinstanceup(v, 1, instance);
    sq_setreleasehook(v, 1, squirrel_texture_destructor);
    return 0;
}

static SQInteger squirrel_texture_draw(HSQUIRRELVM v) {
    Texture * instance;
    sq_getinstanceup(v, 1, (SQUserPointer *)&instance, (SQUserPointer)"TextureTag", SQTrue);
    Vector2 * position = new Vector2();
    SQFloat angle = 0.0;
    Vector2 * origin = new Vector2();
    FlipMode flip = FlipMode::NONE;
    if(sq_gettop(v) > 1) {
        sq_getinstanceup(v, 2, (SQUserPointer *)&position, (SQUserPointer)"Vector2Tag", SQTrue);
        if(sq_gettop(v) > 2) {
            if(SQ_FAILED(sq_getfloat(v, 3, &angle)))
                return sq_throwerror(v, _SC("Argument 2 not a float"));

            if(sq_gettop(v) > 3) {
                sq_getinstanceup(v, 4, (SQUserPointer *)&origin, (SQUserPointer)"Vector2Tag", SQTrue);
                if(sq_gettop(v) > 4) {
                    if(SQ_FAILED(sq_getinteger(v, 5, (SQInteger *)&flip)))
                        return sq_throwerror(v, _SC("Argument 4 not a flip mode"));
                }
            }
        }
    }

    instance->draw(*position, angle, *origin, flip);
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

    sq_pushstring(v, _SC("Font"), -1);
    sq_newclass(v, SQFalse);
    sq_settypetag(v, -1, (SQUserPointer)"FontTag");

    sq_pushstring(v, _SC("constructor"), -1);
    sq_newclosure(v, squirrel_font_constructor, 0);
    sq_setparamscheck(v, 3, _SC(".si"));
    sq_newslot(v, -3, SQFalse);

    sq_newslot(v, -3, SQFalse);

    sq_pushstring(v, _SC("Texture"), -1);
    sq_newclass(v, SQFalse);
    sq_settypetag(v, -1, (SQUserPointer)"TextureTag");

    sq_pushstring(v, _SC("constructor"), -1);
    sq_newclosure(v, squirrel_texture_constructor, 0);
    sq_setparamscheck(v, 2, _SC(".s"));
    sq_newslot(v, -3, SQFalse);

    sq_pushstring(v, _SC("draw"), -1);
    sq_newclosure(v, squirrel_texture_draw, 0);
    sq_setparamscheck(v, -1, _SC("xxnxi"));
    sq_newslot(v, -3, SQFalse);

    sq_newslot(v, -3, SQFalse);
}