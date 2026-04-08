#pragma once

#ifndef COLLIDER_H
#define COLLIDER_H

#include <SDL3/SDL.h>
#include "Component.h"
#include "IRenderable.h"
#include "../math/Vector2.h"

// Forward declaration
class Object;

struct Bound
{
    float top, bottom; // Dont care about x axis
    float left, right; // Dont care about y axis
};

// Collision info returned from collision detection
struct CollisionInfo
{
    bool isColliding = false;
    Vector2 normal;           // Direction from A to B (unit vector)
    float penetrationDepth = 0.0f;  // How far objects overlap
};

class Collider : public Component, public IRenderable
{
protected:
    Object* object;  // Reference to the object this collider belongs to
public:
    Vector2 offset;
    bool isRenderVisible = false;
    
    Collider(Object* obj) : object(obj) {}
    
    virtual bool CheckCollision(Collider *collider) = 0;
    virtual CollisionInfo GetCollisionInfo(Collider *collider) = 0; 
    virtual Bound GetBound() = 0;
    Object* GetObject() const { return object; }

    // IRenderable implementation
    int GetRenderLayer() const override { return 999; } // Always on top of sprites
    bool IsVisible() const override { return isRenderVisible; }
    // Collider itself won't render unless overridden by specific collider shapes like BoxCollider
    void Render(SDL_Renderer* renderer) override {} 
};
#endif // COLLIDER_H
