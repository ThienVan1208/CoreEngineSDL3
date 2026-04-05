#include "../../include/components/Component.h"
#include <SDL3_image/SDL_image.h>
#include "../../include/Core.h"
#include "../../include/SpriteManager.h"

/*
    TRANSFORM
*/
Transform::Transform() : position{0, 0}, scale{1, 1}, rotation{0} {}
void Transform::Translate(float deltaX, float deltaY)
{
    position.x += deltaX;
    position.y += deltaY;
}
void Transform::SetPosition(float x, float y)
{
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


/*
    RECT
*/

RectTransform::RectTransform(Vector2 initSize) : Transform(){
    pivot = Vector2(0.5f, 0.5f);
    anchorMin = Vector2(0.5f, 0.5f);
    anchorMax = Vector2(0.5f, 0.5f);
    anchoredPosition = Vector2(0.0f, 0.0f);
    size = initSize;
}

RectTransform::RectTransform(float sizeX, float sizeY) : Transform(){
    pivot = Vector2(0.5f, 0.5f);
    anchorMin = Vector2(0.5f, 0.5f);
    anchorMax = Vector2(0.5f, 0.5f);
    anchoredPosition = Vector2(0.0f, 0.0f);
    size.x = sizeX;
    size.y = sizeY;
}

RectTransform::RectTransform() : Transform(){
    pivot = Vector2(0.5f, 0.5f);
    anchorMin = Vector2(0.5f, 0.5f);
    anchorMax = Vector2(0.5f, 0.5f);
    anchoredPosition = Vector2(0.0f, 0.0f);
    size = Vector2(10.0f, 5.0f);

}

SDL_FRect RectTransform::CalculateRect(float parentWidth, float parentHeight){
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

void RectTransform::Translate(float deltaX, float deltaY) {
    anchoredPosition.x += deltaX;
    anchoredPosition.y += deltaY;
}

void RectTransform::SetPosition(float x, float y){
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
