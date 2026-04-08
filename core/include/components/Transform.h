#pragma once

#ifndef TRANSFORM_H
#define TRANSFORM_H

#include <SDL3/SDL.h>
#include "Component.h"
#include "../math/Vector2.h"

class Transform : public Component
{
public:
    Vector2 position; // Position in Zan coordinate system
    Vector2 scale;
    float rotation; // in degrees

    Transform();

    virtual void Translate(float deltaX, float deltaY);
    virtual void SetPosition(float x, float y);
    void Rotate(float deltaRotation);
    void SetRotation(float newRotation);
    void Scale(float scaleX, float scaleY);
    void SetScale(float x, float y);

    // Convert from Zan world position to SDL render position (accounting for pivot/center)
    Vector2 GetSDLPosition(Vector2 size);

    // Get SDL rect for rendering (Zan position with Y conversion)
    SDL_FRect GetSDLRect(float width, float height);
};

#endif // TRANSFORM_H
