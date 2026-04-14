#ifndef ENEMY_H
#define ENEMY_H

#include "../../core/include/ZanBehavior.h"
#include "HealthBase.h"

class Player;
class RigidBody;

class Enemy : public ZanBehavior {
private:
    Player* target;
    RigidBody* rigidBody;
    HealthBase* myHealth; // Cached pointer for blazingly fast access!
    float speed;

public:
    Enemy(Player* p);
    void OnStart() override;
    void OnUpdate() override;
    void OnCollisionEnter(Collider* other) override;
};

#endif // ENEMY_H