#pragma once

#ifndef BEHAVIORMANAGER_H
#define BEHAVIORMANAGER_H

#include <vector>
#include <functional>

class Object;
class ZanBehavior;

class BehaviorManager
{
private:
    std::vector<ZanBehavior*> behaviors;

public:
    BehaviorManager();
    ~BehaviorManager();
    static std::function<void(ZanBehavior*)> OnBehaviorRegister;
    static std::function<void(ZanBehavior*)> OnBehaviorUnregister;

    void Init();

    // Call Awke, Start, Update, Enable, Disable on behaviors based on their state and their game object's state
    void UpdateBehaviors();

    void RegisterBehavior(ZanBehavior* behavior);
    void UnregisterBehavior(ZanBehavior* behavior);
};

#endif // BEHAVIORMANAGER_H
