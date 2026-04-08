#include "../../include/components/Transform.h"
#include "../../include/Coordinate.h"

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
    finalNewRotation -= (int)(finalNewRotation); 
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
    // Convert Zan center position to SDL center position
    Vector2 sdlCenter = CoordinateConverter::ZanToSDL(position);
    
    // Then convert from center to top-left corner for rendering
    return Vector2(sdlCenter.x - size.x / 2.0f, sdlCenter.y - size.y / 2.0f);
}
