#include "../../include/components/SpriteRenderer.h"
#include <SDL3_image/SDL_image.h>
#include "../../include/Core.h"
#include "../../include/RenderManager.h"

SpriteRenderer::SpriteRenderer(Transform *trans) : texture(nullptr), transform(trans)
{
    layerOrder = 0;
    LoadTexture(Core::renderer, "core/assets/sprites/defaultSquare.png");
    RenderManager::OnRegisterRenderable(this);
}
SpriteRenderer::~SpriteRenderer()
{
    RenderManager::OnUnregisterRenderable(this);
    if (texture)
        SDL_DestroyTexture(texture);
}

float SpriteRenderer::GetYSortValue() const
{
    return transform ? transform->position.y : 0.0f;
}

void SpriteRenderer::Render(SDL_Renderer* renderer)
{
    if(transform == nullptr || texture == nullptr) return;

    SDL_FRect rect;
    Vector2 rectPos = transform->GetSDLPosition(Vector2(width, height));

    rect.x = rectPos.x;
    rect.y = rectPos.y;
    rect.w = width * transform->scale.x;
    rect.h = height * transform->scale.y;

    SDL_FPoint pivot;
    pivot.x = rect.w / 2;
    pivot.y = rect.h / 2;

    SDL_RenderTextureRotated(
        renderer, 
        texture, 
        nullptr,           // Draw the whole texture
        &rect,             // Apply Transform Position & Scale
        transform->rotation,// Apply Transform Rotation (in degrees)
        &pivot,            // Rotate around the center
        SDL_FLIP_NONE      // No flipping
    );
}

void SpriteRenderer::LoadTexture(SDL_Renderer *initRenderer, const char *filePath)
{

    texture = IMG_LoadTexture(initRenderer, filePath);

    if (!texture)
    {
        SDL_Log("Failed to create texture from surface: %s, Error: %s", filePath, SDL_GetError());
    }
    else
    {
        SDL_GetTextureSize(texture, &width, &height);
    }
}
