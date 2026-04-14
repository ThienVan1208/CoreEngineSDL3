#include "../../include/components/BoxCollider.h"
#include "../../include/Coordinate.h"
#include "../../include/Core.h"
#include "../../include/ObjectManager.h"
#include "../../include/components/SpriteRenderer.h"
#include "../../include/setting/Setting.h"
#include <cfloat>
#include <cmath>
#include <algorithm>

BoxCollider::BoxCollider(Object* obj, Transform *transform) : Collider(obj), transform(transform)
{
    size = Vector2(1.0f, 1.0f); // Default value 1 unit
    offset = Vector2(0.0f, 0.0f);
}

void BoxCollider::OnAttached()
{
    // Auto-size based on SpriteRenderer if available
    GameObject* go = dynamic_cast<GameObject*>(object);
    if (go && go->spriteRenderer) {
        size = Vector2(
            (go->spriteRenderer->GetWidth() * std::abs(transform->GetScale().x)) / Setting::BASE_UNIT, 
            (go->spriteRenderer->GetHeight() * std::abs(transform->GetScale().y)) / Setting::BASE_UNIT
        );
    } else {
        size = Vector2(2.0f, 1.0f); // Sensible fallback in units (100x50 pixels)
    }

    if (Core::physicsManager)
    {
        Core::physicsManager->RegisterCollider(this);
    }
}

void BoxCollider::OnDetached()
{
    if (Core::physicsManager)
    {
        Core::physicsManager->UnregisterCollider(this);
    }
}

bool BoxCollider::CheckCollision(Collider* collider){
    Bound myBound = this->GetBound();
    Bound anotherBound = collider->GetBound();
    return !(myBound.right < anotherBound.left || 
             myBound.left > anotherBound.right || 
             myBound.top < anotherBound.bottom || 
             myBound.bottom > anotherBound.top);
}

CollisionInfo BoxCollider::GetCollisionInfo(Collider* collider)
{
    CollisionInfo info;
    info.isColliding = false;
    info.normal = Vector2(0, 0);
    info.penetrationDepth = 0.0f;
    
    Bound myBound = this->GetBound();
    Bound otherBound = collider->GetBound();
    
    if (myBound.right < otherBound.left || 
        myBound.left > otherBound.right || 
        myBound.top < otherBound.bottom || 
        myBound.bottom > otherBound.top)
    {
        return info;
    }
    
    info.isColliding = true;
    
    float overlapLeft = myBound.right - otherBound.left;
    float overlapRight = otherBound.right - myBound.left;
    float overlapTop = myBound.top - otherBound.bottom;
    float overlapBottom = otherBound.top - myBound.bottom;
    
    float maxPenetration = std::max(size.x, size.y) * 1.5f;  // Clamp penetration
    
    float minHorizontal = FLT_MAX;
    Vector2 horizontalNormal(0, 0);
    
    if (overlapLeft >= 0 && overlapLeft < minHorizontal) {
        minHorizontal = overlapLeft;
        horizontalNormal = Vector2(1, 0); 
    }
    if (overlapRight >= 0 && overlapRight < minHorizontal) {
        minHorizontal = overlapRight;
        horizontalNormal = Vector2(-1, 0);
    }
    
    float minVertical = FLT_MAX;
    Vector2 verticalNormal(0, 0);
    
    if (overlapTop >= 0 && overlapTop < minVertical) {
        minVertical = overlapTop;
        verticalNormal = Vector2(0, 1);
    }
    if (overlapBottom >= 0 && overlapBottom < minVertical) {
        minVertical = overlapBottom;
        verticalNormal = Vector2(0, -1);
    }
    
    // Strict AABB resolution: always resolve along the axis of minimum penetration
    if (minHorizontal < minVertical)
    {
        info.penetrationDepth = std::min(minHorizontal, maxPenetration);
        info.normal = horizontalNormal;
    }
    else
    {
        info.penetrationDepth = std::min(minVertical, maxPenetration);
        info.normal = verticalNormal;
    }
    
    return info;
}

Bound BoxCollider::GetBound(){
    Bound bound;
    bound.left = transform->GetPosition().x + offset.x - size.x / 2.0f;
    bound.right = bound.left + size.x ;
    bound.top = transform->GetPosition().y + offset.y + size.y / 2.0f;
    bound.bottom = bound.top - size.y;

    return bound;
}

void BoxCollider::Render(SDL_Renderer* renderer)
{
    if (!isRenderVisible) return;
    Bound b = GetBound();
    Vector2 topLeftZan = { b.left, b.top };
    Vector2 sdlTopLeft = CoordinateConverter::ZanToSDL(topLeftZan);
    
    SDL_FRect rect;
    rect.x = sdlTopLeft.x;
    rect.y = sdlTopLeft.y;
    rect.w = (b.right - b.left) * Setting::BASE_UNIT;
    rect.h = (b.top - b.bottom) * Setting::BASE_UNIT; // Zenith rect height
    
    SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255); // Red color for debug outlines
    SDL_RenderRect(renderer, &rect);
}
