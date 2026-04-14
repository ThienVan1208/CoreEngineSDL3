#include "../include/PhysicManager.h"
#include "../include/ObjectManager.h"
#include "../include/components/Component.h"
#include "../include/components/Collider.h"
#include "../include/components/RigidBody.h"
#include "../include/components/Transform.h"
#include "../include/ZanBehavior.h"
#include <algorithm>
#include <unordered_set>

std::function<void(Collider *)> Physic::OnColliderRegister;
std::function<void(Collider *)> Physic::OnColliderUnregister;

void Physic::RegisterCollider(Collider *collider)
{
    colliders.push_back(collider);
}

void Physic::UnregisterCollider(Collider *collider)
{
    colliders.erase(std::remove(colliders.begin(), colliders.end(), collider), colliders.end());
    
    // Remove any active collisions involving this collider
    for (auto it = activeCollisions.begin(); it != activeCollisions.end(); )
    {
        if (it->first == collider || it->second == collider)
        {
            Collider* other = (it->first == collider) ? it->second : it->first;
            
            if (other && other->GetObject())
            {
                NotifyCollisionExit(other->GetObject(), collider);
            }
            if (collider->GetObject())
            {
                NotifyCollisionExit(collider->GetObject(), other);
            }
            
            it = activeCollisions.erase(it);
        }
        else
        {
            ++it;
        }
    }
}

void Physic::Update(float deltaTime)
{

    std::unordered_set<Object*> processedObjects; // Track processed objects to avoid duplicate updates
    spatialHash.Clear();
    
    for (auto collider : colliders)
    {
        if (!collider) continue;
        
        Object* object = collider->GetObject();
        if (!object) continue;
        
        
        if (!processedObjects.insert(object).second)
            continue;
        
        // Add to spatial hash for collision detection
        spatialHash.Insert(collider);
        
        // Find and update RigidBody component
        for (auto& comp : object->GetComponents())
        {
            RigidBody* rigidBody = dynamic_cast<RigidBody*>(comp.get());
            if (rigidBody)
            {
                rigidBody->OnPhysicsUpdate(deltaTime);
                break; // Only one RigidBody per object
            }
        }
    }
}

void Physic::NotifyCollisionEnter(Object* collidedObject, Collider* otherCollider)
{
    if (!collidedObject)
        return;
    
    for (auto& comp : collidedObject->GetComponents())
    {
        ZanBehavior* behavior = dynamic_cast<ZanBehavior*>(comp.get());
        if (behavior)
        {
            behavior->OnCollisionEnter(otherCollider);
        }
    }
}

void Physic::NotifyCollisionStay(Object* collidedObject, Collider* otherCollider)
{
    if (!collidedObject)
        return;
    
    for (auto& comp : collidedObject->GetComponents())
    {
        ZanBehavior* behavior = dynamic_cast<ZanBehavior*>(comp.get());
        if (behavior)
        {
            behavior->OnCollisionStay(otherCollider);
        }
    }
}

void Physic::NotifyCollisionExit(Object* collidedObject, Collider* otherCollider)
{
    if (!collidedObject)
        return;
    
    for (auto& comp : collidedObject->GetComponents())
    {
        ZanBehavior* behavior = dynamic_cast<ZanBehavior*>(comp.get());
        if (behavior)
        {
            behavior->OnCollisionExit(otherCollider);
        }
    }
}

