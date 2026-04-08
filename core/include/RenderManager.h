#pragma once

#ifndef RENDERMANAGER_H
#define RENDERMANAGER_H

#include <SDL3/SDL.h>
#include <vector>
#include <functional>
#include "components/IRenderable.h"

class RenderManager
{
public:
    static std::function<void(IRenderable*)> OnRegisterRenderable;
    static std::function<void(IRenderable*)> OnUnregisterRenderable;

    bool isYSortEnabled = false; // Turned off by default

    void Init();
    void RenderAll(SDL_Renderer* renderer);
    
    // Manage Y sorting flag
    void SetYSortEnabled(bool enabled) { isYSortEnabled = enabled; }

private:
    std::vector<IRenderable*> renderables;
    void RegisterRenderable(IRenderable* renderable);
    void UnregisterRenderable(IRenderable* renderable);
};

#endif // RENDERMANAGER_H
