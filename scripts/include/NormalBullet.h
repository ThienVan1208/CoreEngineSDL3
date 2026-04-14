#ifndef NORMALBULLET_H
#define NORMALBULLET_H
#include "BulletBase.h"
class NormalBullet : public BulletBase
{
public:
    NormalBullet();
    void OnAwake() override;
    void OnStart() override;
    void OnUpdate() override;
    void Fire(const Vector2& direction) override;
    
    void OnGetFromPool() override;
    void OnReturnToPool() override;
    
    void OnCollisionEnter(Collider* other) override;
};

#endif