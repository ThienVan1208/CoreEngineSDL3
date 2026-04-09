#include "../include/Core.h"
#include "../include/RenderManager.h"
#include "../include/ObjectManager.h"
#include "../include/BehaviorManager.h"
#include "../include/Input.h"
#include "../include/PhysicManager.h"

SDL_Renderer *Core::renderer = nullptr;
Physic *Core::physicsManager = nullptr;

Core::Core()
{
    renderManager = new RenderManager();
    objectManager = new ObjectManager();
    behaviorManager = new BehaviorManager();
    input = new Input();
    physicsManager = new Physic();

    renderManager->Init();
    objectManager->Init();
    behaviorManager->Init();
    input->Init();
}
Core::Core(SDL_Renderer *rend)
{
    renderer = rend;
    renderManager = new RenderManager();
    objectManager = new ObjectManager();
    behaviorManager = new BehaviorManager();
    input = new Input();
    physicsManager = new Physic();

    renderManager->Init();
    objectManager->Init();
    behaviorManager->Init();
    input->Init();
}

Core::~Core()
{
    delete renderManager;
    delete objectManager;
    delete behaviorManager;
    delete input;
    delete physicsManager;
}
void Core::OnUpdate()
{
    objectManager->UpdateObjects();
    behaviorManager->UpdateBehaviors();
    if (physicsManager)
    {
        physicsManager->Update(1.0f / 60.0f); // 60 FPS physics update
        physicsManager->CheckCollisions();
    }

    input->Update(); // Update previous state at the end of the frame
}
void Core::OnRender()
{
    renderManager->RenderAll(renderer);
}