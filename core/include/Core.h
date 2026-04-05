#pragma once

#ifndef CORE_H
#define CORE_H

#include <SDL3/SDL.h>

class UIManager;
class ObjectManager;
class SpriteManager;
class Input;

class Core{
public:
    static SDL_Renderer* renderer;
    UIManager* uiManager;
    ObjectManager* objectManager;
    SpriteManager* spriteManager;
    Input* input;

    Core();
    Core(SDL_Renderer* rend);

    ~Core();
    void OnUpdate();
    void OnRender();
};

#endif // CORE_H