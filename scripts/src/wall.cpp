#include "../include/wall.h"
#include "../../core/include/ObjectManager.h"
#include <SDL3/SDL.h>

Wall::Wall()
    : ZanBehavior(), rigidBody(nullptr), collider(nullptr)
{
}

void Wall::OnAwake()
{
}

void Wall::OnStart()
{
    if (!gameObject)
        return;

    gameObject->tag = "Wall";

    // Create and attach BoxCollider
    auto col = std::make_unique<BoxCollider>(gameObject, gameObject->transform);
    collider = col.get();
    gameObject->AttachComponent(std::move(col));

    // Create and attach RigidBody as Static
    auto rb = std::make_unique<RigidBody>(gameObject, RigidbodyType::Static);
    rigidBody = rb.get();
    gameObject->AttachComponent(std::move(rb));

    SDL_Log("Wall collider: %f, %f", collider->size.x, collider->size.y);
    SDL_Log("Wall Position: (%.1f, %.1f)", gameObject->transform->GetPosition().x, gameObject->transform->GetPosition().y);
}

void Wall::OnUpdate()
{
}

void Wall::OnCollisionEnter(Collider* other)
{
    if (!other) return;
    SDL_Log("Wall hit by object at: (%.1f, %.1f)", other->GetObject()->transform->GetPosition().x, other->GetObject()->transform->GetPosition().y);
}
