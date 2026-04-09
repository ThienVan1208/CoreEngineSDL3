#ifndef PHYSICMANAGER_H
#define PHYSICMANAGER_H

#include <vector>
#include <functional>
#include <set>
#include <utility>
#include "SpatialHash.h"

class Collider;
class RigidBody;
class Object;

class Physic
{
private:
    std::vector<Collider*> colliders; // All colliders in the scene (optimization: these already have references to their objects)
    std::set<std::pair<Collider*, Collider*>> activeCollisions;
    SpatialHash spatialHash;
    const float SLOP = 0.01f; // Positional correction bias (Slop) to prevent jitter
    
    // Helper to call OnCollision on all behaviors of an object
    void NotifyCollisionEnter(Object* collidedObject, Collider* otherCollider);
    void NotifyCollisionStay(Object* collidedObject, Collider* otherCollider);
    void NotifyCollisionExit(Object* collidedObject, Collider* otherCollider);
    
public:
    static std::function<void(Collider*)> OnColliderRegister;
    static std::function<void(Collider*)> OnColliderUnregister;
    
    void RegisterCollider(Collider* collider);
    void UnregisterCollider(Collider* collider);
    void Update(float deltaTime);
    void CheckCollisions();
};

#endif // PHYSICMANAGER_H