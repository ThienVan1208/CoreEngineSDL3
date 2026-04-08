#pragma once

#ifndef COORDINATE_H
#define COORDINATE_H

#include "Screen.h"
#include <SDL3/SDL.h>

#include "math/Vector2.h"

// Zan Coordinate: Origin at center of screen, Y increases upward
// SDL Coordinate: Origin at top-left, Y increases downward
class CoordinateConverter
{
public:
    static Vector2 ZanToSDL(Vector2 zanPos);
    static Vector2 SDLToZan(Vector2 sdlPos);
};

#endif // COORDINATE_H
