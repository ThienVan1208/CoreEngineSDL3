#ifndef PHYSICMANAGER_H
#define PHYSICMANAGER_H

#include <vector>
#include <functional>

class Collider;
class RigidBody;
class Object;

class Physic
{
private:
    std::vector<Collider*> colliders; // All colliders in the scene (optimization: these already have references to their objects)
    
    // Helper to call OnCollisionEnter on all behaviors of an object
    void NotifyCollisionEnter(Object* collidedObject, Object* otherObject);
    
public:
    static std::function<void(Collider*)> OnColliderRegister;
    static std::function<void(Collider*)> OnColliderUnregister;
    
    void RegisterCollider(Collider* collider);
    void UnregisterCollider(Collider* collider);
    void Update(float deltaTime);
    void CheckCollisions();
};

#endif // PHYSICMANAGER_H