#include "const.hpp"
#include "main.hpp"
#include "meth.hpp"
#include "misc.hpp"
#include "projectiles.hpp"
#include "types.hpp"
#include <SDL3/SDL_rect.h>
#include <SDL3/SDL_render.h>
#include <vector>

enemy_type spawn_enemy(enemy_ai_type ai_type, Uint32 reload_time,
                       float multiplier) {
  enemy_type enemy;
  enemy.ship.texture = texture_from_SVG_file("assets/enemy_1.svg", multiplier);
  enemy.type = ai_type;
  enemy.reload_time = reload_time;

  enemy.ship.rect = {
      static_cast<float>(mode->w),
      static_cast<float>(get_random_num(
          level_screen_limit.y, (level_screen_limit.y + level_screen_limit.h) -
                                    enemy.ship.texture->h)),
      static_cast<float>(enemy.ship.texture->w),
      static_cast<float>(enemy.ship.texture->h)};

  enemy.ship.gun_offset.x = 0;
  enemy.ship.gun_offset.y = 0.5f * enemy.ship.texture->h;
  enemy.size_multiplier = multiplier;
  enemy.ship.health = multiplier * BASE_PROJECTILE_DAMAGE;

  if (ai_type == FLYER) {
    enemy.target = {(level_screen_limit.x + level_screen_limit.w) -
                        enemy.ship.rect.w,
                    static_cast<float>(level_screen_limit.y)};
    enemy.reload_time *= 2;
  }

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
      if (e.ship.rect.y + e.ship.rect.h <
          (level_screen_limit.h + level_screen_limit.y)) {
        e.ship.rect.y++;
      }
      break;
    case 3:
      if (e.ship.rect.y > level_screen_limit.y) {
        e.ship.rect.y--;
      }
      break;
    }
    if (get_random_num(0, 1000) == 0 &&
        SDL_GetTicks() - e.last_shot > e.reload_time) {
      projectiles.push_back(
          spawn_projectile({e.ship.rect.x + e.ship.gun_offset.x,
                            e.ship.rect.y + e.ship.gun_offset.y},
                           e.size_multiplier, 270, NORMAL_PROJECTILE_SPEED,
                           "assets/basic_projectile.svg", nullptr, FOE,
                           BASE_PROJECTILE_DAMAGE * e.size_multiplier));
    }
    break;
  case FLYER: {

    float frames_to_complete_cycle =
        (level_screen_limit.y + level_screen_limit.h) - level_screen_limit.y -
        e.ship.rect.h / 5.0f;
    float horizontal_speed = e.ship.rect.w / frames_to_complete_cycle;

    e.ship.rect.x -= horizontal_speed;

    e.target.x -= horizontal_speed;
    float tolerance = 5.0f;
    bool reached_target = std::abs(e.ship.rect.y - e.target.y) <= tolerance;

    if (reached_target) {

      if (e.target.y <= level_screen_limit.y + tolerance) {

        e.target.y =
            (level_screen_limit.y + level_screen_limit.h) - e.ship.rect.h;
      } else {
        e.target.y = level_screen_limit.y;
      }
    }

    if (e.ship.rect.y < e.target.y) {
      e.ship.rect.y += 3;
    } else if (e.ship.rect.y > e.target.y) {
      e.ship.rect.y -= 3;
    }

    if (SDL_GetTicks() - e.last_shot > e.reload_time) {
      projectiles.push_back(
          spawn_projectile({e.ship.rect.x + e.ship.gun_offset.x,
                            e.ship.rect.y + e.ship.gun_offset.y},
                           e.size_multiplier, 270, NORMAL_PROJECTILE_SPEED,
                           "assets/basic_projectile.svg", nullptr, FOE,
                           BASE_PROJECTILE_DAMAGE * e.size_multiplier));
      e.last_shot = SDL_GetTicks();
    }
  } break;

  case GUNNER:
    e.ship.rect.y = (player.rect.y + (player.rect.h / 2)) - e.ship.rect.h / 2;
    e.ship.rect.x--;

    if (get_random_num(0, 250) == 0 &&
        SDL_GetTicks() - e.last_shot > e.reload_time) {
      projectiles.push_back(
          spawn_projectile({e.ship.rect.x + e.ship.gun_offset.x,
                            e.ship.rect.y + e.ship.gun_offset.y},
                           e.size_multiplier, 270, NORMAL_PROJECTILE_SPEED,
                           "assets/basic_projectile.svg", nullptr, FOE,
                           BASE_PROJECTILE_DAMAGE * e.size_multiplier));
    }

    break;
  case ACE:
    break;
  case BOSS:
    break;
  }
  SDL_RenderTexture(main_sdl_session.renderer, e.ship.texture, nullptr,
                    &e.ship.rect);
}