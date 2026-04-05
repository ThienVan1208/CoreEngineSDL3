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
        std::sort(sprites.begin(), sprites.end(), [](SpriteRenderer* a, SpriteRenderer* b) {
            return a->GetTransform()->position.y < b->GetTransform()->position.y;
        });
    }
    for (auto& sprite : sprites){
        if(sprite->GetTransform() == nullptr || sprite->texture == nullptr) continue; // Skip if no transform or texture

        SDL_FRect rect;
        rect.x = sprite->GetTransform()->position.x;
        rect.y = sprite->GetTransform()->position.y;
        rect.w = (sprite->GetWidth() / Setting::BASE_UNIT) * sprite->GetTransform()->scale.x;
        rect.h = (sprite->GetHeight() / Setting::BASE_UNIT) * sprite->GetTransform()->scale.y;

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