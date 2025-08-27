#include "const.hpp"
#include "main.hpp"
#include "meth.hpp"
#include "misc.hpp"
#include "powerups.hpp"
#include "projectiles.hpp"
#include "types.hpp"
#include <SDL3/SDL_rect.h>
#include <SDL3/SDL_render.h>
#include <SDL3/SDL_timer.h>
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
  if (ai_type == BOSS) {
    static int times_appeared = 1;
    enemy.ship.health = times_appeared * BOSS_HEALTH;
    times_appeared++;
  } else {
    enemy.ship.health = multiplier * BASE_PROJECTILE_DAMAGE;
  }

  if (ai_type == FLYER) {
    enemy.target = {(level_screen_limit.x + level_screen_limit.w) -
                        enemy.ship.rect.w,
                    static_cast<float>(level_screen_limit.y)};
    enemy.reload_time *= 2;
  }

  return enemy;
}

void step_enemy(enemy_type &e, ship_type &player_ship,
                std::vector<projectile> &projectiles,
                std::vector<powerup_type> &powerups) {
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

    // chaeck if target is reached
    if (std::abs(e.ship.rect.y - e.target.y) <= tolerance) {

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
    e.ship.rect.y =
        (player_ship.rect.y + (player_ship.rect.h / 2)) - e.ship.rect.h / 2;
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
  case ACE: {
    if ((player_ship.rect.y + player_ship.rect.h >
         e.ship.rect.y + e.ship.gun_offset.y) &&
        (e.ship.rect.y + e.ship.gun_offset.y > player_ship.rect.y)) {
      if (SDL_GetTicks() - e.last_shot > e.reload_time) {
        projectiles.push_back(
            spawn_projectile({e.ship.rect.x + e.ship.gun_offset.x,
                              e.ship.rect.y + e.ship.gun_offset.y},
                             e.size_multiplier, 270, NORMAL_PROJECTILE_SPEED,
                             "assets/basic_projectile.svg", nullptr, FOE,
                             BASE_PROJECTILE_DAMAGE * e.size_multiplier));
        e.last_shot = SDL_GetTicks();
      }
    }
    SDL_FRect dont_be_in_rect = {0, e.ship.rect.y, static_cast<float>(mode->w),
                                 0};
    {
      SDL_FRect e_rect_dup = e.ship.rect;
      e_rect_dup.x = 0;
      for (projectile &p : projectiles) {
        if (p.type == ALLY) {
          SDL_FRect p_rect_dup = p.rect;
          p_rect_dup.x = 0;
          if (SDL_HasRectIntersectionFloat(&e_rect_dup, &p_rect_dup)) {
            if (p_rect_dup.y < dont_be_in_rect.y) {
              dont_be_in_rect.y = p_rect_dup.y;
            }
            if (p_rect_dup.y + p_rect_dup.h >
                dont_be_in_rect.y + dont_be_in_rect.h) {
              dont_be_in_rect.h = p_rect_dup.y + p_rect_dup.h -
                                  dont_be_in_rect.y + dont_be_in_rect.h;
            }
          }
        }
      }
    }

    if (dont_be_in_rect.h != 0 &&
        SDL_HasRectIntersectionFloat(&e.ship.rect, &dont_be_in_rect)) {
      float up_y = (e.ship.rect.y - (dont_be_in_rect.y + dont_be_in_rect.h -
                                     e.ship.rect.y + e.ship.rect.h)) -
                   1;
      float down_y = (dont_be_in_rect.y + dont_be_in_rect.h) + 1;

      if (std::abs(e.ship.rect.y - up_y) < std::abs(down_y - e.ship.rect.y)) {
        if (up_y < level_screen_limit.y) {
          goto go_down;
        }
      go_up:
        e.ship.rect.y--;
      } else {
        if (down_y + e.ship.rect.h >
            level_screen_limit.y + level_screen_limit.h) {
          goto go_up;
        }
      go_down:
        e.ship.rect.y++;
      }
    } else {

      e.ship.rect.y < (player_ship.rect.y + (player_ship.rect.h / 2)) -
                          e.ship.rect.h / 2
          ? e.ship.rect.y++
          : e.ship.rect.y--;
    }
    e.ship.rect.x--;

  } break;
  case BOSS: {
    if (e.ship.rect.x <=
        static_cast<float>(mode->w) *
            static_cast<float>(static_cast<float>(7) / static_cast<float>(8))) {
      if (SDL_GetTicks() - e.last_shot > e.reload_time) {
        int hole_start_index =
            get_random_num(0, (level_screen_limit.h / 20) - 5);
        int holes = 5;
        int j = 0;
        for (int i = level_screen_limit.y;
             i < level_screen_limit.y + level_screen_limit.h; i += 20) {
          if (!(j >= hole_start_index && holes != 0)) {
            projectiles.push_back(spawn_projectile(
                {static_cast<float>(static_cast<float>(mode->w) *
                                    static_cast<float>(static_cast<float>(7) /
                                                       static_cast<float>(8))),
                 static_cast<float>(i)},
                4, 270, 2, "assets/basic_projectile.svg", nullptr, FOE,
                BASE_PROJECTILE_DAMAGE * 2.5f));
          } else {
            holes--;
          }
          j++;
        }
        e.last_shot = SDL_GetTicks();
      }
      if (get_random_num(0, 3500) == 0) {
        powerups.push_back(summon_powerup(
            {static_cast<float>(
                 get_random_num(level_screen_limit.x,
                                level_screen_limit.x + level_screen_limit.w)),
             static_cast<float>(
                 get_random_num(level_screen_limit.y,
                                level_screen_limit.y + level_screen_limit.h))},
            static_cast<powerup_efect_type>(get_random_num(
                powerup_efect_type::BOOM, powerup_efect_type::BEAM))));
      }
    } else {
      e.ship.rect.x -= 0.5f;
    }
  } break;
  }
  SDL_RenderTexture(main_sdl_session.renderer, e.ship.texture, nullptr,
                    &e.ship.rect);
}