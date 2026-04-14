#include "../include/stress_test.h"
#include "../../core/include/components/BoxCollider.h"
#include "../../core/include/components/RigidBody.h"
#include <cstdlib>

StressTest::StressTest(ObjectManager* manager, int maxLimit)
    : ZanBehavior(), objectManager(manager), objectCount(0), maxObjects(maxLimit), framesToWait(2), currentFrame(0)
{
}

void StressTest::OnStart()
{
}

void StressTest::OnUpdate()
{
    if (!objectManager) return;

    if (objectCount < maxObjects)
    {
        currentFrame++;
        if (currentFrame >= framesToWait)
        {
            currentFrame = 0;
            // Spawn 10 objects at a time
            for (int i = 0; i < 10 && objectCount < maxObjects; i++)
            {
                // Random position near the top
                float x = (std::rand() % 800) - 400.0f;
                float y = (std::rand() % 200) + 200.0f;

                GameObject* newObj = objectManager->CreateObject<GameObject>(x, y);

                auto col = std::make_unique<BoxCollider>(newObj, newObj->transform);
                // Hardcode small size for the stress test objects
                // col->size.x = 20;
                // col->size.y = 20;
                newObj->AttachComponent(std::move(col));

                auto rb = std::make_unique<RigidBody>(newObj, RigidbodyType::Dynamic);
                rb->SetMass(1.0f);
                rb->SetBounciness(1.0f);
                newObj->AttachComponent(std::move(rb));

                objectCount++;
            }
        }
    }
}
