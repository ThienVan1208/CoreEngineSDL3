#ifndef WEAPONBASE_H
#define WEAPONBASE_H

#include <SDL3/SDL.h>
#include "../../core/include/ZanBehavior.h"
#include "../../core/include/Coordinate.h"
#include "../../core/include/data-structure/ObjectPool.h"
#include "BulletBase.h"
#include <stack>
#include <cmath>

class Transform;
class GameObject;
class WeaponBase : public ZanBehavior
{
protected:
    ObjectPool<GameObject> bulletPool;
    std::vector<GameObject*> activeBullets; // keep track of fired bullets
    Transform* weaponPosition;
    Transform* firePoint = nullptr;
public:
    WeaponBase();
    virtual ~WeaponBase() = default;
    void OnAwake() override;
    void OnStart() override;
    void OnUpdate() override;

    virtual void Shoot();
    virtual void SetupWeapon(Transform* weaponTransform);
    virtual void HandleWeaponBehavior();
    
};

#endif // WEAPONBASE_H