#pragma once

#ifndef ZANBEHAVIOR_H
#define ZANBEHAVIOR_H
#include "components/Component.h"

class Object;

class ZanBehavior : public Component
{
protected:
    Object *gameObject; 
    bool hasAwoken = false;           // Tracks if OnAwake has been called
    bool hasStarted = false;          // Tracks if OnStart has been called
    bool isEnabled = true;            // Current enabled state
    bool previouslyEnabled = true;    // Previous frame enabled state (for detecting transitions)

public:
    ZanBehavior();
    ZanBehavior(Object *obj);
    virtual ~ZanBehavior();

    virtual void OnAwake() {}                       
    virtual void OnEnable() {}                      
    virtual void OnStart() {}                       
    virtual void OnUpdate() {}                      
    virtual void OnDisable() {}                     
    virtual void OnDestroy() {}                     
    virtual void OnCollisionEnter(Object *other) {} 
    
    virtual void OnAttached() override;             
    virtual void OnDetached() override;             
    

    Object *GetGameObject() const { return gameObject; }
    void SetGameObject(Object *obj) { gameObject = obj; }


    bool HasStarted() const { return hasStarted; }
    bool IsEnabled() const { return isEnabled; }
    bool HasAwoken() const { return hasAwoken; }
    bool GetPreviouslyEnabled() const { return previouslyEnabled; }
    
    void SetEnabled(bool enabled) { isEnabled = enabled; }
    void SetStarted(bool started) { hasStarted = started; }
    void SetAwoken(bool awoken) { hasAwoken = awoken; }
    void SetPreviouslyEnabled(bool enabled) { previouslyEnabled = enabled; }
};

#endif // ZANBEHAVIOR_H
