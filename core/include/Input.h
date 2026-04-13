#pragma once

#ifndef INPUT_H
#define INPUT_H
#include <SDL3/SDL.h>
#include <vector>
#include "math/Vector2.h"
class Input
{
private:
    static const bool* KeyBoard;
    static std::vector<bool> prekeyState;
    static int numKeys;

    static Uint32 mouseState;
    static Uint32 preMouseState;

public:
    static bool GetKey(SDL_Scancode key);
    static bool GetKeyDown(SDL_Scancode key);
    static bool GetKeyUp(SDL_Scancode key);

    static bool GetMouseButton(int button);
    static bool GetMouseButtonDown(int button);
    static bool GetMouseButtonUp(int button);

    static Vector2 GetMousePosition();

    void Init();
    void Update();

};


#endif