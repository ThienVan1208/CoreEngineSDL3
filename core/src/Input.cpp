#include "../include/Input.h"
#include "../include/Coordinate.h"

const bool* Input::KeyBoard = nullptr;
std::vector<bool> Input::prekeyState;
int Input::numKeys;
Uint32 Input::mouseState = 0;
Uint32 Input::preMouseState = 0;

void Input::Init()
{
    int numKeysOut;
    KeyBoard = SDL_GetKeyboardState(&numKeysOut);
    numKeys = numKeysOut; // ensure numKeys is set
    prekeyState.resize(numKeys, false);
    mouseState = SDL_GetMouseState(nullptr, nullptr);
    preMouseState = mouseState;
}

void Input::Update()
{
    // Update keyboard
    for (int i = 0; i < numKeys; ++i)
    {
        prekeyState[i] = KeyBoard[i];
    }
    
    // Update mouse
    preMouseState = mouseState;
    mouseState = SDL_GetMouseState(nullptr, nullptr);
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

bool Input::GetMouseButton(int button)
{
    Uint32 mask = SDL_BUTTON_MASK(button);
    return (mouseState & mask) != 0;
}

bool Input::GetMouseButtonDown(int button)
{
    Uint32 mask = SDL_BUTTON_MASK(button);
    return (mouseState & mask) != 0 && (preMouseState & mask) == 0;
}

bool Input::GetMouseButtonUp(int button)
{
    Uint32 mask = SDL_BUTTON_MASK(button);
    return (mouseState & mask) == 0 && (preMouseState & mask) != 0;
}

Vector2 Input::GetMousePosition()
{
    float mouseX, mouseY;
    SDL_GetMouseState(&mouseX, &mouseY);

    return CoordinateConverter::SDLToZan(Vector2(mouseX, mouseY));
}