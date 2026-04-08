#pragma once

#ifndef BOXCOLLIDER_H
#define BOXCOLLIDER_H

#include <SDL3/SDL.h>
#include "Collider.h"
#include "Transform.h"
#include "../math/Vector2.h"

class BoxCollider : public Collider
{
public:
    Transform *transform;
    Vector2 size;

    BoxCollider(Object* obj, Transform *transform);
    Bound GetBound() override;
    bool CheckCollision(Collider *collider) override;
    CollisionInfo GetCollisionInfo(Collider *collider) override;  
    void Render(SDL_Renderer* renderer) override;
    
    void OnAttached() override;
    void OnDetached() override;
};
#endif // BOXCOLLIDER_H
