#include "CoordinateDebug.h"
#include "../include/Screen.h"
#include "../include/setting/Setting.h"
#include <cmath>

// Initialize static members
bool CoordinateDebug::drawZanGrid = true;
bool CoordinateDebug::drawSDLGrid = true;
bool CoordinateDebug::drawOrigins = true;
float CoordinateDebug::gridSpacing = Setting::BASE_UNIT;

SDL_Color CoordinateDebug::zanGridColor = {0, 255, 255, 200};        // Cyan
SDL_Color CoordinateDebug::zanOriginColor = {0, 255, 0, 255};        // Green
SDL_Color CoordinateDebug::sdlGridColor = {255, 0, 255, 200};        // Magenta
SDL_Color CoordinateDebug::sdlOriginColor = {255, 0, 0, 255};        // Red
SDL_Color CoordinateDebug::axisColor = {255, 255, 255, 255};         // White

void CoordinateDebug::DrawCoordinateGrids(SDL_Renderer* renderer)
{
    if (drawZanGrid)
        DrawZanAxes(renderer);

    if (drawSDLGrid)
        DrawSDLAxes(renderer);

    if (drawOrigins)
        DrawOrigins(renderer);
}

void CoordinateDebug::DrawZanAxes(SDL_Renderer* renderer)
{
    float screenWidth = Screen::GetWidth();
    float screenHeight = Screen::GetHeight();
    float centerX = screenWidth / 2.0f;
    float centerY = screenHeight / 2.0f;

    // Draw X-axis (horizontal line through center)
    DrawLine(renderer, 0, centerY, screenWidth, centerY, zanGridColor);

    // Draw Y-axis (vertical line through center)
    DrawLine(renderer, centerX, 0, centerX, screenHeight, zanGridColor);

    // Mark units on X-axis
    for (float x = centerX; x < screenWidth; x += gridSpacing)
    {
        DrawLine(renderer, x, centerY - 5, x, centerY + 5, zanGridColor);
    }
    for (float x = centerX; x > 0; x -= gridSpacing)
    {
        DrawLine(renderer, x, centerY - 5, x, centerY + 5, zanGridColor);
    }

    // Mark units on Y-axis
    for (float y = centerY; y < screenHeight; y += gridSpacing)
    {
        DrawLine(renderer, centerX - 5, y, centerX + 5, y, zanGridColor);
    }
    for (float y = centerY; y > 0; y -= gridSpacing)
    {
        DrawLine(renderer, centerX - 5, y, centerX + 5, y, zanGridColor);
    }
}

void CoordinateDebug::DrawSDLAxes(SDL_Renderer* renderer)
{
    float screenWidth = Screen::GetWidth();
    float screenHeight = Screen::GetHeight();

    // Draw X-axis (horizontal line at top)
    DrawLine(renderer, 0, 0, screenWidth, 0, sdlGridColor);

    // Draw Y-axis (vertical line at left)
    DrawLine(renderer, 0, 0, 0, screenHeight, sdlGridColor);

    // Mark units on X-axis (spacing along the top)
    for (float x = 0; x < screenWidth; x += gridSpacing)
    {
        DrawLine(renderer, x, 0, x, 10, sdlGridColor);
    }

    // Mark units on Y-axis (spacing along the left)
    for (float y = 0; y < screenHeight; y += gridSpacing)
    {
        DrawLine(renderer, 0, y, 10, y, sdlGridColor);
    }
}

void CoordinateDebug::DrawOrigins(SDL_Renderer* renderer)
{
    float screenWidth = Screen::GetWidth();
    float screenHeight = Screen::GetHeight();
    float centerX = screenWidth / 2.0f;
    float centerY = screenHeight / 2.0f;

    // Draw Zan origin (center of screen, green)
    DrawPoint(renderer, centerX, centerY, 5.0f, zanOriginColor);

    // Draw SDL origin (top-left, red)
    DrawPoint(renderer, 0, 0, 5.0f, sdlOriginColor);
}

void CoordinateDebug::DrawLine(SDL_Renderer* renderer, float x1, float y1, float x2, float y2, SDL_Color color)
{
    SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a);
    SDL_RenderLine(renderer, x1, y1, x2, y2);
}

void CoordinateDebug::DrawPoint(SDL_Renderer* renderer, float x, float y, float radius, SDL_Color color)
{
    SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a);

    // Draw filled circle using Bresenham-like algorithm
    for (float angle = 0; angle < 360; angle += 10)
    {
        float rad = angle * 3.14159f / 180.0f;
        float x1 = x + radius * std::cos(rad);
        float y1 = y + radius * std::sin(rad);

        float nextAngle = (angle + 10) * 3.14159f / 180.0f;
        float x2 = x + radius * std::cos(nextAngle);
        float y2 = y + radius * std::sin(nextAngle);

        SDL_RenderLine(renderer, x1, y1, x2, y2);
    }
}
