#include "FPSDebug.h"

std::unique_ptr<UIText> FPSDebug::fpsText = nullptr;
uint64_t FPSDebug::lastFpsTime = 0;
int FPSDebug::frameCount = 0;

void FPSDebug::Init()
{
    // Yellow text for debug
    fpsText = std::make_unique<UIText>(50, 50, SDL_Color{255, 255, 0, 255}, "FPS: 0"); 
    lastFpsTime = SDL_GetTicks();
    fpsText->rectTransform->scale = {3.5f, 3.5f}; // Make it bigger for visibility
    frameCount = 0;
}

void FPSDebug::Update()
{
    if (!fpsText) return;

    uint64_t currentTime = SDL_GetTicks();
    frameCount++;

    if (currentTime >= lastFpsTime + 1000)
    {
        fpsText->text = "FPS: " + std::to_string(frameCount);
        frameCount = 0;
        lastFpsTime = currentTime;
    }
}
