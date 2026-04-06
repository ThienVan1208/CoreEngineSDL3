#ifndef PHYSICMANAGER_H
#define PHYSICMANAGER_H

#include <vector>
#include <functional>

class Collider;

class Physic
{
private:
    std::vector<Collider*> colliders;
public:
    static std::function<void(Collider*)> OnColliderRegister;
    static std::function<void(Collider*)> OnColliderUnregister;
    void RegisterCollider(Collider* collider);
    void UnregisterCollider(Collider* collider);
    void CheckCollisions();
};

#endif // PHYSICMANAGER_H