#include "const.hpp"
#include "main.hpp"
#include "meth.hpp"
#include "misc.hpp"
#include "projectiles.hpp"
#include "types.hpp"
#include <SDL3/SDL_render.h>
#include <vector>

enemy_type spawn_enemy(enemy_ai_type ai_type, Uint32 reload_time) {
  enemy_type enemy;
  enemy.ship.texture = texture_from_SVG_file("assets/enemy_1.svg", 1);
  enemy.type = ai_type;
  enemy.reload_time = reload_time;

  enemy.ship.rect = {
      static_cast<float>(mode->w),
      static_cast<float>(get_random_num(0, mode->h - enemy.ship.texture->h)),
      static_cast<float>(enemy.ship.texture->w),
      static_cast<float>(enemy.ship.texture->h)};

  enemy.ship.gun_offset.x = 0;
  enemy.ship.gun_offset.y = 0.5f * enemy.ship.texture->h;
  enemy.size_multiplier = 1;
  return enemy;
}

void step_enemy(enemy_type &e, ship_type &player,
                std::vector<projectile> &projectiles) {
  switch (e.type) {
  case RANDOM:
    switch (get_random_num(0, 3)) {
    case 0:
      e.ship.rect.x++;
      break;
    case 1:
      e.ship.rect.x -= 2;
      break;
    case 2:
      e.ship.rect.y++;
      break;
    case 3:
      e.ship.rect.y--;
      break;
    }
    if (get_random_num(0, 1000) == 0 &&
        SDL_GetTicks() - e.last_shot > e.reload_time) {
      projectiles.push_back(
          spawn_projectile({e.ship.rect.x + e.ship.gun_offset.x,
                            e.ship.rect.y + e.ship.gun_offset.y},
                           e.size_multiplier, 270, NORMAL_PROJECTILE_SPEED,
                           "assets/basic_projectile.svg", nullptr));
    }
    break;
  case FLYER:
    if (e.ship.rect.x != e.target.x && e.ship.rect.y != e.target.y) {
      if (e.ship.rect.y < mode->h / 2) {
        e.target.y = 0;
      } else {
        e.target.y = mode->h - e.ship.rect.h;
      }
      e.target.x = e.ship.rect.x - (e.ship.rect.w / 2);
    }
    break;
  case GUNNER:
    break;
  case ACE:
    break;
  case BOSS:
    break;
  }
  SDL_RenderTexture(main_sdl_session.renderer, e.ship.texture, nullptr,
                    &e.ship.rect);
}