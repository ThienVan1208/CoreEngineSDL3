#ifndef DAMAGEDEALER_H
#define DAMAGEDEALER_H

#include "../../core/include/ZanBehavior.h"

class DamageDealer : public ZanBehavior
{
public:
    float damage = 0.0f;

    DamageDealer() {}
    DamageDealer(float dmg) : damage(dmg) {}

    void OnStart() override {}
    void OnUpdate() override {}
};

#endif // DAMAGEDEALER_H