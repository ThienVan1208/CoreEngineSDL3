#include "../include/Enemy.h"
#include "../include/player.h"
#include "../../core/include/ObjectManager.h"
#include "../../core/include/components/BoxCollider.h"
#include "../../core/include/components/RigidBody.h"
#include "../../core/include/components/SpriteRenderer.h"
#include "../../core/include/components/Transform.h"
#include "../include/DamageDealer.h"
#include <cmath>

Enemy::Enemy(Player* p) : ZanBehavior(), target(p), rigidBody(nullptr), speed(2.0f) {}

void Enemy::OnStart() {
    if (!gameObject) return;

    gameObject->tag = "Enemy";

    auto col = std::make_unique<BoxCollider>(gameObject, gameObject->transform);
    gameObject->AttachComponent(std::move(col));

    auto rb = std::make_unique<RigidBody>(gameObject, RigidbodyType::Dynamic);
    rb->useGravity = false;
    rb->SetMass(1.0f);
    rigidBody = rb.get();
    gameObject->AttachComponent(std::move(rb));

    // Optional: make enemy look different
    SpriteRenderer* sprRenderer = gameObject->GetComponent<SpriteRenderer>();
    if (sprRenderer) {
        sprRenderer->color = {150, 0, 255, 255}; // Purple-ish color
    }
    
    // Add its own Health element natively.
    auto healthObj = std::make_unique<HealthBase>();
    myHealth = healthObj.get();
    myHealth->SetMaxHp(30.0f); // Fast cache of health
    gameObject->AttachComponent(std::move(healthObj));

    // Attach DamageDealer component for hitting player
    auto damageDealer = std::make_unique<DamageDealer>(15.0f);
    gameObject->AttachComponent(std::move(damageDealer));
}

void Enemy::OnCollisionEnter(Collider* other) 
{
    if (!myHealth || !other || !other->GetObject()) return;

    // Check if it's hit by a bullet
    if (other->GetObject()->tag == "Bullet")
    {
        // Unity standard: Fetch component during isolated collision event
        auto damageDealer = other->GetObject()->GetComponent<DamageDealer>();
        if (damageDealer && damageDealer->damage > 0)
        {
            myHealth->TakeDamage(damageDealer->damage);
        }
    }
}

void Enemy::OnUpdate() {
    if (!target || !rigidBody || !gameObject || !target->GetGameObject()) return;

    Vector2 myPos = gameObject->transform->GetPosition();
    Vector2 targetPos = target->GetGameObject()->transform->GetPosition();

    float dx = targetPos.x - myPos.x;
    float dy = targetPos.y - myPos.y;
    float dist = std::sqrt(dx*dx + dy*dy);

    if (dist > 0) {
        rigidBody->SetVelocity(Vector2((dx / dist) * speed, (dy / dist) * speed));
    }
}