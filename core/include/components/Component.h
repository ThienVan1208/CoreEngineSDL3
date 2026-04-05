#pragma once

#ifndef COMPONENT_H
#define COMPONENT_H
#include <SDL3/SDL.h>

struct Vector2
{
    float x;
    float y;
    // Constructors
    Vector2() : x(0.0f), y(0.0f) {}
    Vector2(float x, float y) : x(x), y(y) {}

    Vector2 operator+(const Vector2 &other) const { return Vector2(x + other.x, y + other.y); }
    Vector2 operator-(const Vector2 &other) const { return Vector2(x - other.x, y - other.y); }
    Vector2 operator*(float scalar) const { return Vector2(x * scalar, y * scalar); }
    Vector2 operator/(float div) const {return Vector2(x/div, y/div);}
};

class Component
{
};

class Transform : public Component
{
public:
    Vector2 position;
    Vector2 scale;
    float rotation; // in degrees

    Transform();

    virtual void Translate(float deltaX, float deltaY);

    virtual void SetPosition(float x, float y);

    void Rotate(float deltaRotation);

    void SetRotation(float newRotation);

    void Scale(float scaleX, float scaleY);

    void SetScale(float x, float y);
};

class RectTransform : public Transform{
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

class SpriteRenderer : public Component
{
private:
    float width, height;
    Transform *transform;

public:
    SDL_Texture *texture;

    int layerOrder;
    SpriteRenderer(Transform *trans);
    void LoadTexture(SDL_Renderer *initRenderer, const char *filePath);

    Transform *GetTransform() const { return transform; }
    float GetWidth() const { return width; }
    float GetHeight() const { return height; }
    ~SpriteRenderer();
};
#endif // COMPONENT_H
