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

#include "math/rect2.h"
#include <math.h>

Rect2::Rect2() : position(Vector2()), size(Vector2()) {}

Rect2::Rect2(const Rect2 & other) : position(other.position), size(other.size) {}

Rect2::Rect2(const Vector2 & position, const Vector2 & size) : position(position), size(size) {}

Rect2::Rect2(double x, double y, double width, double height) : position(Vector2(x, y)), size(Vector2(width, height)) {}

Rect2 Rect2::intersect(const Rect2 & with) {
    if(!(position.x <= (with.position.x + with.size.x) && (position.x + size.x) >= with.position.x && position.y <= (with.position.y + with.size.y) && (position.y + size.y) >= with.position.y))
        return Rect2();
    
    double max_x = (position.x >= with.position.x) ? position.x : with.position.x;
    double max_y = (position.y >= with.position.y) ? position.y : with.position.y;
    double min_width = (position.x + size.x <= with.position.x + with.size.x) ? position.x + size.x : with.position.x + with.size.x;
    double min_height = (position.y + size.y <= with.position.y + with.size.y) ? position.y + size.y : with.position.y + with.size.y;
    return Rect2(max_x, max_y, min_width - max_x, min_height - max_y);
}

Rect2 Rect2::union_rect(const Rect2 & with) {
    double min_x = (position.x <= with.position.x) ? position.x : with.position.x;
    double min_y = (position.y <= with.position.y) ? position.y : with.position.y;
    double max_width = (position.x + size.x >= with.position.x + with.size.x) ? position.x + size.x : with.position.x + with.size.x;
    double max_height = (position.y + size.y >= with.position.y + with.size.y) ? position.y + size.y : with.position.y + with.size.y;
    return Rect2(min_x, min_y, max_width - min_x, max_height - min_y);
}