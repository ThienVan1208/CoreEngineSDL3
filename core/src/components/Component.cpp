#include "../../include/components/Component.h"
#include <SDL3_image/SDL_image.h>
#include "../../include/Core.h"
#include "../../include/SpriteManager.h"
#include "../../include/ObjectManager.h"

/*
    TRANSFORM
*/
Transform::Transform() : position{0, 0}, scale{1, 1}, rotation{0} {}

void Transform::Translate(float deltaX, float deltaY)
{
    // Movement in Zan coordinates (Y-up)
    position.x += deltaX;
    position.y += deltaY;
}

void Transform::SetPosition(float x, float y)
{
    // Set position in Zan coordinates
    position.x = x;
    position.y = y;
}

void Transform::Rotate(float deltaRotation)
{
    float finalDetaRotation = deltaRotation / 360.0f;
    finalDetaRotation -= (int)(finalDetaRotation); // Keep it between 0 and 1
    finalDetaRotation *= 360.0f;
    rotation += finalDetaRotation;
}
void Transform::SetRotation(float newRotation)
{
    float finalNewRotation = newRotation / 360.0f;
    finalNewRotation -= (int)(finalNewRotation); // Keep it between 0 and
    finalNewRotation *= 360.0f;
    rotation = finalNewRotation;
}

void Transform::Scale(float scaleX, float scaleY)
{
    scale.x *= scaleX;
    scale.y *= scaleY;
}
void Transform::SetScale(float x, float y)
{
    scale.x = x;
    scale.y = y;
}

Vector2 Transform::GetSDLPosition(Vector2 size){
    // First convert Zan center position to SDL center position
    float sdlCenterX = CoordinateConverter::ZanToSDL_X(position.x);
    float sdlCenterY = CoordinateConverter::ZanToSDL_Y(position.y);
    
    // Then convert from center to top-left corner
    return Vector2(sdlCenterX - size.x / 2.0f, sdlCenterY - size.y / 2.0f);
}

/*
    RECT
*/
RectTransform::RectTransform(Vector2 initSize) : Transform()
{
    pivot = Vector2(0.5f, 0.5f);
    anchorMin = Vector2(0.5f, 0.5f);
    anchorMax = Vector2(0.5f, 0.5f);
    anchoredPosition = Vector2(0.0f, 0.0f);
    size = initSize;
}

RectTransform::RectTransform(float sizeX, float sizeY) : Transform()
{
    pivot = Vector2(0.5f, 0.5f);
    anchorMin = Vector2(0.5f, 0.5f);
    anchorMax = Vector2(0.5f, 0.5f);
    anchoredPosition = Vector2(0.0f, 0.0f);
    size.x = sizeX;
    size.y = sizeY;
}

RectTransform::RectTransform() : Transform()
{
    pivot = Vector2(0.5f, 0.5f);
    anchorMin = Vector2(0.5f, 0.5f);
    anchorMax = Vector2(0.5f, 0.5f);
    anchoredPosition = Vector2(0.0f, 0.0f);
    size = Vector2(10.0f, 5.0f);
}

SDL_FRect RectTransform::CalculateRect(float parentWidth, float parentHeight)
{
    SDL_FRect rect;

    float anchorBoxSizeW = parentWidth * (anchorMax.x - anchorMin.x);
    float anchorBoxSizeH = parentHeight * (anchorMax.y - anchorMin.y);

    float pivotAnchorBoxPosX = parentWidth * anchorMin.x + anchorBoxSizeW * pivot.x;
    float pivotAnchorBoxPosY = parentHeight * anchorMin.y + anchorBoxSizeH * pivot.y;

    float pivotUiPosX = pivotAnchorBoxPosX + anchoredPosition.x;
    float pivotUiPosY = pivotAnchorBoxPosY + anchoredPosition.y;

    rect.x = pivotUiPosX - size.x * pivot.x;
    rect.y = pivotUiPosY - size.y * pivot.y;

    size.x *= scale.x;
    size.y *= scale.y;

    rect.w = size.x;
    rect.h = size.y;

    position.x = rect.x;
    position.y = rect.y;

    return rect;
}

