#include "../include/PhysicManager.h"
#include "../include/components/Component.h"
#include <algorithm>

std::function<void(Collider*)> Physic::OnColliderRegister;
std::function<void(Collider*)> Physic::OnColliderUnregister;

void Physic::RegisterCollider(Collider* collider)
{
    colliders.push_back(collider);
}

void Physic::UnregisterCollider(Collider* collider)
{
    colliders.erase(std::remove(colliders.begin(), colliders.end(), collider), colliders.end());
}

void Physic::CheckCollisions()
{
    for (size_t i = 0; i < colliders.size(); ++i)
    {
        for (size_t j = i + 1; j < colliders.size(); ++j)
        {
            if (colliders[i]->CheckCollision(colliders[j]))
            {
                colliders[i]->OnCollisionEnter(colliders[j]);
                colliders[j]->OnCollisionEnter(colliders[i]);
            }
        }
    }
}