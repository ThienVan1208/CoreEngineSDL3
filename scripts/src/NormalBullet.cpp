#include "../include/NormalBullet.h"
#include "../../core/include/ObjectManager.h"
#include "../../core/include/components/SpriteRenderer.h"
#include "../../core/include/components/RigidBody.h"
#include "../../core/include/components/Collider.h"
#include <cmath>
#include "../../core/include/Time.h"
#include "../include/DamageDealer.h"

NormalBullet::NormalBullet() : BulletBase()
{
}

void NormalBullet::OnAwake() {}
void NormalBullet::OnStart() {
    speed = 15.0f; 
    if (gameObject && !gameObject->GetComponent<DamageDealer>()) {
        auto damageDealer = std::make_unique<DamageDealer>(10.0f);
        gameObject->AttachComponent(std::move(damageDealer));
    }
}
void NormalBullet::OnUpdate() 
{
    if (gameObject && gameObject->transform)
    {
        Vector2 pos = gameObject->transform->GetPosition();
        if (std::abs(pos.x) > 50.0f || std::abs(pos.y) > 50.0f)
        {
            ReturnToPool(); // Use IPoolable method directly
        }
    }
}

void NormalBullet::Fire(const Vector2& direction)
{
    SDL_Log("Firing NormalBullet with direction: (%f, %f)", direction.x, direction.y);
    // Implement firing logic specific to NormalBullet (e.g. set velocity based on direction and speed)
    if (gameObject && gameObject->transform)
    {
        // Normalize the direction vector
        float magnitude = std::sqrt(direction.x * direction.x + direction.y * direction.y);
        if (magnitude > 0)
        {
            Vector2 normalizedDirection = { direction.x / magnitude, direction.y / magnitude };
            Vector2 velocity = { normalizedDirection.x * speed, normalizedDirection.y * speed };

            // Assuming we have a RigidBody component to set the velocity
            auto rigidBody = gameObject->GetComponent<RigidBody>();
            if (rigidBody)
            {
                // Velocity should NOT be multiplied by deltaTime here! 
                // RigidBody already multiplies by deltaTime internally every frame.
                rigidBody->SetVelocity(velocity);
                SDL_Log("Bullet velocity set to: (%f, %f)", velocity.x, velocity.y);
            }
        }
    }
}

void NormalBullet::OnGetFromPool()
{
}

void NormalBullet::OnReturnToPool()
{
    // Reset physics velocity so it doesn't instantly fly away when gotten again incorrectly
    if (gameObject)
    {
        auto rigidBody = gameObject->GetComponent<RigidBody>();
        if (rigidBody)
        {
            rigidBody->SetVelocity({0, 0});
        }
    }
}

void NormalBullet::OnCollisionEnter(Collider* other)
{
    if (!other || !other->GetObject() || !gameObject) return;
    
    std::string tag = other->GetObject()->tag;
    if (tag == "Wall" || tag == "Enemy")
    {
        ReturnToPool(); // Instantly trigger the injected lambda
    }
}
