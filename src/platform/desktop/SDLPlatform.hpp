#pragma once

#include <SDL2/SDL.h>
#include <unordered_map>
#include <string>

#include "../IPlatform.hpp"

class SDLPlatform : public IPlatform {
public:
    bool Initialize() override;
    void Shutdown() override;

    bool LoadAllAssets() override;

    void PollEvents() override;
    bool ShouldClose() const override;

    void BeginFrame() override;
    void RenderScene(const Scene& scene) override;
    void EndFrame() override;

private:
    bool should_close = false;
    SDL_Window* window = nullptr;
    SDL_Renderer* renderer = nullptr;
    Uint64 frameStart = 0;
    double targetFrameTime = 1.0 / 30.0;
    std::unordered_map<std::string, SDL_Texture*> textures;
};