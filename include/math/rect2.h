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

#ifndef RECT2_H
#define RECT2_H

#include "vector2.h"

struct Rect2 {
    Vector2 position;
    Vector2 size;

    Rect2();
    Rect2(const Rect2 & other);
    Rect2(const Vector2 & position, const Vector2 & size);
    Rect2(double x, double y, double width, double height);

    Rect2 intersect(const Rect2 & with);
    Rect2 union_rect(const Rect2 & with);
};

#endif