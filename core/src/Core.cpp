#include "../include/Core.h"
#include "../include/UiManager.h"
#include "../include/ObjectManager.h"
#include "../include/BehaviorManager.h"
#include "../include/SpriteManager.h"
#include "../include/Input.h"
#include "../include/PhysicManager.h"

SDL_Renderer *Core::renderer = nullptr;
Physic *Core::physicsManager = nullptr;

Core::Core()
{
    uiManager = new UIManager();
    objectManager = new ObjectManager();
    behaviorManager = new BehaviorManager();
    spriteManager = new SpriteManager();
    input = new Input();
    physicsManager = new Physic();

    uiManager->Init();
    objectManager->Init();
    behaviorManager->Init();
    spriteManager->Init();
    input->Init();
}
Core::Core(SDL_Renderer *rend)
{
    renderer = rend;
    uiManager = new UIManager();
    objectManager = new ObjectManager();
    behaviorManager = new BehaviorManager();
    spriteManager = new SpriteManager();
    input = new Input();
    physicsManager = new Physic();

    uiManager->Init();
    objectManager->Init();
    behaviorManager->Init();
    spriteManager->Init();
    input->Init();
}

Core::~Core()
{
    delete uiManager;
    delete objectManager;
    delete behaviorManager;
    delete spriteManager;
    delete input;
    delete physicsManager;
}
void Core::OnUpdate()
{
    input->Update();
    objectManager->UpdateObjects();
    behaviorManager->UpdateBehaviors();
    if (physicsManager)
    {
        physicsManager->Update(1.0f / 60.0f); // 60 FPS physics update
        physicsManager->CheckCollisions();
    }
}
void Core::OnRender()
{
    spriteManager->RenderAllSprites(renderer);
    uiManager->RenderElements(renderer);
}