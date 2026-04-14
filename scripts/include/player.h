#ifndef PLAYER_H
#define PLAYER_H
#include <SDL3/SDL.h>
#include "../../core/include/ZanBehavior.h"
#include "../../core/include/Coordinate.h"
#include "../../core/include/components/BoxCollider.h"
#include "../../core/include/components/RigidBody.h"
#include "HealthBase.h"
#include <memory>
class WeaponBase;
class GameObject;
class Player : public ZanBehavior
{
private:
    RigidBody* rigidBody;
    BoxCollider* collider;
    WeaponBase* equippedWeapon;
    HealthBase* myHealth; // Cached fast pointer
    std::unique_ptr<Object> weaponPivot;

public:
    float speed = 1.0f;      

    Player();
    void GetMovement();
    void SetupWeapon(WeaponBase* weapon);
    void ChangeWeapon(WeaponBase* newWeapon);
    void HandleWeapon();
    void OnAwake() override;
    void OnStart() override;
    void OnUpdate() override;
    void OnCollisionEnter(Collider* other) override;
};

#endif // PLAYER_H