#include "Scene.hpp"

void Scene::AddSprite(std::unique_ptr<Sprite> sprite) {
    sprites.push_back(std::move(sprite));
}

void Scene::RegisterHat(HatType type, HatScript hat) {
    hatScripts[type].push_back(hat);
}

RenderedTarget* Scene::GetTargetForStage() {
    for (const auto& spritePtr : sprites) {
        if (spritePtr->clones.size() > 0 && spritePtr->clones[0]->isStage) {
            return spritePtr->clones[0].get();
        }
    }
    return nullptr;  // No stage found, which should never happen ever
}

RenderedTarget* Scene::GetSpriteTargetByName(const std::string& name) {
    for (const auto& spritePtr : sprites) {
        if (spritePtr->name == name && spritePtr->clones.size() > 0) {
            return spritePtr->clones[0].get();
        }
    }
    return nullptr;  // No sprite with that name found
}