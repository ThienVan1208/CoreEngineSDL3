#ifndef OBJECTMANAGER_H
#define OBJECTMANAGER_H

#include <SDL3/SDL.h>
#include <string>
#include <vector>
#include <functional>
#include "components/Component.h"

class Transform;
class SpriteRenderer;
class Object;
class ZanBehavior;

class ObjectManager
{
private:
    std::vector<Object *> objects;
    void RegisterObject(Object *object);
    void UnregisterObject(Object *object);

public:
    static std::function<void(Object *)> OnObjectRegister;
    static std::function<void(Object *)> OnObjectUnregister;
    void Init();
    void UpdateObjects();
    ObjectManager();
    ~ObjectManager();
    const std::vector<Object *> &GetObjects() const { return objects; }
};

class Object
{
private:
    bool isActive;
    SDL_FRect rect;
    std::vector<Component *> components; // All components attached to this object
public:
    Transform *transform;
    Object();
    Object(float x, float y, float width, float height);
    Object(SDL_FRect initRect);
    virtual ~Object() = default;

    virtual void OnEnable() {};
    virtual void OnDisable() {};

    bool IsActive() const;
    void SetActive(bool active);

    float GetWidth() const { return rect.w; }
    float GetHeight() const { return rect.h; }
    SDL_FRect GetRect() const { return rect; }

    void AttachComponent(Component *component);
    void DetachComponent(Component *component);
    const std::vector<Component *> &GetComponents() const { return components; }
};

class GameObject : public Object
{
public:
    SpriteRenderer *spriteRenderer;
    GameObject();
    GameObject(float x, float y, float width, float height);
    GameObject(SDL_FRect initRect);
    GameObject(float x, float y, float width, float height, SpriteRenderer *sprite);

    ~GameObject() override = default;
};

#endif // OBJECTMANAGER_H