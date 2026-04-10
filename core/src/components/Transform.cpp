#include "../../include/components/Transform.h"
#include "../../include/Coordinate.h"
#include <algorithm>
#include <cmath>

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

Transform::Transform() : localPosition{0, 0}, localScale{1, 1}, localRotation{0},
                         parent(nullptr), isDirty(true) {}

void Transform::SetDirty()
{
    isDirty = true;
    for (Transform *child : children)
    {
        child->SetDirty();
    }
}

void Transform::SetParent(Transform *newParent)
{
    if (parent == newParent)
        return;

    if (parent != nullptr)
    {
        auto it = std::find(parent->children.begin(), parent->children.end(), this);
        if (it != parent->children.end())
        {
            parent->children.erase(it);
        }
    }

    parent = newParent;
    if (parent != nullptr)
    {
        parent->children.push_back(this);
    }

    SetDirty();
}

void Transform::SetLocalPosition(float x, float y)
{
    localPosition.x = x;
    localPosition.y = y;
    SetDirty();
}

void Transform::SetLocalScale(float x, float y)
{
    localScale.x = x;
    localScale.y = y;
    SetDirty();
}

void Transform:: SetLocalRotation(float rotation)
{
    float finalRotation = rotation / 360.0f;
    finalRotation -= (int)(finalRotation);
    finalRotation *= 360.0f;
    localRotation = finalRotation;
    SetDirty();
}

Vector2 Transform::GetPosition()
{
    if (isDirty)
    {
        if (parent == nullptr)
        {
            worldPosition = localPosition;
            worldScale = localScale;
            worldRotation = localRotation;
        }
        else
        {
            Vector2 pScale = parent->GetScale();
            Vector2 scaledOffset = {
                localPosition.x * pScale.x,
                localPosition.y * pScale.y
            };

            float parentRot = parent->GetRotation();
            float rad = parentRot * (M_PI / 180.0f);
            float cosA = cos(-rad);
            float sinA = sin(-rad);

            Vector2 rotatedOffset = {
                scaledOffset.x * cosA - scaledOffset.y * sinA,
                scaledOffset.x * sinA + scaledOffset.y * cosA
            };

            worldPosition = parent->GetPosition() + rotatedOffset;
            worldScale = {localScale.x * pScale.x, localScale.y * pScale.y};
            worldRotation = localRotation + parentRot;
        }
        isDirty = false;
    }
    return worldPosition;
}

Vector2 Transform::GetScale()
{
    GetPosition(); // Ensure cache is updated
    return worldScale;
}

float Transform::GetRotation()
{
    GetPosition(); // Ensure cache is updated
    return worldRotation;
}

void Transform::Translate(float deltaX, float deltaY)
{
    SetPosition(GetPosition().x + deltaX, GetPosition().y + deltaY);
}

void Transform::SetPosition(float x, float y)
{
    if (parent == nullptr)
    {
        SetLocalPosition(x, y);
    }
    else
    {
        Vector2 delta = {x - parent->GetPosition().x, y - parent->GetPosition().y};
        float rad = -parent->GetRotation() * (M_PI / 180.0f);
        float cosA = cos(rad);
        float sinA = sin(rad);

        Vector2 unrotatedDelta = {
            delta.x * cosA - delta.y * sinA,
            delta.x * sinA + delta.y * cosA};

        Vector2 pScale = parent->GetScale();
        SetLocalPosition(unrotatedDelta.x / (pScale.x != 0 ? pScale.x : 1),
                         unrotatedDelta.y / (pScale.y != 0 ? pScale.y : 1));
    }
}

void Transform::Rotate(float deltaRotation)
{
    SetRotation(GetRotation() + deltaRotation);
}

void Transform::SetRotation(float newRotation)
{
    if (parent == nullptr)
    {
        SetLocalRotation(newRotation);
    }
    else
    {
        SetLocalRotation(newRotation - parent->GetRotation());
    }
}

void Transform::Scale(float scaleX, float scaleY)
{
    SetScale(GetScale().x * scaleX, GetScale().y * scaleY);
}

void Transform::SetScale(float x, float y)
{
    if (parent == nullptr)
    {
        SetLocalScale(x, y);
    }
    else
    {
        Vector2 pScale = parent->GetScale();
        SetLocalScale(x / (pScale.x != 0 ? pScale.x : 1),
                      y / (pScale.y != 0 ? pScale.y : 1));
    }
}

Vector2 Transform::GetSDLPosition(Vector2 size)
{
    Vector2 sdlCenter = CoordinateConverter::ZanToSDL(GetPosition());
    return Vector2(sdlCenter.x - size.x / 2.0f, sdlCenter.y - size.y / 2.0f);
}
