#pragma once

class Scene;  // Forward declaration of Scene so IPlatform.hpp can include it without circular dependency

class IPlatform {
public:
    virtual ~IPlatform() = default;

    // Initializes the platform
    // For example, on Windows/Linux, this would initialize SDL2
    // On something like GameCube/Wii, this would initialize the GX graphics library and other necessary subsystems
    virtual bool Initialize() = 0;
    virtual void Shutdown() = 0;

    // TODO: Dynamic loading/unloading would be useful for platforms with less RAM, maybe make a LoadAsset() and UnloadAsset() function later
    virtual bool LoadAllAssets() = 0; // Load all assets (images, sounds, etc.) into memory

    // Polls for events (like input, window close, etc.)
    virtual void PollEvents() = 0;

    // Check whether the application should continue running
    virtual bool ShouldClose() const = 0;

    // Rendering stuff
    virtual void BeginFrame() = 0;
    virtual void RenderScene(const Scene& scene) = 0;
    virtual void EndFrame() = 0;
};