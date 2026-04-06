#ifndef OBJECTMANAGER_H
#define OBJECTMANAGER_H

#include <SDL3/SDL.h>
#include <string>
#include <vector>
#include <functional>


class Transform;
class SpriteRenderer;
class Object;
class ZanBehavior;

class ObjectManager{
private:
    std::vector<Object*> objects;
    void RegisterObject(Object* object);
    void UnregisterObject(Object* object);
public:
    static std::function<void(Object*)> OnObjectRegister;
    static std::function<void(Object*)> OnObjectUnregister;
    void Init();
    void UpdateObjects();
    ObjectManager();
    ~ObjectManager();
};

class Object{
private:
    bool isActive;
    SDL_FRect rect;
    std::vector<ZanBehavior*> behaviors;  // All behaviors attached to this object

public:
    Transform* transform;
    Object();
    Object(float x, float y, float width, float height);
    Object(SDL_FRect initRect);
    virtual ~Object() = default;
    
    virtual void OnEnable(){};
    virtual void OnDisable(){};
    
    bool IsActive() const;
    void SetActive(bool active);
    
    // Getters for rect dimensions
    float GetWidth() const { return rect.w; }
    float GetHeight() const { return rect.h; }
    SDL_FRect GetRect() const { return rect; }
    
    // Behavior management
    void AttachBehavior(ZanBehavior* behavior);
    void DetachBehavior(ZanBehavior* behavior);
    const std::vector<ZanBehavior*>& GetBehaviors() const { return behaviors; }
};

class GameObject : public Object{
public:
    SpriteRenderer* spriteRenderer;
    GameObject();
    GameObject(float x, float y, float width, float height);
    GameObject(SDL_FRect initRect);
    GameObject(float x, float y, float width, float height, SpriteRenderer* sprite);

    ~GameObject() override = default;
};

#endif // OBJECTMANAGER_H