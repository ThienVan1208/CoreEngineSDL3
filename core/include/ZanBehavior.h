#pragma once

#ifndef ZANBEHAVIOR_H
#define ZANBEHAVIOR_H

class Object;

class ZanBehavior
{
protected:
    Object* gameObject;  // Reference to the object this behavior is attached to
    bool hasStarted = false;

public:
    ZanBehavior();
    virtual ~ZanBehavior() = default;

    // Lifecycle methods (like Unity's MonoBehaviour)
    virtual void OnAwake() {}      // Called when behavior is first attached
    virtual void OnStart() {}      // Called before first OnUpdate
    virtual void OnUpdate() {}     // Called every frame
    virtual void OnDestroy() {}    // Called when behavior is destroyed

    // Getters
    Object* GetGameObject() const { return gameObject; }
    void SetGameObject(Object* obj) { gameObject = obj; }

    // Lifecycle state
    bool HasStarted() const { return hasStarted; }
    void SetStarted(bool started) { hasStarted = started; }
};

#endif // ZANBEHAVIOR_H
