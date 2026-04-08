#include "../../include/components/RigidBody.h"
#include "../../include/ObjectManager.h"
#include "../../include/components/Transform.h"

RigidBody::RigidBody(Object* obj) : object(obj), velocity{0, 0}, mass{1.0f}, acceleration{0, 0}, type{RigidbodyType::Dynamic}
{
    // Need to collect colliders from the object's components array
    for(auto& comp : obj->GetComponents()){
        Collider* collider = dynamic_cast<Collider*>(comp);
        if(collider){
            colliders.push_back(collider);
        }
    }
}
void RigidBody::AddForce(Vector2 force)
{
    if (type != RigidbodyType::Dynamic) return; 

    acceleration = {force.x / mass, force.y / mass};
    velocity.x += acceleration.x;
    velocity.y += acceleration.y;
}
void RigidBody::AddVelocity(Vector2 deltaVel)
{
    velocity.x += deltaVel.x;
    velocity.y += deltaVel.y;
}

void RigidBody::SetVelocity(Vector2 newVel)
{
    velocity = newVel;
}

void RigidBody::OnPhysicsUpdate(float deltaTime)
{
    if (type != RigidbodyType::Dynamic) return; 

    object->transform->Translate(velocity.x * deltaTime, velocity.y * deltaTime);
}

void RigidBody::ApplyImpulse(Vector2 impulse)
{
    if (type != RigidbodyType::Dynamic) return;

    // Impulse = change in velocity (J = m * dv, so dv = J / m)
    velocity.x += impulse.x / mass;
    velocity.y += impulse.y / mass;
}
