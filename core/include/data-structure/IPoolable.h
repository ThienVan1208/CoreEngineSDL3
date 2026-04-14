#ifndef IPOOLABLE_H
#define IPOOLABLE_H

#include <functional>

class IPoolable
{
public:
    std::function<void()> returnToPool;

    virtual ~IPoolable() = default;
    virtual void OnGetFromPool() {}
    virtual void OnReturnToPool() {}
    
    virtual void ReturnToPool() 
    {
        if (returnToPool)
            returnToPool();
        
            
    }
};

#endif // IPOOLABLE_H