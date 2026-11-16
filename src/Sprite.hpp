#pragma once

#include <cstdint>
#include <memory>
#include <string>
#include <unordered_map>
#include <vector>

#include "Asset.hpp"
#include "Variable.hpp"  // Brings in Value

class RenderedTarget;  // Forward declaration of RenderedTarget so Target can inherit from it without circular dependency

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
    std::unordered_map<std::string, Value> variables;
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

    uint32_t currentCostumeIndex = 0;
    RotationStyle rotationStyle = RotationStyle::ALL_AROUND;

    double volume = 100.0;
    double tempo = 60.0;

    // Extension fields, may be permanently stubbed
    double videoTransparency = 50.0;
    VideoState videoState = VideoState::ON;
    std::string textToSpeechLanguage;
};