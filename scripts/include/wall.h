#ifndef WALL_H
#define WALL_H
#include <SDL3/SDL.h>
#include "../../core/include/ZanBehavior.h"
#include "../../core/include/Coordinate.h"
#include "../../core/include/components/BoxCollider.h"
#include "../../core/include/components/RigidBody.h"

class Wall : public ZanBehavior
{
private:
    RigidBody* rigidBody;
    BoxCollider* collider;

public:
    Wall();
    
    void OnAwake() override;
    void OnStart() override;
    void OnUpdate() override;
    void OnCollisionEnter(Collider* other) override;
};

#endif // WALL_H
