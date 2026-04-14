#include "../include/ground.h"
#include "../../core/include/ObjectManager.h"
#include <SDL3/SDL.h>

Ground::Ground()
    : ZanBehavior(), rigidBody(nullptr), collider(nullptr)
{
}

void Ground::OnStart()
{
    if (!gameObject)
        return;

    // Create and attach collider
    auto col = std::make_unique<BoxCollider>(gameObject, gameObject->transform);
    collider = col.get();
    gameObject->AttachComponent(std::move(col));

    // Custom size/offset mapping can be done after attachment since it's raw ptr
    collider->offset.y = 0;
    collider->offset.x = 0;
    
    // Create and attach rigidbody as static
    auto rb = std::make_unique<RigidBody>(gameObject, RigidbodyType::Static);
    rigidBody = rb.get();
    gameObject->AttachComponent(std::move(rb));
    
    // Static objects do not bounce inherently

    SDL_Log("Ground: pos=(%.1f, %.1f) size=(%.1f, %.1f)",
            gameObject->transform->GetPosition().x,
            gameObject->transform->GetPosition().y,
            collider->size.x,
            collider->size.y);
}
