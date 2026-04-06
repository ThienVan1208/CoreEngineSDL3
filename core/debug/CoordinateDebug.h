#pragma once

#ifndef COORDINATE_DEBUG_H
#define COORDINATE_DEBUG_H

#include <SDL3/SDL.h>

class CoordinateDebug
{
public:
    // Control flags
    static bool drawZanGrid;
    static bool drawSDLGrid;
    static bool drawOrigins;

    // Grid spacing in pixels
    static float gridSpacing;

    // Colors
    static SDL_Color zanGridColor;      // Zan coordinate grid color (cyan)
    static SDL_Color zanOriginColor;    // Zan origin (0,0) at screen center (green)
    static SDL_Color sdlGridColor;      // SDL coordinate grid color (magenta)
    static SDL_Color sdlOriginColor;    // SDL origin (0,0) at top-left (red)
    static SDL_Color axisColor;         // Axis lines color (white)

    // Draw both coordinate axes with unit markers
    static void DrawCoordinateGrids(SDL_Renderer* renderer);

private:
    // Draw Zan coordinate axes (origin at screen center, Y-up)
    static void DrawZanAxes(SDL_Renderer* renderer);

    // Draw SDL coordinate axes (origin at top-left, Y-down)
    static void DrawSDLAxes(SDL_Renderer* renderer);

    // Draw origin points and axis lines
    static void DrawOrigins(SDL_Renderer* renderer);

    // Helper to draw a line
    static void DrawLine(SDL_Renderer* renderer, float x1, float y1, float x2, float y2, SDL_Color color);

    // Helper to draw a circle at a point
    static void DrawPoint(SDL_Renderer* renderer, float x, float y, float radius, SDL_Color color);
};

#endif // COORDINATE_DEBUG_H