void RectTransform::Translate(float deltaX, float deltaY)
{
    anchoredPosition.x += deltaX;
    anchoredPosition.y += deltaY;
}

void RectTransform::SetPosition(float x, float y)
{
    anchoredPosition.x = x;
    anchoredPosition.y = y;
}

/*
    SPRITE RENDERER
*/
SpriteRenderer::SpriteRenderer(Transform *trans) : texture(nullptr), transform(trans)
{
    layerOrder = 0;
    LoadTexture(Core::renderer, "core/assets/sprites/defaultSquare.png");
    SpriteManager::OnSpriteRegister(this);
}
SpriteRenderer::~SpriteRenderer()
{
    SpriteManager::OnSpriteUnregister(this);
    if (texture)
        SDL_DestroyTexture(texture);
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

/*
    COLLIDER
*/
BoxCollider::BoxCollider(Object* obj, Transform *transform) : Collider(obj), transform(transform)
{
    size = Vector2(50.0f, 50.0f);
    offset = Vector2(0.0f, 0.0f);
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
    
    // Check if colliding first
    if (myBound.right < otherBound.left || 
        myBound.left > otherBound.right || 
        myBound.top < otherBound.bottom || 
        myBound.bottom > otherBound.top)
    {
        return info;  // No collision
    }
    
    info.isColliding = true;
    
    // Calculate penetration depth for each axis
    float overlapLeft = myBound.right - otherBound.left;
    float overlapRight = otherBound.right - myBound.left;
    float overlapTop = myBound.top - otherBound.bottom;
    float overlapBottom = otherBound.top - myBound.bottom;
    
    // Find minimum overlap to determine collision normal and depth
    float minOverlap = overlapLeft;
    info.normal = Vector2(-1, 0);  // Push left
    
    if (overlapRight < minOverlap)
    {
        minOverlap = overlapRight;
        info.normal = Vector2(1, 0);  // Push right
    }
    
    if (overlapTop < minOverlap)
    {
        minOverlap = overlapTop;
        info.normal = Vector2(0, 1);  // Push up
    }
    
    if (overlapBottom < minOverlap)
    {
        minOverlap = overlapBottom;
        info.normal = Vector2(0, -1);  // Push down
    }
    
    info.penetrationDepth = minOverlap;
    return info;
}

Bound BoxCollider::GetBound(){
    Bound bound;
    bound.left = transform->position.x + offset.x - size.x / 2.0f;
    bound.right = bound.left + size.x ;
    bound.top = transform->position.y + offset.y + size.y / 2.0f;
    bound.bottom = bound.top - size.y;

    return bound;
}


/*
    RIGIDBODY
*/
RigidBody::RigidBody(Object* obj) : object(obj), velocity{0, 0}, mass{1.0f}, acceleration{0, 0}, type{RigidbodyType::Dynamic}
{
    for(auto& comp : obj->GetComponents()){
        Collider* collider = dynamic_cast<Collider*>(comp);
        if(collider){
            colliders.push_back(collider);
        }
    }
}
void RigidBody::AddForce(Vector2 force)
{
    if (type != RigidbodyType::Dynamic) return; 
    
    acceleration = {force.x / mass, force.y / mass};
    velocity.x += acceleration.x;
    velocity.y += acceleration.y;
}
void RigidBody::AddVelocity(Vector2 deltaVel)
{
    velocity.x += deltaVel.x;
    velocity.y += deltaVel.y;
}

void RigidBody::SetVelocity(Vector2 newVel)
{
    velocity = newVel;
}

void RigidBody::OnPhysicsUpdate(float deltaTime)
{
    if (type != RigidbodyType::Dynamic) return; 

    object->transform->Translate(velocity.x * deltaTime, velocity.y * deltaTime);
}

void RigidBody::ApplyImpulse(Vector2 impulse)
{
    if (type != RigidbodyType::Dynamic) return;

    // Impulse = change in velocity (J = m * dv, so dv = J / m)
    velocity.x += impulse.x / mass;
    velocity.y += impulse.y / mass;
}
