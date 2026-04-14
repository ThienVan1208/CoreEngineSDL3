#ifndef HEALTHBASE_H
#define HEALTHBASE_H

#include "../../core/include/ZanBehavior.h"
#include "../../core/include/ObjectManager.h"
#include <SDL3/SDL.h>

class HealthBase : public ZanBehavior
{
protected:
    float maxHp;
    float currentHp;

public:
    HealthBase() : maxHp(100.0f), currentHp(100.0f) {}

    void SetMaxHp(float hp) 
    { 
        maxHp = hp; 
        currentHp = hp; 
    }

    float GetCurrentHp() const { return currentHp; }

    virtual void TakeDamage(float damageAmount)
    {
        if (currentHp <= 0) return; // Already dead

        currentHp -= damageAmount;
        SDL_Log("[%s] took %.1f damage! Remaining HP: %.1f", 
            gameObject ? gameObject->name.c_str() : "Entity", 
            damageAmount, 
            currentHp);

        if (currentHp <= 0)
        {
            currentHp = 0;
            Die();
        }
    }

    virtual void Die() 
    {
        SDL_Log("[%s] has died!", gameObject ? gameObject->name.c_str() : "Entity");
        if (gameObject)
        {
            gameObject->SetActive(false); // Default logic: Disable the object
        }
    }
};

#endif // HEALTHBASE_H