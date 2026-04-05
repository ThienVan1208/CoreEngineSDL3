#include "../include/Input.h"

const bool* Input::KeyBoard = nullptr;
std::vector<bool> Input::prekeyState;
int Input::numKeys;


void Input::Init()
{
    KeyBoard = SDL_GetKeyboardState(&numKeys);
    prekeyState.resize(numKeys, false);
}

void Input::Update()
{
    for (int i = 0; i < numKeys; ++i)
    {
        prekeyState[i] = KeyBoard[i];
    }
}

bool Input::GetKey(SDL_Scancode key)
{
    if (key < 0 || key >= numKeys) return false;
    return KeyBoard[key];
}

bool Input::GetKeyDown(SDL_Scancode key)
{
    if (key < 0 || key >= numKeys) return false;
    return KeyBoard[key] && !prekeyState[key];
}

bool Input::GetKeyUp(SDL_Scancode key)
{
    if (key < 0 || key >= numKeys) return false;
    return !KeyBoard[key] && prekeyState[key];
}