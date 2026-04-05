#include "../include/UiManager.h"
#include <SDL3_image/SDL_image.h>
#include "../include/Core.h"
#include "../include/Screen.h"

std::function<void(UIElement *)> UIManager::OnUIRegister;
std::function<void(UIElement *)> UIManager::OnUIUnregister;

/*
    UI Element
*/

UIElement::UIElement()
{
    rectTransform = new RectTransform();
    orderLayer = 0;
    isActive = true;
    UIManager::OnUIRegister(this);
}

UIElement::UIElement(float x, float y, float width, float height)
{
    rectTransform = new RectTransform();

    rectTransform->SetPosition(x, y);
    rectTransform->size.x = width;
    rectTransform->size.y = height;

    isActive = true;
    UIManager::OnUIRegister(this);
}
void UIElement::Render(SDL_Renderer *renderer)
{
    if (!isActive || renderer == nullptr)
        return;
};

/**********************************
            TEXT
***********************************/

UIText::UIText() : UIElement(0, 0, 0, 0)
{
    text = "New Text";
    color = {0, 0, 0, 255}; // Default black color
}

UIText::UIText(std::string newText) : UIElement(0, 0, 0, 0)
{
    text = newText;
    color = {0, 0, 0, 255}; // Default black color
}

UIText::UIText(float x, float y, float width, float height, SDL_Color initColor, std::string txt)
    : UIElement(x, y, width, height)
{
    this->text = txt;
    this->color = initColor;
}

UIText::UIText(float x, float y, float width, float height, uint8_t colorArray[4], std::string txt)
    : UIElement(x, y, width, height)
{
    this->text = txt;
    this->color = {colorArray[0], colorArray[1], colorArray[2], colorArray[3]};
}

UIText::UIText(float x, float y, float width, float height, std::string txt)
    : UIElement(x, y, width, height)
{
    this->text = txt;
    this->color = {0, 0, 0, 255}; // Default black color
}

void UIText::Render(SDL_Renderer *renderer)
{
    UIElement::Render(renderer); // Call base render (if needed)
    // Placeholder for text rendering logic
    // In a real implementation, you would use SDL_ttf or a similar library to render the text
    SDL_FRect rect = rectTransform->CalculateRect(Screen::GetWidth(), Screen::GetHeight());

    SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a);
    SDL_RenderDebugText(renderer, rect.x, rect.y, text.c_str());
}

/*
    IMAGE
*/
Image::Image() : UIElement(0, 0, 0, 0)
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
        rectTransform->rotation, // Apply Transform Rotation (in degrees)
        &pivot,                  // Rotate around the center
        SDL_FLIP_NONE            // No flipping
    );

}

/**********************************
            BUTTON
***********************************/
UIButton::UIButton() : UIElement(0, 0, 0, 0)
{
    image = new Image();
    text = new UIText("New Button");

    delete image->rectTransform;
    delete text->rectTransform;

    text->rectTransform = this->rectTransform;
    image->rectTransform = this->rectTransform;

    UIManager::OnUIUnregister(image);
    UIManager::OnUIUnregister(text);
}
// The constructor that actually sets the size!
UIButton::UIButton(float x, float y, float width, float height, std::string txt)
    : UIElement(x, y, width, height) 
{
    image = new Image();
    text = new UIText(txt);

    // 1. Delete the default transforms
    delete image->rectTransform;
    delete text->rectTransform;

    // 2. Share the Button's transform (which now has an actual width and height!)
    image->rectTransform = this->rectTransform;
    text->rectTransform = this->rectTransform;

    // 3. Unregister from the global manager
    UIManager::OnUIUnregister(image);
    UIManager::OnUIUnregister(text);
}
void UIButton::Render(SDL_Renderer *renderer)
{
    UIElement::Render(renderer); // Call base render (if needed)
    // Render button background

    // Render button text
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

/**********************************
            UIManager
***********************************/
UIManager::~UIManager()
{
    OnUIRegister = nullptr;
    OnUIUnregister = nullptr;
}
void UIManager::Init()
{
    OnUIRegister = [this](UIElement *element)
    {
        RegisterElement(element);
    };
    OnUIUnregister = [this](UIElement *element)
    {
        UnregisterElement(element);
    };
}
void UIManager::RegisterElement(UIElement *element)
{
    elements.push_back(element);
}
void UIManager::RenderElements(SDL_Renderer *renderer)
{
    for (auto &element : elements)
    {
        if (element->isActive)
            element->Render(renderer);
    }
}
void UIManager::UnregisterElement(UIElement *element)
{
    elements.erase(std::remove(elements.begin(), elements.end(), element), elements.end());
}