#pragma once

#ifndef IRENDERABLE_H
#define IRENDERABLE_H

#include <SDL3/SDL.h>

class IRenderable
{
public:
    virtual ~IRenderable() = default;

    // The order layer (higher values render on top)
    virtual int GetRenderLayer() const = 0;

    // Optional Y-sort value (e.g. for top-down perspective depth)
    virtual float GetYSortValue() const { return 0.0f; }

    // Allows disabling rendering for particular objects (e.g. debug colliders)
    virtual bool IsVisible() const { return true; }

    // Core render method
    virtual void Render(SDL_Renderer* renderer) = 0;
};

#endif // IRENDERABLE_H
