#include "../include/ObjectManager.h"
#include "../include/components/Component.h"
#include "../include/components/Transform.h"
#include "../include/components/SpriteRenderer.h"
#include "../include/ZanBehavior.h"
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

    // for (auto &object : objects)
    // {

    // }
}

/*
    OBJECT
*/
Object::Object()
{
    transform = new Transform();
    isActive = true;
    rect = {0, 0, 0, 0};
    if (ObjectManager::OnObjectRegister)
        ObjectManager::OnObjectRegister(this);
}
Object::Object(float x, float y, float width, float height) : transform(new Transform())
{
    transform->SetPosition(x, y);
    rect = {x, y, width, height};
    isActive = true;
    if (ObjectManager::OnObjectRegister)
        ObjectManager::OnObjectRegister(this);
}
Object::Object(SDL_FRect initRect) : transform(new Transform())
{
    // Assuming SDL_FRect has x, y, w, h members
    transform->SetPosition(initRect.x, initRect.y);
    rect = initRect;
    isActive = true;
    if (ObjectManager::OnObjectRegister)
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

void Object::AttachComponent(Component* component)
{
    if (!component) return;
    
    // Set gameObject reference for ZanBehavior components
    ZanBehavior* behavior = dynamic_cast<ZanBehavior*>(component);
    if (behavior)
    {
        behavior->SetGameObject(this);
    }
    
    component->OnAttached();
    components.push_back(component);
}

void Object::DetachComponent(Component* component)
{
    if (!component) return;
    component->OnDetached();
    components.erase(std::remove(components.begin(), components.end(), component), components.end());
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
    spriteRenderer->SetSize(width, height);
}
GameObject::GameObject(SDL_FRect initRect) : Object(initRect)
{
    spriteRenderer = new SpriteRenderer(transform);
    spriteRenderer->SetSize(initRect.w, initRect.h);
}

GameObject::GameObject(float x, float y, float width, float height, SpriteRenderer *sprite) : Object(x, y, width, height)
{
    spriteRenderer = sprite;
    spriteRenderer->SetSize(width, height);
}