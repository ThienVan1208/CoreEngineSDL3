#include "../include/Coordinate.h"
#include "../include/Screen.h"

// Zan Coordinate: Origin at center of screen, Y increases upward
// SDL Coordinate: Origin at top-left, Y increases downward
// All positions are CENTER positions until rendering

// Pure coordinate system conversion (center positions in both systems)
float CoordinateConverter::ZanToSDL_X(float zanX)
{
    return zanX + Screen::GetWidth() / 2.0f;
}

float CoordinateConverter::ZanToSDL_Y(float zanY)
{
    return Screen::GetHeight() / 2.0f - zanY;
}

float CoordinateConverter::SDLToZan_X(float sdlX)
{
    return sdlX - Screen::GetWidth() / 2.0f;
}

float CoordinateConverter::SDLToZan_Y(float sdlY)
{
    return Screen::GetHeight() / 2.0f - sdlY;
}

// Rendering conversion: Zan center position -> SDL rect with top-left corner
SDL_FRect CoordinateConverter::ZanCenterToSDLRect(float zanCenterX, float zanCenterY, float width, float height)
{
    // Step 1: Convert center position from Zan to SDL coordinate system
    float sdlCenterX = ZanToSDL_X(zanCenterX);
    float sdlCenterY = ZanToSDL_Y(zanCenterY);
    
    // Step 2: Convert from center to top-left corner (for SDL rendering)
    SDL_FRect rect;
    rect.x = sdlCenterX - width / 2.0f;
    rect.y = sdlCenterY - height / 2.0f;
    rect.w = width;
    rect.h = height;
    
    return rect;
}
