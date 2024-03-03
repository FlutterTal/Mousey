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

#include "vector2.h"
#include <math.h>

Vector2::Vector2() : x(0), y(0) {}

Vector2::Vector2(const Vector2 & other) : x(other.x), y(other.y) {}

Vector2::Vector2(double x, double y) : x(x), y(y) {}

Vector2 Vector2::normalized() {
    Vector2 v = *this;
    double l = v.x * v.x + v.y * v.y;
    if(l != 0) {
        l = sqrt(l);
        v.x /= l;
        v.y /= l;
    }

    return v;
}

double Vector2::dot(const Vector2 & with) {
    return x * with.x + y * with.y;
}

double Vector2::cross(const Vector2 & with) {
    return x * with.y - y * with.x;
}

Vector2 Vector2::project(const Vector2 & to) {
    Vector2 v = *this;
    v.x = to.x * (dot(to) / (to.x * to.x + to.y * to.y));
    v.y = to.y * (dot(to) / (to.x * to.x + to.y * to.y));
    return v;
}