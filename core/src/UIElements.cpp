#include "../include/UIElements.h"
#include <SDL3_image/SDL_image.h>
#include "../include/Core.h"
#include "../include/Screen.h"
#include "../include/RenderManager.h"

/*
    UI Element
*/

UIElement::UIElement()
{
    rectTransform = std::make_shared<RectTransform>();
    orderLayer = 0;
    isActive = true;
    RenderManager::OnRegisterRenderable(this);
}

UIElement::UIElement(float x, float y)
{
    rectTransform = std::make_shared<RectTransform>();

    rectTransform->SetPosition(x, y);

    isActive = true;
    RenderManager::OnRegisterRenderable(this);
}

UIElement::~UIElement()
{
    RenderManager::OnUnregisterRenderable(this);
}

void UIElement::Render(SDL_Renderer *renderer)
{
    if (!isActive || renderer == nullptr)
        return;
}

/**********************************
            TEXT
***********************************/

UIText::UIText() : UIElement(0, 0)
{
    text = "New Text";
    color = {0, 0, 0, 255}; // Default black color
}

UIText::UIText(std::string newText) : UIElement(0, 0)
{
    text = newText;
    color = {0, 0, 0, 255}; // Default black color
}

UIText::UIText(float x, float y, SDL_Color initColor, std::string txt)
    : UIElement(x, y)
{
    this->text = txt;
    this->color = initColor;
}

UIText::UIText(float x, float y, std::string txt)
    : UIElement(x, y)
{
    this->text = txt;
    this->color = {0, 0, 0, 255}; // Default black color
}

void UIText::Render(SDL_Renderer *renderer)
{
    UIElement::Render(renderer); // Call base render (if needed)
    
    SDL_FRect rect = rectTransform->CalculateRect(Screen::GetWidth(), Screen::GetHeight());

    // Apply scale to the text by scaling the SDL renderer
    float prevScaleX, prevScaleY;
    SDL_GetRenderScale(renderer, &prevScaleX, &prevScaleY);

    float newScaleX = prevScaleX * rectTransform->GetScale().x;
    float newScaleY = prevScaleY * rectTransform->GetScale().y;

    // Prevent zero scale
    if (newScaleX <= 0.001f) newScaleX = 0.001f;
    if (newScaleY <= 0.001f) newScaleY = 0.001f;

    SDL_SetRenderScale(renderer, newScaleX, newScaleY);

    SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a);
    
    // Divide target position by newScale so it correctly places the text at the logical x, y
    SDL_RenderDebugText(renderer, rect.x / newScaleX, rect.y / newScaleY, text.c_str());

    // Restore previous scale
    SDL_SetRenderScale(renderer, prevScaleX, prevScaleY);
}

/*
    IMAGE
*/
Image::Image() : UIElement(0, 0)
{
    LoadTexture(Core::renderer, "core/assets/sprites/defaultSquare.png");
}

void Image::LoadTexture(SDL_Renderer *initRenderer, const char *filePath)
{
    texture = IMG_LoadTexture(initRenderer, filePath);

    if (!texture)
    {
        SDL_Log("Failed to create texture from surface: %s, Error: %s", filePath, SDL_GetError());
    }
    else
    {
        SDL_GetTextureSize(texture, &rectTransform->size.x, &rectTransform->size.y);
    }
}

void Image::Render(SDL_Renderer *renderer)
{
    SDL_FRect rect = rectTransform->CalculateRect(Screen::GetWidth(), Screen::GetHeight());

    SDL_FPoint pivot;
    pivot.x = rect.w / 2;
    pivot.y = rect.h / 2;

    SDL_RenderTextureRotated(
        renderer,
        texture,
        nullptr,                 // Draw the whole texture
        &rect,                   // Apply Transform Position & Scale
        rectTransform->GetRotation(), // Apply Transform Rotation (in degrees)
        &pivot,                  // Rotate around the center
        SDL_FLIP_NONE            // No flipping
    );
}

/**********************************
            BUTTON
***********************************/
UIButton::UIButton() : UIElement(0, 0)
{
    image = std::make_unique<Image>();
    text = std::make_unique<UIText>("New Button");

    text->rectTransform = this->rectTransform;
    image->rectTransform = this->rectTransform;

    RenderManager::OnUnregisterRenderable(image.get());
    RenderManager::OnUnregisterRenderable(text.get());
}

// The constructor that actually sets the size!
UIButton::UIButton(float x, float y, std::string txt)
    : UIElement(x, y) 
{
    image = std::make_unique<Image>();
    text = std::make_unique<UIText>(txt);

    image->rectTransform = this->rectTransform;
    text->rectTransform = this->rectTransform;

    RenderManager::OnUnregisterRenderable(image.get());
    RenderManager::OnUnregisterRenderable(text.get());
}

void UIButton::Render(SDL_Renderer *renderer)
{
    UIElement::Render(renderer); // Call base render (if needed)

    image->Render(renderer);
    text->Render(renderer);
}

void UIButton::AddListener(std::function<void()> &callback)
{
    onClick.push_back(&callback);
}
void UIButton::RemoveListener(std::function<void()> &callback)
{
    onClick.erase(std::remove(onClick.begin(), onClick.end(), &callback), onClick.end());
}
void UIButton::RemoveAllListeners()
{
    onClick.clear();
}
