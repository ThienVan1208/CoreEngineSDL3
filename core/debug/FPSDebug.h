#pragma once

#ifndef FPS_DEBUG_H
#define FPS_DEBUG_H

#include <SDL3/SDL.h>
#include <memory>
#include "../include/UIElements.h"

class FPSDebug
{
public:
    static void Init();
    static void Update();

private:
    static std::unique_ptr<UIText> fpsText;
    static uint64_t lastFpsTime;
    static int frameCount;
};

#endif // FPS_DEBUG_H
