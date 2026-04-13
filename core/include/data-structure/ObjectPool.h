#ifndef OBJECTPOOL_H
#define OBJECTPOOL_H

#include <queue>
#include <memory>

class Object;

template <typename T>
class ObjectPool
{
private:
    Object *prefab;
    std::queue<std::unique_ptr<T>> pool;
    int initialSize;

public:
    ObjectPool() : prefab(nullptr), initialSize(0) {}
    ObjectPool(Object *prefab, int initialSize = 10) : prefab(prefab), initialSize(initialSize)
    {
        pool = std::queue<std::unique_ptr<T>>();
        for (int i = 0; i < initialSize; ++i)
        {
            auto obj = std::make_unique<T>();
            // obj->SetActive(false);
            pool.push(std::move(obj));
        }
    }
    
    ~ObjectPool()
    {
        while (!pool.empty())
        {
            pool.pop();
        }
    }
    
    T *GetObject()
    {
        if (pool.empty())
        {
            return nullptr;
        }
        else
        {
            std::unique_ptr<T> obj = std::move(pool.front());
            pool.pop();
            return obj.release();
        }
    }
    
    void ReturnPool(T *obj)
    {
        if (obj)
        {
            // obj->SetActive(false);
            pool.push(std::unique_ptr<T>(obj));
        }
    }
};

#endif // OBJECTPOOL_H
