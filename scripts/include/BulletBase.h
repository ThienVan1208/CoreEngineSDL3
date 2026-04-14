#ifndef BULLETBASE_H
#define BULLETBASE_H
#include <SDL3/SDL.h>
#include "../../core/include/ZanBehavior.h"
#include "../../core/include/math/Vector2.h"
#include "../../core/include/data-structure/IPoolable.h"

class BulletBase : public ZanBehavior, public IPoolable
{
protected:
    float speed;
public:

    BulletBase() : speed(0) {}
    void OnAwake() override {}
    void OnStart() override {}
    void OnUpdate() override {}
    void SetSpeed(float newSpeed) { speed = newSpeed; }
    float GetSpeed() const { return speed; }
    virtual void Fire(const Vector2& direction) {}

    // Implement IPoolable interface methods
    void OnGetFromPool() override {}
    void OnReturnToPool() override {}
};

#endif