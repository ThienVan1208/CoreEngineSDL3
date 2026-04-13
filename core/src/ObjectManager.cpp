#include "../include/ObjectManager.h"
#include "../include/components/Component.h"
#include "../include/components/Transform.h"
#include "../include/components/SpriteRenderer.h"
#include "../include/ZanBehavior.h"
#include <algorithm>

/*
    OBJECT MANAGER
*/
ObjectManager::ObjectManager() {}
ObjectManager::~ObjectManager() {}

void ObjectManager::Init()
{
}

void ObjectManager::RegisterObject(std::unique_ptr<Object> object)
{
    if (object) {
        objects.push_back(std::move(object));
    }
}

void ObjectManager::UnregisterObject(Object *object)
{
    auto it = std::remove_if(objects.begin(), objects.end(),
                             [object](const std::unique_ptr<Object>& ptr) { return ptr.get() == object; });
    if (it != objects.end()) {
        objects.erase(it, objects.end());
    }
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
    name = "New Object";
    tag = "Default";
    layer = 0;
    auto tr = std::make_unique<Transform>();
    transform = tr.get();
    AttachComponent(std::move(tr));
    isActive = true;
    rect = {0, 0, 0, 0};
}
Object::Object(float x, float y)
{
    name = "New Object";
    tag = "Default";
    layer = 0;
    auto tr = std::make_unique<Transform>();
    transform = tr.get();
    AttachComponent(std::move(tr));
    transform->SetPosition(x, y);
    rect = {x, y, 0, 0}; // Width and height default to 0
    isActive = true;
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

void Object::AttachComponent(std::unique_ptr<Component> component)
{
    if (!component) return;
    
    // Set gameObject reference for ZanBehavior components
    ZanBehavior* behavior = dynamic_cast<ZanBehavior*>(component.get());
    if (behavior)
    {
        behavior->SetGameObject(this);
    }
    
    component->OnAttached();
    components.push_back(std::move(component));
}

void Object::DetachComponent(Component* component)
{
    if (!component) return;
    component->OnDetached();
    
    auto it = std::remove_if(components.begin(), components.end(),
                             [component](const std::unique_ptr<Component>& ptr) { return ptr.get() == component; });
    if (it != components.end()) {
        components.erase(it, components.end());
    }
}

/*
    GAME OBJECT
*/
GameObject::GameObject() : Object()
{
    auto spr = std::make_unique<SpriteRenderer>(transform);
    spriteRenderer = spr.get();
    AttachComponent(std::move(spr));
}
GameObject::GameObject(float x, float y) : Object(x, y)
{
    auto spr = std::make_unique<SpriteRenderer>(transform);
    spriteRenderer = spr.get();
    AttachComponent(std::move(spr));
}

GameObject::GameObject(float x, float y, SpriteRenderer *sprite) : Object(x, y)
{
    // Warning: passing raw pointers to constructors is deprecated in the new memory model
    // Assuming the user will refactor this constructor out or handle ownership outside.
    spriteRenderer = sprite;
    if (spriteRenderer)
    {
        // Notice: This is unsafe if the raw pointer is not wrapped.
    }
}