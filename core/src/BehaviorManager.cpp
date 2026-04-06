#include "../include/BehaviorManager.h"
#include "../include/ZanBehavior.h"
#include "../include/ObjectManager.h"
#include <algorithm>
std::function<void(ZanBehavior *)> BehaviorManager::OnBehaviorRegister;
std::function<void(ZanBehavior *)> BehaviorManager::OnBehaviorUnregister;

BehaviorManager::BehaviorManager()
{
}

BehaviorManager::~BehaviorManager()
{
    behaviors.clear();
}

void BehaviorManager::Init()
{

    OnBehaviorRegister = [this](ZanBehavior *behavior)
    {
        RegisterBehavior(behavior);
    };

    OnBehaviorUnregister = [this](ZanBehavior *behavior)
    {
        UnregisterBehavior(behavior);
    };
}

void BehaviorManager::UpdateBehaviors()
{
    for(auto& behavior : behaviors)
    {
        if (!behavior->GetGameObject())
            continue;

        // Determine if behavior should be active
        bool objectActive = behavior->GetGameObject()->IsActive();
        bool behaviorEnabled = behavior->IsEnabled();
        bool isCurrentlyActive = objectActive && behaviorEnabled;


        if (!behavior->HasAwoken())
        {
            behavior->OnAwake();
            behavior->SetAwoken(true);
        }


        bool wasEnabled = behavior->GetPreviouslyEnabled();
        
        // Transition: disabled -> enabled
        if (!wasEnabled && isCurrentlyActive)
        {
            behavior->OnEnable();
        }
        // Transition: enabled -> disabled
        else if (wasEnabled && !isCurrentlyActive)
        {
            behavior->OnDisable();
        }

        // Update previous state for next frame
        behavior->SetPreviouslyEnabled(isCurrentlyActive);

        if (!isCurrentlyActive)
            continue;


        if (!behavior->HasStarted())
        {
            behavior->OnStart();
            behavior->SetStarted(true);
        }

        behavior->OnUpdate();
    }
}

void BehaviorManager::RegisterBehavior(ZanBehavior *behavior)
{
    if (!behavior)
        return;
    behaviors.push_back(behavior);
}

void BehaviorManager::UnregisterBehavior(ZanBehavior *behavior)
{
    if (!behavior)
        return;
    behaviors.erase(std::remove(behaviors.begin(), behaviors.end(), behavior), behaviors.end());
}
