#include "Sequencer.hpp"

void Sequencer::StepAllThreads(double dt) {
    // Step through each thread in the sequencer
    for (auto& thread : threads) {
        if (thread->isKilled) {
            continue;  // We'll get rid of this thread after the loop, but for now just skip it
        } else if (thread->status == ThreadStatus::DONE) {
            continue;  // Skip finished threads
        } else if (thread->status == ThreadStatus::YIELD_TICK) {
            thread->status = ThreadStatus::RUNNING;  // Reset to RUNNING for the next tick
            continue;
        } else if (thread->status == ThreadStatus::YIELD || thread->status == ThreadStatus::RUNNING) {
            if (thread->waitTimer > 0.0) {  // If there is a wait block, decrement timer by delta time
                thread->waitTimer -= dt;
                if (thread->waitTimer <= 0.0) {
                    thread->waitTimer = 0.0;  // Reset to zero
                    thread->status = ThreadStatus::RUNNING;  // Resume the thread
                } else {
                    continue;  // Still waiting, skip stepping this thread
                }
            }
            StepThread(*thread);  // Where the magic happens
        }
    }
    // Remove threads marked as killed or done
    for (int i = 0; i < threads.size(); ) {
        if (threads[i]->isKilled || threads[i]->status == ThreadStatus::DONE) {
            threads.erase(threads.begin() + i);
            // Don't increment cause we're removing from a list being iterated over
        } else {
            i++;
        }
    }
}

inline void Sequencer::StepThread(Thread& thread) {
    thread.status = thread.script(thread);  // Execute the script function and update the status
}

void Sequencer::StartHats(HatType type, Scene& scene) {
    auto it = scene.hatScripts.find(type);
    if (it != scene.hatScripts.end()) {
        // If the hat type restarts threads, kill all existing threads of that type
        if (HatRestartsThreads(type)) {
            for (auto& thread : threads) {
                if (thread->hatType == type) {
                    thread->isKilled = true;
                }
            }
        }
        for (const auto& hatScript : it->second) {
            auto newThread = std::make_unique<Thread>();
            newThread->hatType = type;
            newThread->target = hatScript.target;
            newThread->script = hatScript.script;
            threads.push_back(std::move(newThread));
        }
    }
}