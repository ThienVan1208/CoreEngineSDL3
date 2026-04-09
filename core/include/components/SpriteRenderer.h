#pragma once

#ifndef SPRITERENDERER_H
#define SPRITERENDERER_H

#include <SDL3/SDL.h>
#include "Component.h"
#include "Transform.h"
#include "IRenderable.h"

// Forward declaration if needed, or included directly via SDL
struct SDL_Texture;
struct SDL_Renderer;

class SpriteRenderer : public Component, public IRenderable
{
private:
    float width, height;
    Transform *transform;

public:
    SDL_Texture *texture;
    SDL_Color color;
    int layerOrder;
    SpriteRenderer(Transform *trans);
    void LoadTexture(SDL_Renderer *initRenderer, const char *filePath);

    Transform *GetTransform() const { return transform; }
    float GetWidth() const { return width; }
    float GetHeight() const { return height; }
    void SetSize(float w, float h);
    ~SpriteRenderer() override;

    // IRenderable implementation
    int GetRenderLayer() const override { return layerOrder; }
    float GetYSortValue() const override;
    void Render(SDL_Renderer* renderer) override;
};
#endif // SPRITERENDERER_H
