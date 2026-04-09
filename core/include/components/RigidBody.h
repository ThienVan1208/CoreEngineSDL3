#pragma once

#ifndef RIGIDBODY_H
#define RIGIDBODY_H

#include <SDL3/SDL.h>
#include <vector>
#include "Component.h"
#include "Collider.h"
#include "../math/Vector2.h"

// Forward declaration
class Object;

static constexpr float GRAVITY = -980.0f; // Units/s² (pixels, Y-up)

enum RigidbodyType
{
    Dynamic,
    Kinematic,
    Static
};

class RigidBody : public Component
{
private:
    Vector2 velocity;
    float mass;
    float bounciness;
    Vector2 acceleration;
    std::vector<Collider*> colliders; 
    Object* object; 

public:
    RigidbodyType type;
    bool useGravity = true;
    float gravityScale = 1.0f;

    RigidBody(Object* obj, RigidbodyType type = RigidbodyType::Dynamic, float mass = 1.0f);

    void AddForce(Vector2 force);
    void AddVelocity(Vector2 deltaVel);

    void SetVelocity(Vector2 newVel);
    Vector2 GetVelocity() const { return velocity; }
    
    void SetMass(float newMass) { mass = newMass; }
    float GetMass() const { return mass; }

    void SetBounciness(float newBounciness) { bounciness = newBounciness; }
    float GetBounciness() const { return bounciness; }
    
    void OnPhysicsUpdate(float deltaTime);
    Object* GetObject() const { return object; }
    std::vector<Collider*> GetColliders() const { return colliders; }

    void ApplyImpulse(Vector2 impulse);
};

#endif // RIGIDBODY_H
