#pragma once

#ifndef RECTTRANSFORM_H
#define RECTTRANSFORM_H

#include <SDL3/SDL.h>
#include "Transform.h"
#include "../math/Vector2.h"

class RectTransform : public Transform
{
public:
    Vector2 pivot;
    Vector2 anchorMax;
    Vector2 anchorMin;
    Vector2 anchoredPosition;
    Vector2 size; // this is the actual size of UI. EX: image has 10x5 pixel in size

    RectTransform();
    RectTransform(Vector2 initSize);
    RectTransform(float sizeX, float sizeY);

    SDL_FRect CalculateRect(float parentWidth, float parentHeight);

    void Translate(float deltaX, float deltaY) override;
    void SetPosition(float x, float y) override;
};
#endif // RECTTRANSFORM_H
