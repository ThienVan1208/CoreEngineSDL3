#ifndef OBJECTMANAGER_H
#define OBJECTMANAGER_H

#include <SDL3/SDL.h>
#include <string>
#include <vector>
#include <functional>


class Transform;
class SpriteRenderer;
class Object;

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
public:
    Transform* transform;
    Object();
    Object(float x, float y, float width, float height);
    Object(SDL_FRect initRect);
    virtual ~Object() = default;
    virtual void OnEnable(){};
    virtual void OnDisable(){};
    virtual void OnStart(){};
    virtual void OnUpdate(){};
    
    bool IsActive() const;
    void SetActive(bool active);
};

class GameObject : public Object{
public:
    SpriteRenderer* spriteRenderer;
    GameObject();
    GameObject(float x, float y, float width, float height);
    GameObject(SDL_FRect initRect);

    GameObject(float x, float y, float width, float height, SpriteRenderer* sprite);
};

#endif // OBJECTMANAGER_H