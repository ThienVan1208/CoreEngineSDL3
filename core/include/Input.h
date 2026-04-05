#pragma once

#ifndef INPUT_H
#define INPUT_H
#include <SDL3/SDL.h>
#include <vector>
class Input
{
private:
    static const bool* KeyBoard;
    static std::vector<bool> prekeyState;
    static int numKeys;

public:
    static bool GetKey(SDL_Scancode key);
    static bool GetKeyDown(SDL_Scancode key);
    static bool GetKeyUp(SDL_Scancode key);

    void Init();
    void Update();

};


#endif