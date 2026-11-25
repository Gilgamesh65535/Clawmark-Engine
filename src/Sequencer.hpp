#pragma once

#include <memory>
#include <vector>

#include "HatType.hpp"
#include "Scene.hpp"
#include "Thread.hpp"

class Scene;  // Forward declaration of Sequencer so Scene can have a pointer to it without circular dependency

static constexpr double WORK_TIME = 0.75 * (1.0 / 30.0);  // Unused currently, will be used when multiple passes are implemented

class Sequencer {
public:
    // In Scratch's sequencer.js, these functions are called "stepThreads" and "stepThread",
    // but that one character difference makes it so confusing so this is way better
    void StepAllThreads(double dt);
    void StepThread(Thread& thread);

    void StartHats(HatType type, Scene& scene);  // Starts all threads for a given hat type

    std::vector<std::unique_ptr<Thread>> threads;
};