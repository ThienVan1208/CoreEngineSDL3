#include "../../include/components/RectTransform.h"

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

    rect.w = size.x * GetScale().x;
    rect.h = size.y * GetScale().y;

    SetLocalPosition(rect.x, rect.y);

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
