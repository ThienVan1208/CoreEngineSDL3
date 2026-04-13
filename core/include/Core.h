#pragma once

#ifndef CORE_H
#define CORE_H

#include <SDL3/SDL.h>
#include <memory>
#include "PhysicManager.h"

class RenderManager;
class ObjectManager;
class BehaviorManager;
class Input;

class Core{
public:
    static SDL_Renderer* renderer;
    static std::unique_ptr<Physic> physicsManager;
    static std::unique_ptr<ObjectManager> objectManager;
    
    std::unique_ptr<RenderManager> renderManager;
    std::unique_ptr<BehaviorManager> behaviorManager;
    std::unique_ptr<Input> input;

    Core();
    Core(SDL_Renderer* rend);

    ~Core();
    void OnUpdate();
    void OnRender();
};

#endif // CORE_H