#include "../include/Coordinate.h"
#include "../include/Screen.h"
#include "../include/components/Component.h"
#include "../include/setting/Setting.h"

// Zan Coordinate: Origin at center of screen, Y increases upward
// SDL Coordinate: Origin at top-left, Y increases downward

Vector2 CoordinateConverter::ZanToSDL(Vector2 zanPos)
{
    return Vector2(
        zanPos.x * Setting::BASE_UNIT + Screen::GetWidth() / 2.0f,
        Screen::GetHeight() / 2.0f - zanPos.y * Setting::BASE_UNIT
    );
}

Vector2 CoordinateConverter::SDLToZan(Vector2 sdlPos)
{
    return Vector2(
        (sdlPos.x - Screen::GetWidth() / 2.0f) / Setting::BASE_UNIT,
        (Screen::GetHeight() / 2.0f - sdlPos.y) / Setting::BASE_UNIT
    );
}
