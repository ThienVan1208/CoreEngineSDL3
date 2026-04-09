#include "../include/PhysicManager.h"
#include "../include/ObjectManager.h"
#include "../include/components/Component.h"
#include "../include/components/Collider.h"
#include "../include/components/RigidBody.h"
#include "../include/components/Transform.h"
#include "../include/ZanBehavior.h"
#include <algorithm>

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

    std::vector<Object*> processedObjects; // Track processed objects to avoid duplicate updates
    
    for (auto collider : colliders)
    {
        if (!collider) continue;
        
        Object* object = collider->GetObject();
        if (!object) continue;
        
        
        if (std::find(processedObjects.begin(), processedObjects.end(), object) != processedObjects.end())
            continue;
        
        processedObjects.push_back(object);
        
        // Find and update RigidBody component
        for (auto& comp : object->GetComponents())
        {
            RigidBody* rigidBody = dynamic_cast<RigidBody*>(comp);
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
        ZanBehavior* behavior = dynamic_cast<ZanBehavior*>(comp);
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
        ZanBehavior* behavior = dynamic_cast<ZanBehavior*>(comp);
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
        ZanBehavior* behavior = dynamic_cast<ZanBehavior*>(comp);
        if (behavior)
        {
            behavior->OnCollisionExit(otherCollider);
        }
    }
}

void Physic::CheckCollisions()
{
    std::set<std::pair<Collider*, Collider*>> newActiveCollisions;

    for (size_t i = 0; i < colliders.size(); i++)
    {
        for (size_t j = i + 1; j < colliders.size(); j++)
        {
            Collider* colliderA = colliders[i];
            Collider* colliderB = colliders[j];
            
            if (!colliderA || !colliderB)
                continue;
            
            Object* objectA = colliderA->GetObject();
            Object* objectB = colliderB->GetObject();
            
            if (!objectA || !objectB || objectA == objectB)
                continue;  // Skip if same object or null
            
            // Get detailed collision info
            CollisionInfo collisionInfo = colliderA->GetCollisionInfo(colliderB);
            
            if (collisionInfo.isColliding)
            {
                // Find RigidBodies on both objects
                RigidBody* rigidBodyA = nullptr;
                RigidBody* rigidBodyB = nullptr;
                
                for (auto& comp : objectA->GetComponents())
                {
                    rigidBodyA = dynamic_cast<RigidBody*>(comp);
                    if (rigidBodyA) break;
                }
                
                for (auto& comp : objectB->GetComponents())
                {
                    rigidBodyB = dynamic_cast<RigidBody*>(comp);
                    if (rigidBodyB) break;
                }
                
                // At least one object must have a rigidbody
                if (rigidBodyA || rigidBodyB)
                {
                    // Calculate collision normal (from A to B)
                    Vector2 normal = collisionInfo.normal;
                    float penetration = collisionInfo.penetrationDepth;
                    
                    // Separate overlapping objects
                    float positionCorrectionMagnitude = penetration + 0.01f;  // 0.01f = small bias to prevent sticking
                    
                    // SDL_Log("━━━ COLLISION RESPONSE ━━━");
                    // SDL_Log("Normal: (%.2f, %.2f) | Penetration: %.2f", normal.x, normal.y, penetration);
                    
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
                            // SDL_Log("Object A pushed by: (%.2f, %.2f)", pushX, pushY);
                        }
                        else if (rigidBodyA)
                        {
                            // SDL_Log("Object A: Static (not moved)");
                        }
                        
                        if (invMassB > 0.0f)
                        {
                            float pushX = normal.x * positionCorrectionMagnitude * pushRatioB;
                            float pushY = normal.y * positionCorrectionMagnitude * pushRatioB;
                            objectB->transform->Translate(pushX, pushY);
                            // SDL_Log("Object B pushed by: (%.2f, %.2f)", pushX, pushY);
                        }
                        else if (rigidBodyB)
                        {
                            // SDL_Log("Object B: Static (not moved)");
                        }
                    }
                    
                    // Calculate impulse magnitude
                    // Formula: |J| = -(1 + e) * ((vB - vA) · n) / (1/mA + 1/mB)
                    // where e = restitution (0 = inelastic, 1 = elastic), n = collision normal
                    float restitution = 0.5f;  // Bounciness 
                    
                    Vector2 velA = rigidBodyA ? rigidBodyA->GetVelocity() : Vector2(0, 0);
                    Vector2 velB = rigidBodyB ? rigidBodyB->GetVelocity() : Vector2(0, 0);
                    Vector2 relativeVelocity = {velB.x - velA.x, velB.y - velA.y};
                    
                    float velAlongNormal = relativeVelocity.x * normal.x + relativeVelocity.y * normal.y;
                    
                    // Only resolve if objects are moving toward each other
                    if (velAlongNormal < 0)
                    {
                        float massA = (rigidBodyA && rigidBodyA->type == RigidbodyType::Dynamic) ? rigidBodyA->GetMass() : 999999.0f;  // Large mass for static
                        float massB = (rigidBodyB && rigidBodyB->type == RigidbodyType::Dynamic) ? rigidBodyB->GetMass() : 999999.0f;
                        
                        float impulseScalar = -(1 + restitution) * velAlongNormal / (1.0f / massA + 1.0f / massB);
                        
                        Vector2 impulse = {normal.x * impulseScalar, normal.y * impulseScalar};
                        
                        // Apply equal and opposite impulses
                        if (rigidBodyA)
                        {
                            rigidBodyA->ApplyImpulse({-impulse.x, -impulse.y});
                        }
                        
                        if (rigidBodyB)
                        {
                            rigidBodyB->ApplyImpulse(impulse);
                        }
                    }
                }
                
                // Track active collisions properly
                std::pair<Collider*, Collider*> collisionPair;
                if (colliderA < colliderB)
                    collisionPair = {colliderA, colliderB};
                else
                    collisionPair = {colliderB, colliderA};
                
                newActiveCollisions.insert(collisionPair);
                
                // Emitting Correct Callbacks
                if (activeCollisions.find(collisionPair) == activeCollisions.end())
                {
                    // Newly began colliding
                    NotifyCollisionEnter(objectA, colliderB);
                    NotifyCollisionEnter(objectB, colliderA);
                }
                else
                {
                    // Continue colliding
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