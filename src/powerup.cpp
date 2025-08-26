#include "misc.hpp"
#include "types.hpp"
#include <SDL3/SDL_rect.h>
#include <SDL3/SDL_render.h>
#include <SDL3/SDL_timer.h>
constexpr char *powerup_filenames[] = {[TWO_X] = "assets/powerups/2x.svg",
                                       [BOOM] = "assets/powerups/boom.svg",
                                       [THREE_X] = "assets/powerups/3x.svg",
                                       [HEALTH] = "assets/powerups/health.svg",
                                       [BEAM] = "assets/powerups/beam.svg",
                                       [FIVE_X] = "assets/powerups/5x.svg"};

powerup_type summon_powerup(SDL_FPoint pos, powerup_efect_type type) {
  powerup_type powerup;
  powerup.texture = texture_from_SVG_file(powerup_filenames[type], 1);
  powerup.type = type;
  powerup.rect = {pos.x, pos.y, static_cast<float>(powerup.texture->w),
                  static_cast<float>(powerup.texture->h)};
  powerup.spawn_tick = SDL_GetTicks();
  return powerup;
}