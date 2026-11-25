#include "Globals.hpp"

#include "Scene.hpp"
#include "Sequencer.hpp"

double g_timer = 0.0;
bool g_redrawRequested = false;

Scene* g_scene = nullptr;
Sequencer* g_sequencer = nullptr;
IPlatform* g_platform = nullptr;