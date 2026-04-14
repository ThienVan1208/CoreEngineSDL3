#ifndef OBJECTPOOL_H
#define OBJECTPOOL_H

#include <queue>
#include <functional>
#include "IPoolable.h"

class Object;

template <typename T>
class ObjectPool
{
private:
    std::queue<T*> pool;
    std::function<T*()> createFunc;
    std::function<void(T*)> actionOnGet;
    std::function<void(T*)> actionOnRelease;
    std::function<void(T*)> actionOnDestroy;

public:
    ObjectPool() {}

    // Mimic Unity's ObjectPool with actions
    ObjectPool(std::function<T*()> createFunc, 
               std::function<void(T*)> actionOnGet = nullptr, 
               std::function<void(T*)> actionOnRelease = nullptr, 
               std::function<void(T*)> actionOnDestroy = nullptr, 
               int initialSize = 10) 
        : createFunc(createFunc), actionOnGet(actionOnGet), 
          actionOnRelease(actionOnRelease), actionOnDestroy(actionOnDestroy)
    {
        for (int i = 0; i < initialSize; ++i)
        {
            T* obj = createFunc();
            if (obj)
            {
                if (actionOnRelease) actionOnRelease(obj); // Initialize state as released
                pool.push(obj);
            }
        }
    }
    
    ~ObjectPool()
    {
        while (!pool.empty())
        {
            if (actionOnDestroy) actionOnDestroy(pool.front());
            pool.pop();
        }
    }
    
    T *GetObject()
    {
        T* obj = nullptr;
        if (pool.empty())
        {
            if (createFunc) 
            {
                obj = createFunc(); // Dynamically grow if empty
            }
        }
        else
        {
            obj = pool.front();
            pool.pop();
        }
        
        if (obj && actionOnGet)
        {
            actionOnGet(obj);
        }
        return obj;
    }
    
    void ReturnPool(T *obj)
    {
        if (obj)
        {
            if (actionOnRelease) actionOnRelease(obj);
            pool.push(obj);
        }
    }
};

#endif // OBJECTPOOL_H
