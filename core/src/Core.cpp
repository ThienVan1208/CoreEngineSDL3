#include "../include/Core.h"
#include "../include/RenderManager.h"
#include "../include/ObjectManager.h"
#include "../include/BehaviorManager.h"
#include "../include/Input.h"
#include "../include/PhysicManager.h"

SDL_Renderer *Core::renderer = nullptr;
std::unique_ptr<Physic> Core::physicsManager = nullptr;
std::unique_ptr<ObjectManager> Core::objectManager = nullptr;

Core::Core()
{
    renderManager = std::make_unique<RenderManager>();
    objectManager = std::make_unique<ObjectManager>();
    behaviorManager = std::make_unique<BehaviorManager>();
    input = std::make_unique<Input>();
    physicsManager = std::make_unique<Physic>();

    renderManager->Init();
    objectManager->Init();
    behaviorManager->Init();
    input->Init();
}
Core::Core(SDL_Renderer *rend)
{
    renderer = rend;
    renderManager = std::make_unique<RenderManager>();
    objectManager = std::make_unique<ObjectManager>();
    behaviorManager = std::make_unique<BehaviorManager>();
    input = std::make_unique<Input>();
    physicsManager = std::make_unique<Physic>();

    renderManager->Init();
    objectManager->Init();
    behaviorManager->Init();
    input->Init();
}

Core::~Core()
{
    // unique_ptr automatically cleans up memory
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