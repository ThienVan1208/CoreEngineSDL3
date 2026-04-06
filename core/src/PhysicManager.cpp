#include "../include/PhysicManager.h"
#include "../include/ObjectManager.h"
#include "../include/components/Component.h"
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

void Physic::NotifyCollisionEnter(Object* collidedObject, Object* otherObject)
{
    if (!collidedObject)
        return;
    
    for (auto& comp : collidedObject->GetComponents())
    {
        ZanBehavior* behavior = dynamic_cast<ZanBehavior*>(comp);
        if (behavior)
        {
            behavior->OnCollisionEnter(otherObject);
        }
    }
}

void Physic::CheckCollisions()
{
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
                    
                    if (rigidBodyA && rigidBodyA->type == RigidbodyType::Dynamic)
                    {
                        objectA->transform->Translate(-normal.x * positionCorrectionMagnitude / 2.0f, 
                                                       -normal.y * positionCorrectionMagnitude / 2.0f);
                    }
                    
                    if (rigidBodyB && rigidBodyB->type == RigidbodyType::Dynamic)
                    {
                        objectB->transform->Translate(normal.x * positionCorrectionMagnitude / 2.0f, 
                                                       normal.y * positionCorrectionMagnitude / 2.0f);
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
                        float massA = rigidBodyA ? rigidBodyA->GetMass() : 999999.0f;  // Large mass for static
                        float massB = rigidBodyB ? rigidBodyB->GetMass() : 999999.0f;
                        
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
                
                // Notify both objects' behaviors (even without rigidbodies)
                NotifyCollisionEnter(objectA, objectB);
                NotifyCollisionEnter(objectB, objectA);
            }
        }
    }
}