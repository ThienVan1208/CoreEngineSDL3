#include "../../include/components/SpriteRenderer.h"
#include <SDL3_image/SDL_image.h>
#include "../../include/Core.h"
#include "../../include/RenderManager.h"

SpriteRenderer::SpriteRenderer(Transform *trans) : texture(nullptr), transform(trans)
{
    layerOrder = 0;
    color = {255, 255, 255, 255}; // Default to white (no tint)
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
    return transform ? transform->GetPosition().y : 0.0f;
}

void SpriteRenderer::Render(SDL_Renderer* renderer)
{
    if(transform == nullptr || texture == nullptr) return;

    float finalWidth = width * std::abs(transform->GetScale().x);
    float finalHeight = height * std::abs(transform->GetScale().y);
    Vector2 rectPos = transform->GetSDLPosition(Vector2(finalWidth, finalHeight));

    SDL_FRect rect;
    rect.x = rectPos.x;
    rect.y = rectPos.y;
    rect.w = finalWidth;
    rect.h = finalHeight;

    SDL_FPoint pivot;
    pivot.x = rect.w / 2;
    pivot.y = rect.h / 2;

    SDL_SetTextureColorMod(texture, color.r, color.g, color.b);
    SDL_SetTextureAlphaMod(texture, color.a);

    int flip = SDL_FLIP_NONE;
    if (transform->GetScale().x < 0) flip |= SDL_FLIP_HORIZONTAL;
    if (transform->GetScale().y < 0) flip |= SDL_FLIP_VERTICAL;

    SDL_RenderTextureRotated(
        renderer, 
        texture, 
        nullptr,           // Draw the whole texture
        &rect,             // Apply Transform Position & Scale
        transform->GetRotation(),// Apply Transform Rotation (in degrees)
        &pivot,            // Rotate around the center
        (SDL_FlipMode)flip // Apply flipping
    );

    SDL_SetTextureColorMod(texture, 255, 255, 255); // Reset to default
    SDL_SetTextureAlphaMod(texture, 255);

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

void SpriteRenderer::SetSize(float w, float h)
{
    width = w;
    height = h;
}
