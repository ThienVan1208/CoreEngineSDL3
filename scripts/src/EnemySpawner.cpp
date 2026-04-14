#include "../include/EnemySpawner.h"
#include "../include/Enemy.h"
#include "../../core/include/ObjectManager.h"
#include "../../core/include/components/Transform.h"
#include <cstdlib>

EnemySpawner::EnemySpawner(ObjectManager* manager, Player* p)
    : objectManager(manager), player(p), frameCounter(0), spawnInterval(30) {} // Spawn every 30 frames (0.5 sec at 60fps)

void EnemySpawner::OnUpdate() {
    if (!objectManager || !player) return;

    frameCounter++;
    if (frameCounter >= spawnInterval) {
        frameCounter = 0;
        
        // Random position away from center
        float x = (std::rand() % 32) - 16.0f;
        float y = (std::rand() % 18) - 9.0f;

        // Push outwards if too close to center
        if (std::abs(x) < 8.0f && std::abs(y) < 4.0f) {
            x += (x > 0 ? 8.0f : -8.0f); 
            y += (y > 0 ? 6.0f : -6.0f);
        }

        GameObject* newObj = objectManager->CreateObject<GameObject>(x, y);
        newObj->AttachComponent(std::make_unique<Enemy>(player));
        
        // Slightly decrease spawn interval to make it harder over time
        if (spawnInterval > 2 && (std::rand() % 10 == 0)) {
            spawnInterval--; 
        }
    }
}