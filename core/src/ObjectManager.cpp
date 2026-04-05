#include "../include/ObjectManager.h"
#include "../include/components/Component.h"
#include <algorithm>

/*
    OBJECT MANAGER
*/
std::function<void(Object *)> ObjectManager::OnObjectRegister;
std::function<void(Object *)> ObjectManager::OnObjectUnregister;

ObjectManager::ObjectManager()
{
    // Constructor can be empty or used for initialization if needed
}
ObjectManager::~ObjectManager()
{
    OnObjectRegister = nullptr;
    OnObjectUnregister = nullptr;
}

void ObjectManager::Init()
{
    OnObjectRegister = [this](Object *object)
    {
        RegisterObject(object);
    };

    OnObjectUnregister = [this](Object *object)
    {
        UnregisterObject(object);
    };
}

void ObjectManager::RegisterObject(Object *object)
{
    objects.push_back(object);
}

void ObjectManager::UnregisterObject(Object *object)
{
    objects.erase(std::remove(objects.begin(), objects.end(), object), objects.end());
}

void ObjectManager::UpdateObjects()
{
    for (auto &object : objects)
    {
        if (object->IsActive())
            object->OnUpdate();
    }
}

/*
    OBJECT
*/
Object::Object()
{
    transform = new Transform();
    isActive = true;
    rect = {0, 0, 0, 0};
    ObjectManager::OnObjectRegister(this);
}
Object::Object(float x, float y, float width, float height) : transform(new Transform())
{
    transform->SetPosition(x, y);
    rect = {x, y, width, height};
    isActive = true;
    ObjectManager::OnObjectRegister(this);
}
Object::Object(SDL_FRect initRect) : transform(new Transform())
{
    // Assuming SDL_FRect has x, y, w, h members
    transform->SetPosition(initRect.x, initRect.y);
    rect = initRect;
    isActive = true;
    ObjectManager::OnObjectRegister(this);
}

bool Object::IsActive() const
{
    return isActive;
}
void Object::SetActive(bool active)
{
    if (isActive == active)
        return;
    isActive = active;
    if (isActive)
        OnEnable();
    else
        OnDisable();
}

/*
    GAME OBJECT
*/
GameObject::GameObject() : Object()
{
    spriteRenderer = new SpriteRenderer(transform);
}
GameObject::GameObject(float x, float y, float width, float height) : Object(x, y, width, height)
{
    spriteRenderer = new SpriteRenderer(transform);
}
GameObject::GameObject(SDL_FRect initRect) : Object(initRect)
{
    spriteRenderer = new SpriteRenderer(transform);
}

GameObject::GameObject(float x, float y, float width, float height, SpriteRenderer *sprite) : Object(x, y, width, height)
{
    spriteRenderer = sprite;
}