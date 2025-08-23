#pragma once
#include <SDL3/SDL_stdinc.h>

constexpr int SCREEN_WIDTH = 1600;
constexpr int SCREEN_HEIGHT = 1200;
constexpr int WORLD_WIDTH = 20000;
constexpr int WORLD_HEIGHT = 200000;
constexpr int TARGET_FPS = 120;
constexpr Uint64 TARGET_FRAME_TIME_NS = 1'000'000'000 / TARGET_FPS;
