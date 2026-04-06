#include "../include/SpriteManager.h"
#include "../include/components/Component.h"
#include "../include/setting/Setting.h"
#include <algorithm>
std::function<void(SpriteRenderer *)> SpriteManager::OnSpriteRegister;
std::function<void(SpriteRenderer*)> SpriteManager::OnSpriteUnregister;
void SpriteManager::Init()
{
    OnSpriteRegister = [this](SpriteRenderer *sprite)
    {
        RegisterSprite(sprite);
    };

    OnSpriteUnregister = [this](SpriteRenderer *sprite)
    {
        UnregisterSprite(sprite);
    };
}
void SpriteManager::RegisterSprite(SpriteRenderer *sprite)
{
    sprites.push_back(sprite);
    SortSpritesByLayer();
}

void SpriteManager::UnregisterSprite(SpriteRenderer *sprite)
{
    sprites.erase(std::remove(sprites.begin(), sprites.end(), sprite), sprites.end());
}

void SpriteManager::SortSpritesByLayer()
{
    std::sort(sprites.begin(), sprites.end(), [](SpriteRenderer* a, SpriteRenderer* b) {
        return a->layerOrder < b->layerOrder;
    });
}

void SpriteManager::RenderAllSprites(SDL_Renderer* renderer)
{
    if(isYSortEnabled) {
        std::stable_sort(sprites.begin(), sprites.end(), [this](SpriteRenderer* a, SpriteRenderer* b) {
        if (a->layerOrder != b->layerOrder) {
            return a->layerOrder < b->layerOrder; // Sort by Layer first
        }
        if (isYSortEnabled) {
            return a->GetTransform()->position.y < b->GetTransform()->position.y; // Sort by Y if same layer
        }
        return false; 
    });
    }
    for (auto& sprite : sprites){
        if(sprite->GetTransform() == nullptr || sprite->texture == nullptr) continue; // Skip if no transform or texture

        SDL_FRect rect;
        Vector2 rectPos = sprite->GetTransform()->GetSDLPosition(Vector2(sprite->GetWidth(), sprite->GetHeight()));

        rect.x = rectPos.x;
        rect.y = rectPos.y;
        rect.w = (sprite->GetWidth()) * sprite->GetTransform()->scale.x;
        rect.h = (sprite->GetHeight()) * sprite->GetTransform()->scale.y;

        SDL_FPoint pivot;
        pivot.x = rect.w / 2;
        pivot.y = rect.h / 2;

        SDL_RenderTextureRotated(
            renderer, 
            sprite->texture, 
            nullptr,           // Draw the whole texture
            &rect,             // Apply Transform Position & Scale
            sprite->GetTransform()->rotation,// Apply Transform Rotation (in degrees)
            &pivot,            // Rotate around the center
            SDL_FLIP_NONE      // No flipping
        );

    }
}