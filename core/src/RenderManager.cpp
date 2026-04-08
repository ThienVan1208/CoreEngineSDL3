#include "../include/RenderManager.h"
#include <algorithm>

std::function<void(IRenderable*)> RenderManager::OnRegisterRenderable;
std::function<void(IRenderable*)> RenderManager::OnUnregisterRenderable;

void RenderManager::Init()
{
    OnRegisterRenderable = [this](IRenderable* renderable)
    {
        RegisterRenderable(renderable);
    };

    OnUnregisterRenderable = [this](IRenderable* renderable)
    {
        UnregisterRenderable(renderable);
    };
}

void RenderManager::RegisterRenderable(IRenderable* renderable)
{
    renderables.push_back(renderable);
}

void RenderManager::UnregisterRenderable(IRenderable* renderable)
{
    renderables.erase(std::remove(renderables.begin(), renderables.end(), renderable), renderables.end());
}

void RenderManager::RenderAll(SDL_Renderer* renderer)
{
    
    std::stable_sort(renderables.begin(), renderables.end(), [this](IRenderable* a, IRenderable* b) {
        if (a->GetRenderLayer() != b->GetRenderLayer()) {
            return a->GetRenderLayer() < b->GetRenderLayer(); // Sort by Layer first
        }
        if (isYSortEnabled) {
            return a->GetYSortValue() < b->GetYSortValue(); // Sort by Y-axis depth if on the same layer
        }
        return false;
    });

    for (auto& renderable : renderables) {
        if (renderable != nullptr && renderable->IsVisible()) {
            renderable->Render(renderer);
        }
    }
}
