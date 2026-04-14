#include "../include/WeaponBase.h"
#include "../../core/include/Core.h"
#include "../../core/include/ObjectManager.h"
#include "../../core/include/Input.h"
#include "../../core/include/components/SpriteRenderer.h"
#include "../../core/include/components/BoxCollider.h"
#include "../../core/include/components/RigidBody.h"
#include "../include/BulletBase.h"
#include "../include/NormalBullet.h"
#include <SDL3/SDL.h>
#include <cmath>

WeaponBase::WeaponBase() : ZanBehavior()
{
}

void WeaponBase::OnAwake()
{
}

void WeaponBase::OnStart()
{
    if (!gameObject)
        return;

    gameObject->transform->SetScale(2.0f, 0.2f); // Make weapon smaller for visibility

    // Create a spawn point for bullets at the tip of the weapon
    GameObject* fpObj = Core::objectManager->CreateObject<GameObject>(0.0f, 0.0f);
    fpObj->name = "FirePoint";
    fpObj->transform->SetParent(gameObject->transform);
    fpObj->transform->SetLocalPosition(0.5f, 0.0f); // Half of the weapon's width to put it at the barrel tip
    firePoint = fpObj->transform;

    auto sprRenderer = gameObject->GetComponent<SpriteRenderer>();
    if (sprRenderer)
    {
        sprRenderer->color = {0, 0, 255, 255}; // Blue weapon for visibility
    }

    int initialBulletPoolSize = 10;
    // Initialize bullet pool using lambda factory
    bulletPool = ObjectPool<GameObject>(
        [this]() -> GameObject* { // createFunc
            GameObject* bulletObj = Core::objectManager->CreateObject<GameObject>(-9999.0f, -9999.0f);
            bulletObj->tag = "Bullet";
            bulletObj->transform->SetScale(0.3f, 0.3f);
            
            auto spr = bulletObj->GetComponent<SpriteRenderer>();
            if (spr) {
                spr->color = {0, 0, 0, 255}; // Black bullets
            }
            
            auto rb = std::make_unique<RigidBody>(bulletObj, RigidbodyType::Dynamic, 1.0f);
            rb->useGravity = false;
            bulletObj->AttachComponent(std::move(rb));
            
            auto col = std::make_unique<BoxCollider>(bulletObj, bulletObj->transform);
            bulletObj->AttachComponent(std::move(col));
            
            BulletBase* bulletComp = new NormalBullet();
            bulletComp->SetSpeed(15.0f);
            
            // Inject the ReturnPool action directly into the bullet's IPoolable interface!
            bulletComp->returnToPool = [this, bulletObj]() {
                if (this) {
                    this->bulletPool.ReturnPool(bulletObj);
                }
            };
            
            bulletObj->AttachComponent(std::unique_ptr<Component>(bulletComp));
            
            return bulletObj;
        },
        [](GameObject* obj) { // actionOnGet
            obj->SetActive(true);
            auto bullet = obj->GetComponent<BulletBase>();
            if (bullet) {
                bullet->OnGetFromPool();
            }
        },
        [](GameObject* obj) { // actionOnRelease
            obj->SetActive(false);
            obj->transform->SetPosition(-9999.0f, -9999.0f);
            
            auto bullet = obj->GetComponent<BulletBase>();
            if (bullet) {
                bullet->OnReturnToPool();
            }
        },
        nullptr, // actionOnDestroy
        initialBulletPoolSize
    );
}

void WeaponBase::OnUpdate()
{
}

void WeaponBase::SetupWeapon(Transform* weaponTransform)
{
    weaponPosition = weaponTransform;
    if (gameObject && weaponPosition)
    {
        gameObject->transform->SetParent(weaponPosition);
        gameObject->transform->SetLocalPosition(1.0f, 0.0f); // Offset weapon from its pivot
    }
}

void WeaponBase::Shoot()
{
    if (!gameObject || !gameObject->transform || !Core::objectManager) return;
    if (!weaponPosition) return;

    auto wPos = weaponPosition->GetPosition();
    auto spawnPos = firePoint ? firePoint->GetPosition() : wPos; // Shoot exactly from the gun tip
    auto mousePosition = Input::GetMousePosition();

    float dx = mousePosition.x - wPos.x;
    float dy = mousePosition.y - wPos.y;
    Vector2 direction = {dx, dy};

    // Get a bullet from the real object pool
    GameObject* bulletObj = bulletPool.GetObject();
    if (!bulletObj) return; 

    // Reset its position cleanly back to the gun's barrel tip instead of the player's center
    bulletObj->transform->SetPosition(spawnPos.x, spawnPos.y);

    // Keep track of it so we can iterate them for expiration later if needed
    activeBullets.push_back(bulletObj);

    // Tell the BulletBase to fire off using velocity
    auto bulletComp = bulletObj->GetComponent<NormalBullet>();
    if (bulletComp)
    {
        bulletComp->Fire(direction);
    }
}

void WeaponBase::HandleWeaponBehavior()
{
    auto mousePosition = Input::GetMousePosition();

    if (weaponPosition && gameObject && gameObject->transform)
    {
        float dx = mousePosition.x - weaponPosition->GetPosition().x;
        float dy = mousePosition.y - weaponPosition->GetPosition().y;

        float angleRadians = std::atan2(dy, dx);
        float angleDegrees = angleRadians * (180.0f / 3.14159265f);

        weaponPosition->SetRotation(-angleDegrees);

        // if (weaponPosition->parent) 
        // {
        //     if (dx < 0) {
        //         weaponPosition->parent->SetScale(-1.0f, 1.0f);  
        //         gameObject->transform->SetScale(1.0f, -1.0f); 
        //     } else {
        //         weaponPosition->parent->SetScale(1.0f, 1.0f);   
        //         gameObject->transform->SetScale(1.0f, 1.0f); 
        //     }
        // }
    }
}