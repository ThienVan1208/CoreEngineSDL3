#ifndef STRESS_TEST_H
#define STRESS_TEST_H

#include "../../core/include/ZanBehavior.h"
#include "../../core/include/ObjectManager.h"

class StressTest : public ZanBehavior
{
private:
    ObjectManager* objectManager;
    int objectCount;
    int maxObjects;
    int framesToWait;
    int currentFrame;

public:
    StressTest(ObjectManager* manager, int maxLimit = 5000);
    void OnStart() override;
    void OnUpdate() override;
};

#endif // STRESS_TEST_H
