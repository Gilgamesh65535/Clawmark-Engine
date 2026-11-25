#pragma once

#include <cstdint>
#include <string>

class Costume {
public:
    std::string assetId;
    std::string name;
    uint8_t bitmapResolution = 1;
    std::string dataFormat;
    std::string filename;
    uint32_t rotationCenterX = 0;
    uint32_t rotationCenterY = 0;

    // These aren't stored in Scratch, but they're needed for the renderer, and so we save the costume width and height when loading the costume
    uint32_t width = 0;
    uint32_t height = 0;

    Costume() = default;

    // Constructor for Costume
    Costume(const std::string& name,
            uint8_t bitmapResolution,
            const std::string& dataFormat,
            const std::string& assetId,
            double rotationCenterX = 0,
            double rotationCenterY = 0)
        : assetId(assetId),
          name(name),
          bitmapResolution(bitmapResolution),
          dataFormat(dataFormat),
          rotationCenterX(rotationCenterX),
          rotationCenterY(rotationCenterY)
    {}
};

class Sound {
public:
    std::string name;
    std::string dataFormat;  // "wav", "mp3", etc.
    std::string assetId;  // 32-char hex string
    std::string filename;  // 32-char hex string + ".wav", ".mp3", etc. ("md5ext" in Scratch's code)
    uint32_t rate;  // Sample rate in Hz
    uint32_t sampleCount;  // Number of samples in the sound
};