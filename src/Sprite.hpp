#pragma once

#include <algorithm>
#include <cmath>
#include <cstdint>
#include <memory>
#include <string>
#include <unordered_map>
#include <vector>

#include "Asset.hpp"
#include "Globals.hpp"
#include "Variable.hpp"

class RenderedTarget;
extern bool g_redrawRequested;

struct Effects {
    float color      = 0.0f;
    float fisheye    = 0.0f;
    float whirl      = 0.0f;
    float pixelate   = 0.0f;
    float mosaic     = 0.0f;
    float brightness = 0.0f;
    float ghost      = 0.0f;
};

enum class RotationStyle : uint8_t {
    ALL_AROUND,
    LEFT_RIGHT,
    DONT_ROTATE
};

enum class VideoState : uint8_t {
    OFF,
    ON,
    ON_FLIPPED
};

// Sprite:
// A Sprite is the definition of a character: its assets and the list of live
// clones, it does NOT hold running state

class Sprite {
public:
    std::string name;  // In Scratch, it's saved as a UTF-16 string, but in project.json it's always UTF-8
    std::vector<Costume> costumes;
    std::vector<Sound>   sounds;

    // All live instances (original + clones) of this sprite
    // The first element [0] is always the original RenderedTarget
    std::vector<std::unique_ptr<RenderedTarget>> clones;
};

// Target:
// A Target is a live instance with its own variable state
// Blocks are absent, Clawmark does not interpret the block tree directly

class Target {
public:
    uint32_t id;
    std::unordered_map<std::string, Variable> variables;

    Variable* LookupVariableByNameAndType(const std::string& name, std::string type, bool skipStage = false);
    const Variable* LookupVariableByNameAndType(const std::string& name, std::string type, bool skipStage = false) const;
};

// RenderedTarget:
// Extends Target with all visual/audible state for a specific instance of a
// Sprite on stage. The stage itself is also a RenderedTarget (isStage = true)

class RenderedTarget : public Target {
public:
    Sprite* sprite = nullptr;  // Non-owning pointer back to parent Sprite

    Effects effects;
    uint32_t drawableId = 0;  // Render-engine texture ID for current costume

    bool isOriginal = true;  // False for clones
    bool isStage = false;

    // Some of this stuff differs from SDL, the rendering code automatically converts it to SDL's coordinate system and rotation system
    double x = 0.0;  // 0 is center of stage, +x is right, -x is left, unlike SDL where 0 is left edge of window
    double y = 0.0;  // 0 is center of stage, +y is up, -y is down, unlike SDL where 0 is top edge of window and +y is down
    double direction = 90.0;  // 90° is facing right, unlike SDL where 0° is facing right
    bool draggable = false;
    bool dragging = false;
    bool visible = true;
    double size = 100.0;  // Percentage

    uint32_t currentCostume = 0;
    RotationStyle rotationStyle = RotationStyle::ALL_AROUND;

    double volume = 100.0;
    double tempo = 60.0;

    // Extension fields, may be permanently stubbed
    double videoTransparency = 50.0;
    VideoState videoState = VideoState::ON;
    std::string textToSpeechLanguage;

    // Rendered target functions for blocks

    void SetXY(double x, double y, bool force = false);

    inline void SetDirection(double direction) {
        if (isStage || !std::isfinite(direction)) return;
        if (visible) g_redrawRequested = true;

        // Keep direction between -179 and +180 degrees
        direction = std::fmod(direction, 360.0);
        while (direction > 180.0) direction -= 360.0;
        while (direction <= -180.0) direction += 360.0;
        this->direction = direction;
    }

    inline void SetDraggable(bool draggable) {
        if (isStage) return;
        this->draggable = !!draggable;
    }

    inline void SetVisible(bool visible) {
        if (isStage) return;
        if (this->visible) g_redrawRequested = true;
        this->visible = !!visible;
    }

    inline void SetSize(double size) {
        if (isStage || !std::isfinite(size)) return;
        if (!sprite || sprite->costumes.empty()) return;

        const Costume& costume = sprite->costumes[currentCostume];

        if (costume.width <= 0 || costume.height <= 0 ||
            costume.bitmapResolution <= 0) {
            return;
        }

        const double origW = static_cast<double>(costume.width) / costume.bitmapResolution;
        const double origH = static_cast<double>(costume.height) / costume.bitmapResolution;

        if (!std::isfinite(origW) || !std::isfinite(origH) || origW <= 0.0 || origH <= 0.0) {
            return;
        }

        const double minScale = std::min(1.0, std::max(5.0 / origW, 5.0 / origH));
        const double maxScale = std::min(
            (1.5 * 480.0) / origW,
            (1.5 * 360.0) / origH
        );

        this->size = std::clamp(size / 100.0, minScale, maxScale) * 100.0;
        if (visible) g_redrawRequested = true;
    }

    inline void SetEffect(const std::string& effectName, double value) {
        // Stub
    }

    inline void ClearEffects() {
        if (visible) g_redrawRequested = true;
        this->effects = Effects();
    }

    inline void SetCostume(double index) {
        if (visible) g_redrawRequested = true;

        if (!sprite || sprite->costumes.empty()) return;

        if (!std::isfinite(index)) {
            currentCostume = 0;
            return;
        }

        const int numCostumes = static_cast<int>(sprite->costumes.size());
        const int roundedIndex = static_cast<int>(std::round(index));
        const int indexInt = ((roundedIndex % numCostumes) + numCostumes) % numCostumes;

        currentCostume = static_cast<uint32_t>(indexInt);
    }

private:
    std::pair<double, double> GetFencedPosition(double newX, double newY);
};