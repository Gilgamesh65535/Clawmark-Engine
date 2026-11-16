#pragma once

#include <vector>
#include <memory>
#include "Sprite.hpp"

class Scene {
public:
    std::vector<std::unique_ptr<Sprite>> sprites;

    void AddSprite(std::unique_ptr<Sprite> sprite);
};