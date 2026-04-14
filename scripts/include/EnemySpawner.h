#ifndef ENEMY_SPAWNER_H
#define ENEMY_SPAWNER_H

#include "../../core/include/ZanBehavior.h"

class ObjectManager;
class Player;

class EnemySpawner : public ZanBehavior {
private:
    ObjectManager* objectManager;
    Player* player;
    int frameCounter;
    int spawnInterval;

public:
    EnemySpawner(ObjectManager* manager, Player* p);
    void OnUpdate() override;
};

#endif // ENEMY_SPAWNER_H