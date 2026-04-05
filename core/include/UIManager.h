#pragma once

#ifndef UIMANAGER_H
#define UIMANAGER_H

#include <SDL3/SDL.h>
#include <string>
#include <vector>
#include <functional>
#include <algorithm>
#include "./components/Component.h"
#include "./setting/Setting.h"
class UIElement;

class UIManager
{
public:
    static std::function<void(UIElement *)> OnUIRegister;
    static std::function<void(UIElement*)> OnUIUnregister;
    void Init();
    void RenderElements(SDL_Renderer *renderer);
    ~UIManager();

private:
    std::vector<UIElement *> elements;
    void RegisterElement(UIElement *element);
    void UnregisterElement(UIElement *element);
};

class UIElement
{
public:
    int orderLayer;
    RectTransform* rectTransform;
    bool isActive;
    UIElement();
    UIElement(float x, float y, float width, float height);

    virtual ~UIElement() = default;
    virtual void Render(SDL_Renderer *renderer);
};

class UIText : public UIElement
{
public:
    std::string text;
    SDL_Color color;

    UIText();
    UIText(std::string txt);

    UIText(float x, float y, float width, float height, SDL_Color initColor, std::string txt);

    UIText(float x, float y, float width, float height, uint8_t colorArray[4], std::string txt);

    UIText(float x, float y, float width, float height, std::string txt);
        

    ~UIText() override = default;

    void Render(SDL_Renderer *renderer) override;
};
class Image : public UIElement{
public:
    SDL_Texture* texture;
    SDL_Color color;
    Image();
    Image(const char* filePath);
    Image(float x, float y, float width, float height);
    void Render(SDL_Renderer *renderer) override;
    void LoadTexture(SDL_Renderer *initRenderer, const char* filePath);
};
class UIButton : public UIElement
{
private:
    std::vector<std::function<void()>*> onClick;

public:
    UIText* text;
    Image* image;
    UIButton();
    UIButton(float x, float y, float width, float height, std::string txt); // <-- Add this!

    ~UIButton() override = default;

    void AddListener(std::function<void()>& callback);

    void RemoveListener(std::function<void()>& callback);
 
    void RemoveAllListeners();


    void Render(SDL_Renderer *renderer) override;
};

#endif // UIMANAGER_H