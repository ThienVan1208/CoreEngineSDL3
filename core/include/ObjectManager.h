#ifndef OBJECTMANAGER_H
#define OBJECTMANAGER_H

#include <SDL3/SDL.h>
#include <string>
#include <vector>
#include <functional>
#include <memory>
#include "components/Component.h"

class Transform;
class SpriteRenderer;
class Object;
class ZanBehavior;

class ObjectManager
{
private:
    std::vector<std::unique_ptr<Object>> objects;
    void RegisterObject(std::unique_ptr<Object> object);
    void UnregisterObject(Object *object); // Raw pointer for lookup

public:
    void Init();
    void UpdateObjects();
    ObjectManager();
    ~ObjectManager();
    const std::vector<std::unique_ptr<Object>> &GetObjects() const { return objects; }
    
    // Helper template to cleanly register dynamically created objects
    template<typename T, typename... Args>
    T* CreateObject(Args&&... args) {
        auto obj = std::make_unique<T>(std::forward<Args>(args)...);
        T* raw_ptr = obj.get();
        RegisterObject(std::move(obj));
        return raw_ptr;
    }
};

class Object
{
private:
    bool isActive;
    SDL_FRect rect;
    std::vector<std::unique_ptr<Component>> components; // Ownership of components
public:
    Transform *transform;
    Object();
    Object(float x, float y);
    virtual ~Object() = default;

    virtual void OnEnable() {};
    virtual void OnDisable() {};

    bool IsActive() const;
    void SetActive(bool active);

    float GetWidth() const { return rect.w; }
    float GetHeight() const { return rect.h; }
    SDL_FRect GetRect() const { return rect; }

    template<typename T>
    T* GetComponent() const
    {
        for (const auto& comp : components)
        {
            T* casted = dynamic_cast<T*>(comp.get());
            if (casted)
                return casted;
        }
        return nullptr; // Not found
    }


    void AttachComponent(std::unique_ptr<Component> component);
    void DetachComponent(Component *component);
    const std::vector<std::unique_ptr<Component>> &GetComponents() const { return components; }
};

class GameObject : public Object
{
public:
    SpriteRenderer *spriteRenderer;
    GameObject();
    GameObject(float x, float y);
    GameObject(float x, float y, SpriteRenderer *sprite);

    ~GameObject() override = default;
};

#endif // OBJECTMANAGER_H