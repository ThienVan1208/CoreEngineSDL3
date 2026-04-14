#include "../include/player.h"
#include "../../core/include/Core.h"
#include "../../core/include/ObjectManager.h"
#include "../../core/include/Input.h"
#include "../../core/include/components/SpriteRenderer.h"
#include <SDL3/SDL.h>
#include <cmath>
#include "../include/WeaponBase.h"
#include "../../core/include/Time.h"
#include "../include/DamageDealer.h"

Player::Player()
    : ZanBehavior(), rigidBody(nullptr), collider(nullptr)
{
}

void Player::OnAwake()
{
}

void Player::OnStart()
{
    if (!gameObject)
        return;

    gameObject->tag = "Player";

    speed = 10.0f; // Scale to logical unit speed
    
    // Create and attach collider
    auto col = std::make_unique<BoxCollider>(gameObject, gameObject->transform);
    collider = col.get();
    gameObject->AttachComponent(std::move(col));

    // Create and attach rigidbody
    auto rb = std::make_unique<RigidBody>(gameObject, RigidbodyType::Dynamic, 1.0f);
    rb->useGravity = false;
    rigidBody = rb.get();
    
    gameObject->AttachComponent(std::move(rb));

    SpriteRenderer* sprRenderer = gameObject->GetComponent<SpriteRenderer>();
    if (sprRenderer)
    {
        sprRenderer->color = {255, 0, 0, 255}; // Red player for visibility
    }
    SDL_Log("Player collider: %f, %f", collider->size.x, collider->size.y);

    // Setup highly accessible custom health
    auto healthObj = std::make_unique<HealthBase>();
    myHealth = healthObj.get();
    myHealth->SetMaxHp(100.0f);
    gameObject->AttachComponent(std::move(healthObj));
}

void Player::SetupWeapon(WeaponBase* weapon)
{
    if (!weaponPivot) {
        weaponPivot = std::make_unique<Object>();
        weaponPivot->transform->SetParent(gameObject->transform);
        weaponPivot->transform->SetLocalPosition(0.0f, 0.0f); // Pivot at player center
    }

    equippedWeapon = weapon;
    weapon->SetupWeapon(weaponPivot->transform);
}
void Player::ChangeWeapon(WeaponBase* newWeapon)
{
    if (equippedWeapon)
    {
        // Detach current weapon
        equippedWeapon->GetGameObject()->transform->SetParent(nullptr);
    }
    equippedWeapon = newWeapon;
    SetupWeapon(equippedWeapon);
}

void Player::HandleWeapon()
{
    if (equippedWeapon && Input::GetKeyDown(SDL_SCANCODE_SPACE))
    {
        equippedWeapon->Shoot();
    }
}


void Player::GetMovement()
{
    if (!gameObject || !rigidBody || !collider)
        return;

    // Movement for Top-Down logic
    float vx = 0.0f;
    float vy = 0.0f;
    
    if (Input::GetKey(SDL_SCANCODE_LEFT) || Input::GetKey(SDL_SCANCODE_A)){
        vx = -speed;
    }
        
    if (Input::GetKey(SDL_SCANCODE_RIGHT) || Input::GetKey(SDL_SCANCODE_D)){
        vx = speed;
    }
        
    if (Input::GetKey(SDL_SCANCODE_UP) || Input::GetKey(SDL_SCANCODE_W))
        vy = speed;
    if (Input::GetKey(SDL_SCANCODE_DOWN) || Input::GetKey(SDL_SCANCODE_S))
        vy = -speed;

    rigidBody->SetVelocity(Vector2(vx, vy));
}

void Player::OnUpdate()
{
    GetMovement();
    HandleWeapon();
    
    if (equippedWeapon)
    {
        equippedWeapon->HandleWeaponBehavior();
    }
}

void Player::OnCollisionEnter(Collider *other)
{
    if (other->GetObject()->tag == "Enemy")
    {
        if (myHealth)
        {
            auto damageDealer = other->GetObject()->GetComponent<DamageDealer>();
            if (damageDealer && damageDealer->damage > 0)
            {
                myHealth->TakeDamage(damageDealer->damage);
                SDL_Log("Player touched Enemy! Took %f damage. Current HP: %f", damageDealer->damage, myHealth->GetCurrentHp());
            }
        }
    }
}
