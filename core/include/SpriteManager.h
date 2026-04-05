#pragma once

#ifndef SpriteManager_H
#define SpriteManager_H
#include <vector>
#include <functional>
#include <SDL3/SDL.h>
class SpriteRenderer;

class SpriteManager
{
public:
    bool isYSortEnabled = false; // Future feature for sorting by Y position
    static std::function<void(SpriteRenderer *)> OnSpriteRegister;
    static std::function<void(SpriteRenderer*)> OnSpriteUnregister;
    void Init();
    void RenderAllSprites(SDL_Renderer* renderer);
private:
    std::vector<SpriteRenderer*> sprites;
    void RegisterSprite(SpriteRenderer *sprite);
    void UnregisterSprite(SpriteRenderer *sprite);
    void SortSpritesByLayer();


};


#endif