#include "../include/ZanBehavior.h"
#include "../include/ObjectManager.h"
#include "../include/components/Component.h"
#include "../include/BehaviorManager.h"
#include <SDL3/SDL.h>

ZanBehavior::ZanBehavior() : gameObject(nullptr), hasAwoken(false), hasStarted(false), isEnabled(true), previouslyEnabled(true)
{
}
ZanBehavior::ZanBehavior(Object *obj) : gameObject(obj), hasAwoken(false), hasStarted(false), isEnabled(true), previouslyEnabled(true)
{
    if (BehaviorManager::OnBehaviorRegister)
        BehaviorManager::OnBehaviorRegister(this);
}

ZanBehavior::~ZanBehavior()
{
    if (BehaviorManager::OnBehaviorUnregister)
        BehaviorManager::OnBehaviorUnregister(this);
}

void ZanBehavior::OnAttached()
{

    if (BehaviorManager::OnBehaviorRegister)
    {
        BehaviorManager::OnBehaviorRegister(this);
    }
}

void ZanBehavior::OnDetached()
{
    if (BehaviorManager::OnBehaviorUnregister)
        BehaviorManager::OnBehaviorUnregister(this);
}
