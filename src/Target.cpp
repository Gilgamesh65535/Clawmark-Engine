#include "Globals.hpp"
#include "Scene.hpp"
#include "Sprite.hpp"

Variable* Target::LookupVariableByNameAndType(const std::string& name, std::string type, bool skipStage) {
    for (auto& [varId, var] : variables) {
        if (var.name == name && var.type == type) {
            return &var;
        }
    }

    // In Scratch this also does && if !isStage, but we can't do that here so hopefully it works right
    if (!skipStage && g_scene) {
        RenderedTarget* stageTarget = g_scene->GetTargetForStage();
        if (stageTarget) {
            for (auto& [varId, var] : stageTarget->variables) {
                if (var.name == name && var.type == type) {
                    return &var;
                }
            }
        }
    }

    return nullptr;
}

const Variable* Target::LookupVariableByNameAndType(const std::string& name, std::string type, bool skipStage) const {
    for (const auto& [varId, var] : variables) {
        if (var.name == name && var.type == type) {
            return &var;
        }
    }

    // In Scratch this also does && if !isStage, but we can't do that here so hopefully it works right
    if (!skipStage && g_scene) {
        const RenderedTarget* stageTarget = g_scene->GetTargetForStage();
        if (stageTarget) {
            for (const auto& [varId, var] : stageTarget->variables) {
                if (var.name == name && var.type == type) {
                    return &var;
                }
            }
        }
    }

    return nullptr;
}