#include "Scene.hpp"

void Scene::AddSprite(std::unique_ptr<Sprite> sprite) {
    sprites.push_back(std::move(sprite));
}