#pragma once

#ifndef CORE_H
#define CORE_H

#include <SDL3/SDL.h>
#include "PhysicManager.h"

class UIManager;
class ObjectManager;
class BehaviorManager;
class SpriteManager;
class Input;

class Core{
public:
    static SDL_Renderer* renderer;
    static Physic* physicsManager;
    
    UIManager* uiManager;
    ObjectManager* objectManager;
    BehaviorManager* behaviorManager;
    SpriteManager* spriteManager;
    Input* input;

    Core();
    Core(SDL_Renderer* rend);

    ~Core();
    void OnUpdate();
    void OnRender();
};

#endif // CORE_H