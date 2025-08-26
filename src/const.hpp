#pragma once
#include <SDL3/SDL_stdinc.h>
#include <string>

constexpr int SCREEN_WIDTH = 1600;
constexpr int SCREEN_HEIGHT = 1200;
constexpr int WORLD_WIDTH = 20000;
constexpr int WORLD_HEIGHT = 200000;
constexpr int TARGET_FPS = 120;
constexpr Uint64 TARGET_FRAME_TIME_NS = 1'000'000'000 / TARGET_FPS;
constexpr char program_name[] = "Vector blow";
constexpr char any_key[] = "Press any SPACE bars you may have to start";

#define WIDTH_RATIO 15
#define HEIGHT_RATIO 41.53846153846154
#define MIN_INPUT_DELAY_MOVEMENT 0
#define MIN_INPUT_DELAY_FIRE 400
#define NORMAL_PROJECTILE_SPEED 3
#define LEVEL_UI_RATIO 0.05f
#define BASE_PROJECTILE_DAMAGE 10
#define HEALTH_BAR_THICKNESS 1
#define OVERHEAT_PER_SHOT 7.5f
#define ANY_KEY_BLINK_DELAY 1000
#define POWERUP_LIFE_TIME 15000
#define DEFAULT_POWERUP_DURATION 15000
