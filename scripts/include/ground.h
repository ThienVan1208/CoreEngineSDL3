#ifndef GROUND_H
#define GROUND_H

#include "../../core/include/ZanBehavior.h"
#include "../../core/include/components/BoxCollider.h"
#include "../../core/include/components/RigidBody.h"

class Ground : public ZanBehavior
{
private:
    RigidBody* rigidBody;
    BoxCollider* collider;

public:

    Ground();

    void OnStart() override;
};

#endif // GROUND_H
