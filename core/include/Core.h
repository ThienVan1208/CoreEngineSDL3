#pragma once

#ifndef CORE_H
#define CORE_H

#include <SDL3/SDL.h>
#include "PhysicManager.h"

class RenderManager;
class ObjectManager;
class BehaviorManager;
class Input;

class Core{
public:
    static SDL_Renderer* renderer;
    static Physic* physicsManager;
    
    RenderManager* renderManager;
    ObjectManager* objectManager;
    BehaviorManager* behaviorManager;
    Input* input;

    Core();
    Core(SDL_Renderer* rend);

    ~Core();
    void OnUpdate();
    void OnRender();
};

#endif // CORE_H