#pragma once

#include <functional>
#include <memory>
#include <unordered_map>
#include <vector>

#include "HatType.hpp"
#include "Sprite.hpp"
#include "Thread.hpp"

struct HatScript {
    std::function<ThreadStatus(Thread&)> script;
    RenderedTarget* target;
};

class Scene {
public:
    std::vector<std::unique_ptr<Sprite>> sprites;
    std::unordered_map<HatType, std::vector<HatScript>> hatScripts;

    void AddSprite(std::unique_ptr<Sprite> sprite);
    void RegisterHat(HatType type, HatScript hat);

    // Functions used by blocks
    RenderedTarget* GetTargetForStage();
    RenderedTarget* GetSpriteTargetByName(const std::string& name);
};