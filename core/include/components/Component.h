#pragma once

#ifndef COMPONENT_H
#define COMPONENT_H

class Component
{
public:
    virtual ~Component() = default;
    virtual void OnAttached() {} // Called when component is attached to an object
    virtual void OnDetached() {} // Called when component is detached from an object
};

#endif // COMPONENT_H
