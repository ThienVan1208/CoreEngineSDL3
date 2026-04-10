#pragma once

#ifndef UIELEMENTS_H
#define UIELEMENTS_H

#include <SDL3/SDL.h>
#include <string>
#include <vector>
#include <functional>
#include <algorithm>
#include "components/Component.h"
#include "components/IRenderable.h"
#include <memory>
#include "components/RectTransform.h"
#include "setting/Setting.h"

class UIElement : public IRenderable
{
public:
    int orderLayer;
    std::shared_ptr<RectTransform> rectTransform;
    bool isActive;
    
    UIElement();
    UIElement(float x, float y);

    virtual ~UIElement();

    // IRenderable overrides
    int GetRenderLayer() const override { return 1000 + orderLayer; } // UI renders at 1000+
    bool IsVisible() const override { return isActive; }
    virtual void Render(SDL_Renderer *renderer) override;
};

class UIText : public UIElement
{
public:
    std::string text;
    SDL_Color color;

    UIText();
    UIText(std::string txt);
    UIText(float x, float y, SDL_Color initColor, std::string txt);
    UIText(float x, float y, std::string txt);

    ~UIText() override = default;

    void Render(SDL_Renderer *renderer) override;
};

class Image : public UIElement{
public:
    SDL_Texture* texture;
    SDL_Color color;
    
    Image();
    Image(const char* filePath);
    Image(float x, float y);
    
    void Render(SDL_Renderer *renderer) override;
    void LoadTexture(SDL_Renderer *initRenderer, const char* filePath);
};

class UIButton : public UIElement
{
private:
    std::vector<std::function<void()>*> onClick;

public:
    std::unique_ptr<UIText> text;
    std::unique_ptr<Image> image;
    
    UIButton();
    UIButton(float x, float y, std::string txt);

    ~UIButton() override = default;

    void AddListener(std::function<void()>& callback);
    void RemoveListener(std::function<void()>& callback);
    void RemoveAllListeners();

    void Render(SDL_Renderer *renderer) override;
};

#endif // UIELEMENTS_H
