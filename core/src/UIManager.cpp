#include "../include/UiManager.h"

std::function<void(UIElement *)> UIManager::OnUIRegister;
std::function<void(UIElement*)> UIManager::OnUIUnregister;

/*
    UI Element
*/

UIElement:: UIElement(): rectTransform(){
    orderLayer = 0;
    isActive = true;
    UIManager::OnUIRegister(this);
}

UIElement::UIElement(float x, float y, float width, float height) : rectTransform()
{
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
    SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a);
    SDL_RenderDebugText(renderer, rectTransform->position.x, rectTransform->position.y, text.c_str());
}


/*
    IMAGE
*/
Image::Image() : UIElement(0, 0, 0, 0){

}

void Image::LoadTexture(SDL_Renderer *initRenderer, const char* filePath){
    
}


/**********************************
            BUTTON
***********************************/
UIButton::UIButton() : UIElement(0, 0, 0, 0)
{
    text = new UIText(0, 0, 0, 0, "New Button");
    color = {255, 255, 255, 255}; // Default white color
}
UIButton::UIButton(float x, float y, float width, float height)
    : UIElement(x, y, width, height)
{
    text = new UIText(x + width / 2, y, width, height, color, "New Button");
    this->color = {255, 255, 255, 255};
}
UIButton::UIButton(float x, float y, float width, float height, SDL_Color initColor, std::string txt)
    : UIElement(x, y, width, height)
{
    this->text = new UIText(x + width / 2, y, width, height, txt);
    this->color = initColor;
}

UIButton::UIButton(float x, float y, float width, float height, uint8_t colorArray[4], std::string txt)
    : UIElement(x, y, width, height)
{
    this->text = new UIText(x + width / 2, y, width, height, txt);
    this->color = {colorArray[0], colorArray[1], colorArray[2], colorArray[3]};
}

UIButton::UIButton(float x, float y, float width, float height, std::string txt)
    : UIElement(x, y, width, height)
{
    this->text = new UIText(x + width / 2, y, width, height, txt);
    this->color = {0, 0, 0, 255}; // Default white color
}
void UIButton::Render(SDL_Renderer *renderer)
{
    UIElement::Render(renderer); // Call base render (if needed)
    // Render button background
    SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a);
    SDL_RenderFillRect(renderer, &rect);

    // Render button text
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