#pragma once

#ifndef TIME_H
#define TIME_H

#include <SDL3/SDL.h>

class Time
{
private:
    static Uint64 lastTime;
    static Uint64 perfFreq;

public:
    static float deltaTime;

    static void Init();
    static void Update();
};

#endif // TIME_H
