#include "misc.hpp"
#include "types.hpp"
#include <SDL3/SDL_rect.h>
#include <SDL3/SDL_render.h>
#include <SDL3/SDL_timer.h>
constexpr char *powerup_filenames[5] = {
    "assets/powerups/2x.svg", "assets/powerups/boom.svg",
    "assets/powerups/3x.svg", "assets/powerups/beam.svg",
    "assets/powerups/5x.svg"};

powerup_type summon_powerup(SDL_FPoint pos, powerup_efect_type type) {
  powerup_type powerup;
  powerup.texture = texture_from_SVG_file(powerup_filenames[type], 1);
  powerup.type = type;
  powerup.rect = {pos.x, pos.y, static_cast<float>(powerup.texture->w),
                  static_cast<float>(powerup.texture->h)};
  powerup.spawn_tick = SDL_GetTicks();
  return powerup;
}