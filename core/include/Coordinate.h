#pragma once

#ifndef COORDINATE_H
#define COORDINATE_H

#include "Screen.h"
#include <SDL3/SDL.h>
// Zan Coordinate System: Origin at center of screen, Y increases upward, all positions are CENTER positions
// SDL Coordinate System: Origin at top-left, Y increases downward, rects use TOP-LEFT positions

class CoordinateConverter
{
public:
    // Pure coordinate system conversion (only origin and Y-direction, no size offset)
    // Input/Output: center positions in respective coordinate systems
    static float ZanToSDL_X(float zanX);
    static float ZanToSDL_Y(float zanY);
    
    static float SDLToZan_X(float sdlX);
    static float SDLToZan_Y(float sdlY);
    
    // Rendering conversion: Zan center position -> SDL top-left rect position
    // This accounts for both coordinate system change AND center->corner conversion
    // Use this when you need to render an object
    static SDL_FRect ZanCenterToSDLRect(float zanCenterX, float zanCenterY, float width, float height);
    static SDL_FRect ZanCenterToSDLRect(struct Vector2 zanCenter, float width, float height);
    
    // Helper functions for Vector2
    struct Vector2;
    static Vector2 ZanToSDL(Vector2 zanPos);
    static Vector2 SDLToZan(Vector2 sdlPos);
};

#endif // COORDINATE_H