void Physic::CheckCollisions()
{
    std::set<std::pair<Collider*, Collider*>> newActiveCollisions;
    std::set<std::pair<Collider*, Collider*>> checkedPairs;

    for (auto colliderA : colliders)
    {
        if (!colliderA) continue;
        Object* objectA = colliderA->GetObject();
        if (!objectA) continue;

        auto neighbors = spatialHash.Query(colliderA);

        for (auto colliderB : neighbors)
        {
            if (!colliderB || colliderA == colliderB)
                continue;

            Object* objectB = colliderB->GetObject();
            if (!objectB || objectA == objectB)
                continue;

            // Ensure we check each pair only once
            std::pair<Collider*, Collider*> pair = (colliderA < colliderB) ? std::make_pair(colliderA, colliderB) : std::make_pair(colliderB, colliderA);
            if (checkedPairs.count(pair)) continue;
            checkedPairs.insert(pair);

            // Get detailed collision info
            CollisionInfo collisionInfo = colliderA->GetCollisionInfo(colliderB);

            if (collisionInfo.isColliding)
            {
                // Find RigidBodies on both objects
                RigidBody* rigidBodyA = nullptr;
                RigidBody* rigidBodyB = nullptr;

                for (auto& comp : objectA->GetComponents())
                {
                    rigidBodyA = dynamic_cast<RigidBody*>(comp.get());
                    if (rigidBodyA) break;
                }

                for (auto& comp : objectB->GetComponents())
                {
                    rigidBodyB = dynamic_cast<RigidBody*>(comp.get());
                    if (rigidBodyB) break;
                }

                // At least one object must have a rigidbody
                if (rigidBodyA || rigidBodyB)
                {
                    // Calculate collision normal (from A to B)
                    Vector2 normal = collisionInfo.normal;
                    float penetration = collisionInfo.penetrationDepth;

                    // Separate overlapping objects using SLOP bias
                    if (penetration > SLOP)
                    {
                        // Use a percentage of the penetration (Linear Projection) to prevent jitter
                        const float percent = 0.8f; // Penetration percentage to correct
                        float positionCorrectionMagnitude = (penetration - SLOP) * percent;

                        float invMassA = (rigidBodyA && rigidBodyA->type == RigidbodyType::Dynamic) ? 1.0f / rigidBodyA->GetMass() : 0.0f;
                        float invMassB = (rigidBodyB && rigidBodyB->type == RigidbodyType::Dynamic) ? 1.0f / rigidBodyB->GetMass() : 0.0f;
                        float sumInvMass = invMassA + invMassB;

                        if (sumInvMass > 0.0f)
                        {
                            float pushRatioA = invMassA / sumInvMass;
                            float pushRatioB = invMassB / sumInvMass;

                            if (invMassA > 0.0f)
                            {
                                float pushX = -normal.x * positionCorrectionMagnitude * pushRatioA;
                                float pushY = -normal.y * positionCorrectionMagnitude * pushRatioA;
                                objectA->transform->Translate(pushX, pushY);
                            }

                            if (invMassB > 0.0f)
                            {
                                float pushX = normal.x * positionCorrectionMagnitude * pushRatioB;
                                float pushY = normal.y * positionCorrectionMagnitude * pushRatioB;
                                objectB->transform->Translate(pushX, pushY);
                            }
                        }
                    }

                    // Calculate impulse magnitude
                    // Get bounciness from each RigidBody, default to 0.0f
                    float bounceA = rigidBodyA ? rigidBodyA->GetBounciness() : 0.0f;
                    float bounceB = rigidBodyB ? rigidBodyB->GetBounciness() : 0.0f;
                    float restitution = std::max(bounceA, bounceB);

                    Vector2 velA = rigidBodyA ? rigidBodyA->GetVelocity() : Vector2(0, 0);
                    Vector2 velB = rigidBodyB ? rigidBodyB->GetVelocity() : Vector2(0, 0);
                    Vector2 relativeVelocity = { velB.x - velA.x, velB.y - velA.y };

                    float velAlongNormal = relativeVelocity.x * normal.x + relativeVelocity.y * normal.y;

                    // Only resolve if objects are moving toward each other
                    if (velAlongNormal < 0)
                    {
                        float massA = (rigidBodyA && rigidBodyA->type == RigidbodyType::Dynamic) ? rigidBodyA->GetMass() : 999999.0f;
                        float massB = (rigidBodyB && rigidBodyB->type == RigidbodyType::Dynamic) ? rigidBodyB->GetMass() : 999999.0f;

                        float impulseScalar = -(1 + restitution) * velAlongNormal / (1.0f / massA + 1.0f / massB);

                        Vector2 impulse = { normal.x * impulseScalar, normal.y * impulseScalar };

                        if (rigidBodyA) rigidBodyA->ApplyImpulse({ -impulse.x, -impulse.y });
                        if (rigidBodyB) rigidBodyB->ApplyImpulse(impulse);
                    }
                }

                newActiveCollisions.insert(pair);

                // Emitting Correct Callbacks
                if (activeCollisions.find(pair) == activeCollisions.end())
                {
                    NotifyCollisionEnter(objectA, colliderB);
                    NotifyCollisionEnter(objectB, colliderA);
                }
                else
                {
                    NotifyCollisionStay(objectA, colliderB);
                    NotifyCollisionStay(objectB, colliderA);
                }
            }
        }
    }
    
    // Find all past collisions that are no longer active to emit OnCollisionExit
    for (const auto& pastCollision : activeCollisions)
    {
        if (newActiveCollisions.find(pastCollision) == newActiveCollisions.end())
        {
            Collider* colliderA = pastCollision.first;
            Collider* colliderB = pastCollision.second;
            
            if (colliderA && colliderA->GetObject())
                NotifyCollisionExit(colliderA->GetObject(), colliderB);
            
            if (colliderB && colliderB->GetObject())
                NotifyCollisionExit(colliderB->GetObject(), colliderA);
        }
    }
    
    // Update active collisions for next frame
    activeCollisions = newActiveCollisions;
}