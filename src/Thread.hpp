#pragma once

#include <cstdint>
#include <functional>
#include <vector>

#include "HatType.hpp"

class RenderedTarget;

enum class ThreadStatus : uint8_t {
    RUNNING,
    YIELD,  // Yield until next frame
    YIELD_TICK,  // Yield for exactly one tick
    DONE,
};

struct LoopFrame {
    int loopState;  // Which loop we're inside of (so nested loops work)
    int iterations;  // How many left (-1 = infinite/while)
};

// Scratch threads are weird cause they're not like the traditional definition of a thread in a normal programming language,
// it's just a collection of blocks executed in order until they yield or finish
class Thread {
public:
    ThreadStatus status = ThreadStatus::RUNNING;
    HatType hatType;
    int state = 0;  // Which step of the script we're on
    bool isKilled = false;

    RenderedTarget* target = nullptr;  // Which sprite instance owns this thread

    double waitTimer = 0.0; // Used by "wait X seconds"

    std::vector<LoopFrame> loopStack;  // Used by loops to keep track of where to jump back to when the loop continues

    // The actual script function that will be executed
    std::function<ThreadStatus(Thread&)> script;
};