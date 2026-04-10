#pragma once

#ifndef TRANSFORM_H
#define TRANSFORM_H

#include <SDL3/SDL.h>
#include <vector>
#include "Component.h"
#include "../math/Vector2.h"

class Transform : public Component
{
private:
    Vector2 localPosition;
    Vector2 localScale;
    float localRotation;
    
    // Cached World States
    Vector2 worldPosition;
    Vector2 worldScale;
    float worldRotation;

    bool isDirty;
    void SetDirty();

public:
    Transform* parent;
    std::vector<Transform*> children;

    Transform();

    void SetParent(Transform* newParent);

    // Local Transformations
    Vector2 GetLocalPosition() const { return localPosition; }
    Vector2 GetLocalScale() const { return localScale; }
    float GetLocalRotation() const { return localRotation; }

    void SetLocalPosition(float x, float y);
    void SetLocalScale(float x, float y);
    void SetLocalRotation(float rotation);

    // World Transformations (Calculated)
    Vector2 GetPosition();
    Vector2 GetScale();
    float GetRotation();

    // Helper functions
    virtual void Translate(float deltaX, float deltaY); // Modifies world position
    virtual void SetPosition(float x, float y);         // Sets world position
    void Rotate(float deltaRotation);                   // Modifies world rotation
    void SetRotation(float newRotation);                // Sets world rotation
    void Scale(float scaleX, float scaleY);             // Modifies world scale
    void SetScale(float x, float y);                    // Sets world scale

    // Convert from Zan world position to SDL render position (accounting for pivot/center)
    Vector2 GetSDLPosition(Vector2 size);

    // Get SDL rect for rendering (Zan position with Y conversion)
    SDL_FRect GetSDLRect(float width, float height);
};

#endif // TRANSFORM_H
