#include "../include/Core.h"
#include "../include/UiManager.h"
#include "../include/ObjectManager.h"
#include "../include/SpriteManager.h"
#include "../include/Input.h"

SDL_Renderer *Core::renderer = nullptr;

Core::Core()
{
    uiManager = new UIManager();
    objectManager = new ObjectManager();
    spriteManager = new SpriteManager();
    input = new Input();

    uiManager->Init();
    objectManager->Init();
    spriteManager->Init();
    input->Init();
}
Core::Core(SDL_Renderer *rend)
{
    renderer = rend;
    uiManager = new UIManager();
    objectManager = new ObjectManager();
    spriteManager = new SpriteManager();
    input = new Input();

    uiManager->Init();
    objectManager->Init();
    spriteManager->Init();
    input->Init();
}

Core::~Core()
{
    delete uiManager;
    delete objectManager;
    delete spriteManager;
    delete input;
}
void Core::OnUpdate()
{
    input->Update();
    objectManager->UpdateObjects();
}
void Core::OnRender()
{
    spriteManager->RenderAllSprites(renderer);
    uiManager->RenderElements(renderer);
}