#include <algorithm>
#include <cmath>
#include <filesystem>
#include <iostream>
#include <string>
#include <vector>

#include "SDLPlatform.hpp"
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include "Scene.hpp"
#include "Sprite.hpp"

bool SDLPlatform::Initialize() {
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        return false;
    }
    
    // Initialize SDL_image for PNG and JPG support
    int imgFlags = IMG_INIT_PNG | IMG_INIT_JPG;
    int imgResult = IMG_Init(imgFlags);
    if ((imgResult & imgFlags) != imgFlags) {
        return false;
    }
    SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "1");  // Makes the rendering look fancier (anti-aliasing)

    // Create a window and renderer
    window = SDL_CreateWindow("Scratch", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 480, 360, SDL_WINDOW_SHOWN);
    if (!window) {
        return false;
    }

    renderer = SDL_CreateRenderer(window, -1, 0);
    if (!renderer) {
        SDL_DestroyWindow(window);
        window = nullptr;
        SDL_Quit();
        return false;
    }

    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    return true;
}

void SDLPlatform::Shutdown() {
    if (renderer) {  // Stop renderer
        SDL_DestroyRenderer(renderer);
        renderer = nullptr;
    }

    if (window) {  // Stop window
        SDL_DestroyWindow(window);
        window = nullptr;
    }

    for (auto& [name, tex] : textures) {  // Free all textures
        SDL_DestroyTexture(tex);
    }
    textures.clear();

    SDL_Quit();
}

bool SDLPlatform::LoadAllAssets() {
    // Check if ./assets exists
    if (!std::filesystem::exists("assets") || !std::filesystem::is_directory("assets")) {
        return true;  // No assets directory found, but this isn't a fatal error; just means no assets to load
    }

    // Get a list of everything in ./assets
    std::vector<std::string> assetFiles;
    for (const auto& entry : std::filesystem::directory_iterator("assets")) {
        if (entry.is_regular_file()) {
            assetFiles.push_back(entry.path().string());
        }
    }

    // Image and sound formats accepted by Scratch; TODO: Verify which formats are actually accepted by Scratch
    const std::vector<std::string> IMAGE_FORMATS = { ".png", ".jpg", ".svg" };
    const std::vector<std::string> SOUND_FORMATS = { ".wav", ".mp3" };

    // Load each asset file into memory, sorting images into VRAM and sounds into RAM
    for (const auto& file : assetFiles) {
        if (std::find(IMAGE_FORMATS.begin(), IMAGE_FORMATS.end(), std::filesystem::path(file).extension().string()) != IMAGE_FORMATS.end()) {
            // Load image into VRAM
            SDL_Surface* surface = IMG_Load(file.c_str());
            if (!surface) return false;  // Failed to load image
            SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);
            SDL_FreeSurface(surface);
            if (!texture) return false;
            // Store it by filename
            std::string name = std::filesystem::path(file).stem().string(); // "cat" from "assets/cat.png"
            textures[name] = texture;
        }
    }

    return true;
}

void SDLPlatform::PollEvents() {
    SDL_Event event;

    while (SDL_PollEvent(&event)) {
        if (event.type == SDL_QUIT)
            should_close = true;
    }
}

bool SDLPlatform::ShouldClose() const {
    return should_close;
}

void SDLPlatform::BeginFrame() {
    frameStart = SDL_GetPerformanceCounter();
    SDL_RenderClear(renderer);
}

void SDLPlatform::RenderScene(const Scene& scene) {
    for (const auto& spritePtr : scene.sprites) {
        const Sprite& sprite = *spritePtr;
        for (const auto& clonePtr : sprite.clones) {
            const RenderedTarget& clone = *clonePtr;

            if (!clone.visible) continue;  // Skip invisible clones

            // Get the current costume for this clone
            if (clone.currentCostumeIndex >= clone.sprite->costumes.size()) continue;  // Invalid costume index
            const Costume& costume = clone.sprite->costumes[clone.currentCostumeIndex];

            // Find the texture for this costume
            std::string textureName = std::string(costume.assetId.begin(), costume.assetId.end());
            auto it = textures.find(textureName);
            if (it == textures.end()) continue;  // Texture not found
            SDL_Texture* texture = it->second;

            // Calculate the position and size to render the texture
            int texW, texH;
            SDL_QueryTexture(texture, NULL, NULL, &texW, &texH);

            // Adjust for bitmap resolution and clone size
            SDL_Rect destRect;
            u_int8_t bitmapResolution = costume.bitmapResolution;
            destRect.w = static_cast<int>(texW * (clone.size / 100.0) / bitmapResolution);
            destRect.h = static_cast<int>(texH * (clone.size / 100.0) / bitmapResolution);
            destRect.x = static_cast<int>(240 + clone.x - destRect.w / 2);  // Center of stage is (240, 180)
            destRect.y = static_cast<int>(180 - clone.y - destRect.h / 2);  // Invert y-axis

            // Handle angles and flipping
            double angle = 0.0;
            bool flipHorizontal = false;
            if (clone.rotationStyle == RotationStyle::ALL_AROUND) {
                angle = clone.direction - 90.0;  // Convert Scratch direction to SDL angle
            } else if (clone.rotationStyle == RotationStyle::LEFT_RIGHT) {
                angle = 0.0;
                flipHorizontal = (clone.direction < 0.0); // Scratch: negative = left
            }

            // Render the sprite
            SDL_RenderCopyEx(
                renderer,
                texture,
                NULL,
                &destRect,
                angle,
                NULL,
                flipHorizontal ? SDL_FLIP_HORIZONTAL : SDL_FLIP_NONE
            );
        }
    }
}

void SDLPlatform::EndFrame() {
    SDL_RenderPresent(renderer);

    // Check how long the frame took
    Uint64 frameEnd = SDL_GetPerformanceCounter();
    double frameTime = (double)(frameEnd - frameStart) / SDL_GetPerformanceFrequency();

    // If the frame took less time than the target, sleep for the remaining time
    if (frameTime < targetFrameTime) {
        double remaining = targetFrameTime - frameTime;
        SDL_Delay((Uint32)(remaining * 1000.0));
    }
}