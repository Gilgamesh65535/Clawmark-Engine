#pragma once

#include <cstdint>
#include <functional>

enum class HatType {
    WHEN_FLAG_CLICKED,
    WHEN_KEY_PRESSED,
    WHEN_THIS_SPRITE_CLICKED,
    WHEN_TOUCHING_OBJECT,
    WHEN_STAGE_CLICKED,
    WHEN_BACKDROP_SWITCHES_TO,
    WHEN_GREATER_THAN,
    WHEN_BROADCAST_RECEIVED
};

inline bool HatRestartsThreads(HatType type) {
    switch (type) {
        case HatType::WHEN_FLAG_CLICKED: return true;
        case HatType::WHEN_KEY_PRESSED: return false;
        case HatType::WHEN_THIS_SPRITE_CLICKED: return true;
        case HatType::WHEN_TOUCHING_OBJECT: return false;
        case HatType::WHEN_STAGE_CLICKED: return true;
        case HatType::WHEN_BACKDROP_SWITCHES_TO: return true;
        case HatType::WHEN_GREATER_THAN: return false;
        case HatType::WHEN_BROADCAST_RECEIVED: return true;
        default: return false;
    }
}

namespace std {
    template<> struct hash<HatType> {
        size_t operator()(HatType h) const {
            return static_cast<size_t>(h);
        }
    };
}