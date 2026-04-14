#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>
#include "./core/include/Core.h"
#include "./core/include/UIElements.h"
#include "./core/include/ObjectManager.h"
#include "./scripts/include/player.h"
#include "./scripts/include/WeaponBase.h"
#include "./scripts/include/wall.h"
#include "./scripts/include/ground.h"
#include "./scripts/include/EnemySpawner.h"
#include "./core/include/Screen.h"
#include "./core/debug/CoordinateDebug.h"
#include "./core/debug/FPSDebug.h"
#include "./core/include/ZanBehavior.h"
#include "./core/include/components/SpriteRenderer.h"
#include "./core/include/setting/Setting.h"
#include "./core/include/Time.h"
#include <memory>


const float SCREEN_WIDTH = 1600.0f;
const float SCREEN_HEIGHT = 900.0f;
const SDL_Color GROUND_COLOR = {0, 128, 0, 255};
int main(int argc, char *argv[])
{
    if (!SDL_Init(SDL_INIT_VIDEO))
    {
        SDL_Log("SDL could not initialize! SDL_Error: %s", SDL_GetError());
        return 1;
    }

    SDL_Window *window = SDL_CreateWindow("SDL3 2026 Test", SCREEN_WIDTH, SCREEN_HEIGHT, 0);
    SDL_Renderer *renderer = SDL_CreateRenderer(window, nullptr);
    if (!window || !renderer)
    {
        SDL_Log("Window or renderer could not be created! SDL_Error: %s", SDL_GetError());
        return 1;
    }
    std::unique_ptr<Screen> screen = std::make_unique<Screen>(SCREEN_WIDTH, SCREEN_HEIGHT);
    std::unique_ptr<Core> core = std::make_unique<Core>(renderer);
    // Configure FPS / Time
    FPSDebug::Init();
    Time::Init();

    auto buttonElement = std::make_unique<UIButton>(50, 150, "Click Me!");
    
    // Player spawns just above the ground (ground top = -435 + 15 = -420)
    GameObject *playerObject = core->objectManager->CreateObject<GameObject>(0, 0);
    auto playerComponent = std::make_unique<Player>();
    Player *player = playerComponent.get();
    playerObject->AttachComponent(std::move(playerComponent));

    // Spawn Weapon
    GameObject *weaponObject = core->objectManager->CreateObject<GameObject>(0, 0);
    auto weaponComponent = std::make_unique<WeaponBase>();
    WeaponBase *weapon = weaponComponent.get();
    weaponObject->AttachComponent(std::move(weaponComponent));

    // Give Weapon to Player
    player->SetupWeapon(weapon);

    // Setup Top-Down Arena Boundaries
    float wallTopY = (SCREEN_HEIGHT / 2.0f) / Setting::BASE_UNIT - 1.0f;
    GameObject *wallTop = core->objectManager->CreateObject<GameObject>(0, wallTopY);
    wallTop->transform->SetScale(SCREEN_WIDTH / 100.0f, 1.0f); // Default sprite is ~100x100
    wallTop->AttachComponent(std::make_unique<Wall>());

    float wallBottomY = -(SCREEN_HEIGHT / 2.0f) / Setting::BASE_UNIT + 1.0f;
    GameObject *wallBottom = core->objectManager->CreateObject<GameObject>(0, wallBottomY);
    wallBottom->transform->SetScale(SCREEN_WIDTH / 100.0f, 1.0f);
    wallBottom->AttachComponent(std::make_unique<Wall>());

    float wallLeftX = -(SCREEN_WIDTH / 2.0f) / Setting::BASE_UNIT + 1.0f;
    GameObject *wallLeft = core->objectManager->CreateObject<GameObject>(wallLeftX, 0);
    wallLeft->transform->SetScale(1.0f, SCREEN_HEIGHT / 100.0f);
    wallLeft->AttachComponent(std::make_unique<Wall>());

    float wallRightX = (SCREEN_WIDTH / 2.0f) / Setting::BASE_UNIT - 1.0f;
    GameObject *wallRight = core->objectManager->CreateObject<GameObject>(wallRightX, 0);
    wallRight->transform->SetScale(1.0f, SCREEN_HEIGHT / 100.0f);
    wallRight->AttachComponent(std::make_unique<Wall>());
    
    // Create EnemySpawner Object
    GameObject *spawnerObject = core->objectManager->CreateObject<GameObject>(0, 0);
    // spawnerObject->AttachComponent(std::make_unique<EnemySpawner>(core->objectManager.get(), player));

    bool running = true;
    while (running)
    {
        Time::Update();
        FPSDebug::Update();

        SDL_Event event;
        while (SDL_PollEvent(&event))
        {
            if (event.type == SDL_EVENT_QUIT)
                running = false;
        }

        // Update game logic
        core->OnUpdate();

        // Draw a red background FIRST
        SDL_SetRenderDrawColor(renderer, GROUND_COLOR.r, GROUND_COLOR.g, GROUND_COLOR.b, GROUND_COLOR.a);
        SDL_RenderClear(renderer);

        // THEN render objects
        core->OnRender();

        // Debug: Draw coordinate grids
        CoordinateDebug::DrawCoordinateGrids(renderer);

        // 3. Present the finished frame to the user
        SDL_RenderPresent(renderer);
    }

    // delete core;
    // delete textElement;
    // delete buttonElement;
    // delete player;

    SDL_DestroyRenderer(renderer);

    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}