#include "../include/Time.h"

Uint64 Time::lastTime = 0;
Uint64 Time::perfFreq = 0;
float Time::deltaTime = 0.0f;

void Time::Init()
{
    perfFreq = SDL_GetPerformanceFrequency();
    lastTime = SDL_GetPerformanceCounter();
}

void Time::Update()
{
    Uint64 currentTime = SDL_GetPerformanceCounter();
    // High-resolution delta time in seconds
    deltaTime = static_cast<float>(currentTime - lastTime) / perfFreq; 
    
    // Cap delta time to avoid huge physics jumps if the window is dragged or suspended
    if (deltaTime > 0.1f) {
        deltaTime = 0.1f;
    }
    
    lastTime = currentTime;
}
