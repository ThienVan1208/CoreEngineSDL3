#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>
#include "./core/include/Core.h"
#include "./core/include/UIManager.h"
#include "./scripts/include/player.h"
int main(int argc, char *argv[])
{
    if (!SDL_Init(SDL_INIT_VIDEO))
    {
        SDL_Log("SDL could not initialize! SDL_Error: %s", SDL_GetError());
        return 1;
    }

    SDL_Window *window = SDL_CreateWindow("SDL3 2026 Test", 1600, 900, 0);
    SDL_Renderer *renderer = SDL_CreateRenderer(window, nullptr);
    if (!window || !renderer)
    {
        SDL_Log("Window or renderer could not be created! SDL_Error: %s", SDL_GetError());
        return 1;
    }

    Core* core = new Core(renderer);
    UIText* textElement = new UIText(50, 50, 200, 50, SDL_Color{255, 255, 255, 255}, "Hello, SDL3!");
    UIButton* buttonElement = new UIButton(50, 150, 200, 50, SDL_Color{0, 255, 0, 255}, "Click Me!");
    Player* player = new Player(100, 100, 50, 50);
    bool running = true;
    while (running)
    {
        SDL_Event event;
        while (SDL_PollEvent(&event))
        {
            if (event.type == SDL_EVENT_QUIT)
                running = false;
        }

        // Update game logic
        core->OnUpdate();

        // Draw a red background FIRST
        SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
        SDL_RenderClear(renderer);

        // THEN render objects
        core->OnRender();

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