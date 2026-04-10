#pragma once

#ifndef MEMORYPOOL_H
#define MEMORYPOOL_H

#include <vector>
#include <memory>
#include <functional>
#include <stdexcept>

template <typename T>
class MemoryPool
{
private:
    std::vector<std::unique_ptr<T>> pool;
    std::vector<T*> available;

    // Optional function to reset states on release
    std::function<void(T*)> resetFunction;
    std::function<void(T*)> initializeFunction;

public:
    MemoryPool(size_t initialSize = 100, 
               std::function<void(T*)> initFunc = nullptr,
               std::function<void(T*)> resetFunc = nullptr)
    {
        resetFunction = resetFunc;
        initializeFunction = initFunc;
        Expand(initialSize);
    }

    ~MemoryPool()
    {
        pool.clear();
        available.clear();
    }

    void Expand(size_t count)
    {
        for (size_t i = 0; i < count; ++i)
        {
            auto newItem = std::make_unique<T>();
            if (initializeFunction) {
                initializeFunction(newItem.get());
            }
            available.push_back(newItem.get());
            pool.push_back(std::move(newItem));
        }
    }

    T* Acquire()
    {
        if (available.empty())
        {
            // Auto expand if pool is depleted
            Expand(std::max<size_t>(10, pool.size() / 2)); 
        }

        T* item = available.back();
        available.pop_back();

        return item;
    }

    void Release(T* item)
    {
        if (!item) return;
        
        if (resetFunction) {
            resetFunction(item);
        }
        
        available.push_back(item);
    }

    size_t GetTotalSize() const { return pool.size(); }
    size_t GetAvailableCount() const { return available.size(); }
};

#endif // MEMORYPOOL_H
