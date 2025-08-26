#include "const.hpp"
#include "enemies.hpp"
#include "macro.hpp"
#include "main.hpp"
#include "meth.hpp"
#include "powerups.hpp"
#include "projectiles.hpp"
#include "text.hpp"
#include "types.hpp"
#include <SDL3/SDL_oldnames.h>
#include <SDL3/SDL_pixels.h>
#include <SDL3/SDL_rect.h>
#include <SDL3/SDL_render.h>
#include <SDL3/SDL_scancode.h>
#include <SDL3/SDL_stdinc.h>
#include <SDL3/SDL_time.h>
#include <SDL3/SDL_timer.h>
#include <array>
#include <cstring>
#include <string>
#include <tuple>
#include <vector>

constexpr std::array<SDL_FPoint, 5> overheat_block = {
    {{0.5f, 0.0f}, {1.0f, 0.0f}, {0.5f, 1.0f}, {0.0f, 1.0f}, {0.5f, 0.0f}},
};

// returns true on win and false on lose  and score
std::tuple<Uint64, bool> play_level(const float initial_score_multiplyer) {
  Uint64 score = 0;
  ship_type player_ship = []() -> ship_type {
    SDL_Texture *player_ship_texture = nullptr;

    SDL_IOStream *texture_file = SDL_IOFromFile("assets/player_ship.svg", "r");

    float ship_width = static_cast<float>(static_cast<double>(mode->w) /
                                          static_cast<double>(WIDTH_RATIO));
    float ship_height = static_cast<float>(static_cast<double>(mode->h) /
                                           static_cast<double>(HEIGHT_RATIO));

    SDL_Surface *player_ship_surface =
        IMG_LoadSizedSVG_IO(texture_file, ship_width, ship_height);
    SDL_CloseIO(texture_file);

    player_ship_texture = SDL_CreateTextureFromSurface(
        main_sdl_session.renderer, player_ship_surface);
    SDL_DestroySurface(player_ship_surface);

    SDL_FRect player_ship_rect = {0, (mode->h / 2) - (ship_height / 2),
                                  ship_width, ship_height};

    SDL_FPoint gun_offset = {ship_width * 0.9f, ship_height * 0.9f};

    return {player_ship_rect, gun_offset, player_ship_texture, player.health};
  }();
  bool running = true;
  bool paused = false;
  float overheat_counter = 0;
  SDL_Color overheat_bar_color = {HEX_TO_SDL_COLOR(0xf78600ff)};
  Uint64 lastFrameTime = SDL_GetTicksNS();
  float deltaTime = 0.0f;
  Uint8 player_ship_speed = 1;
  std::vector<projectile> projectiles;
  std::vector<enemy_type> enemies;
  std::vector<powerup_type> powerups;
  std::vector<active_powerup> active_powerups;
  SDL_FRect health_bar = {
      static_cast<float>(level_screen_limit.x),
      static_cast<float>(level_screen_limit.y - HEALTH_BAR_THICKNESS -
                         /*padding*/ 1),
      static_cast<float>(level_screen_limit.x + level_screen_limit.w),
      HEALTH_BAR_THICKNESS};
  bool overheat_mode = false;
  float score_multiplyer;
  int calculated_damage;
  Uint32 fire_delay;
  float cooldown_per_shot;
  double enemy_spawn_rate = 500;
  while (running) {
    const Uint64 frameStart = SDL_GetTicksNS();

    // calculate powerup effects
    score_multiplyer = initial_score_multiplyer;
    calculated_damage = player.damage;
    fire_delay = MIN_INPUT_DELAY_FIRE;
    cooldown_per_shot = OVERHEAT_PER_SHOT / 2;
    for (active_powerup &ap : active_powerups) {

      switch (ap.type) {
      case TWO_X:
        score_multiplyer *= 2;
        break;
      case BOOM:
        calculated_damage *= 3;
        break;
      case THREE_X:
        score_multiplyer *= 3;
        break;
      case BEAM:
        fire_delay /= 5;
        cooldown_per_shot *= 5;
        break;
      case FIVE_X:
        score_multiplyer *= 5;
        break;
      }
      if (SDL_GetTicks() - ap.activation_tick > DEFAULT_POWERUP_DURATION) {
        active_powerups.erase(active_powerups.begin() +
                              (&ap - active_powerups.data()));
        // DEBUG
        SDL_Time time;
        SDL_GetCurrentTime(&time);
        std::clog << "[DEBUG] " << "poweroup deleted at " << time << "\n";
      }
    }

    SDL_Event event;
    while (SDL_PollEvent(&event)) {
      switch (event.type) {
      case SDL_EVENT_QUIT:
        running = false;
        break;
      default:
        break;
      }
    }

    {
      const bool *keystate = SDL_GetKeyboardState(NULL);
      static Uint32 last_toggle_direction[4];
      std::memset(last_toggle_direction, 0, sizeof(last_toggle_direction));
      if ((keystate[SDL_SCANCODE_DOWN] || keystate[SDL_SCANCODE_S]) &&
          (SDL_GetTicks() - last_toggle_direction[0] >
           MIN_INPUT_DELAY_MOVEMENT)) {
        if (player_ship.rect.y + player_ship.rect.h <
            (level_screen_limit.h + level_screen_limit.y)) {
          player_ship.rect.y += player_ship_speed;
        }
        last_toggle_direction[0] = SDL_GetTicks();
      };
      if ((keystate[SDL_SCANCODE_UP] || keystate[SDL_SCANCODE_W]) &&
          (SDL_GetTicks() - last_toggle_direction[1] >
           MIN_INPUT_DELAY_MOVEMENT)) {
        if (player_ship.rect.y > level_screen_limit.y) {
          player_ship.rect.y -= player_ship_speed;
        }
        last_toggle_direction[1] = SDL_GetTicks();
      }
      if ((keystate[SDL_SCANCODE_RIGHT] || keystate[SDL_SCANCODE_D]) &&
          (SDL_GetTicks() - last_toggle_direction[2] >
           MIN_INPUT_DELAY_MOVEMENT)) {
        if (player_ship.rect.x + player_ship.rect.w < mode->w) {
          player_ship.rect.x += player_ship_speed;
        }
        last_toggle_direction[2] = SDL_GetTicks();
      }
      if ((keystate[SDL_SCANCODE_LEFT] || keystate[SDL_SCANCODE_A]) &&
          (SDL_GetTicks() - last_toggle_direction[3] >
           MIN_INPUT_DELAY_MOVEMENT)) {
        if (player_ship.rect.x > 0) {
          player_ship.rect.x -= player_ship_speed;
        }
        last_toggle_direction[3] = SDL_GetTicks();
      }
      static Uint32 last_pouse_tick;
      if (keystate[SDL_SCANCODE_P] && SDL_GetTicks() - last_pouse_tick > 200) {
        SDL_Delay(100000);
        last_pouse_tick = SDL_GetTicks();
      }

      const SDL_MouseButtonFlags mousestate =
          SDL_GetMouseState(nullptr, nullptr);
      static Uint32 last_fire = 0;
      if ((mousestate & SDL_BUTTON_LMASK || keystate[SDL_SCANCODE_SPACE]) &&
          SDL_GetTicks() - last_fire > fire_delay && !overheat_mode) {
        projectiles.push_back(spawn_projectile(
            {player_ship.rect.x + player_ship.gun_offset.x,
             player_ship.rect.y + player_ship.gun_offset.y},
            1, 90, NORMAL_PROJECTILE_SPEED, "assets/basic_projectile.svg",
            nullptr, ALLY, calculated_damage));

        overheat_counter += OVERHEAT_PER_SHOT;

        last_fire = SDL_GetTicks();
      }
    }

    // chance to spawn enemy every frame
    if (get_random_num(0, enemy_spawn_rate) == 0) {
      enemies.push_back(spawn_enemy(
          static_cast<enemy_ai_type>(get_weighted_random(RANDOM, GUNNER)),
          get_random_num(200, 1000), get_weighted_random_float(0.9f, 5)));
    }

    SDL_SetRenderDrawColor(main_sdl_session.renderer, 0, 0, 0, 255);
    SDL_RenderClear(main_sdl_session.renderer);

    for (projectile &p : projectiles) {
      if (p.rect.x > mode->w || p.rect.y > mode->h) {
        projectiles.erase(projectiles.begin() + (&p - projectiles.data()));
      } else {
        if (p.type == ALLY) {
          for (enemy_type &e : enemies) {
            if (SDL_HasRectIntersectionFloat(&p.rect, &e.ship.rect)) {
              e.ship.health -= p.damage;
              if (e.ship.health <= 0) {
                // TODO play explosion or something
                score += (((e.type + 1) * 10) * e.size_multiplier) *
                         score_multiplyer;
                if (get_random_num(0, 20 / e.size_multiplier) == 0) {
                  powerups.push_back(summon_powerup(
                      {e.ship.rect.x, e.ship.rect.y},
                      static_cast<powerup_efect_type>(
                          get_weighted_random(powerup_efect_type::TWO_X,
                                              powerup_efect_type::FIVE_X))));
                }
                enemies.erase(enemies.begin() + (&e - enemies.data()));
              }
              projectiles.erase(projectiles.begin() +
                                (&p - projectiles.data()));
              goto skip_projectile_step;
            }
          }
        } else {
          if (SDL_HasRectIntersectionFloat(&p.rect, &player_ship.rect)) {
            player_ship.health -= p.damage;
            if (player_ship.health <= 0) {
              return std::make_tuple(score, false);
            }
            projectiles.erase(projectiles.begin() + (&p - projectiles.data()));
          }
        }
        step_projectile(p);
      skip_projectile_step:
      }
    }
    for (enemy_type &e : enemies) {
      step_enemy(e, player_ship, projectiles);
    }
    // process powerups
    for (powerup_type &pu : powerups) {
      if (SDL_HasRectIntersectionFloat(&player_ship.rect, &pu.rect)) {
        active_powerups.push_back({pu.type, SDL_GetTicks()});
        // DEBUG
        SDL_Time time;
        SDL_GetCurrentTime(&time);
        std::clog << "[DEBUG] " << "poweroup got at " << time << "\n";
        goto erase_powerup;
      } else {
        if (SDL_GetTicks() - pu.spawn_tick < POWERUP_LIFE_TIME) {
          SDL_RenderTexture(main_sdl_session.renderer, pu.texture, nullptr,
                            &pu.rect);
        } else {
        erase_powerup:
          powerups.erase(powerups.begin() + (&pu - powerups.data()));
        }
      }
    }

    SDL_RenderTexture(main_sdl_session.renderer, player_ship.texture, nullptr,
                      &player_ship.rect);

    // draw UI
    SDL_SetRenderDrawColor(
        main_sdl_session.renderer,
        HEX_TO_SDL_COLOR(0xffffffff)); // set white render color

    SDL_RenderLine(
        main_sdl_session.renderer, level_screen_limit.x, level_screen_limit.y,
        level_screen_limit.x + level_screen_limit.w, level_screen_limit.y);

    SDL_RenderLine(main_sdl_session.renderer, level_screen_limit.x,
                   level_screen_limit.y + level_screen_limit.h,
                   level_screen_limit.x + level_screen_limit.w,
                   level_screen_limit.y + level_screen_limit.h);

    {
      std::string score_str = std::to_string(score);
      vector_print({5, 5}, level_screen_limit.y - 10, score_str);
    }

    // render health bar
    SDL_SetRenderDrawColor(main_sdl_session.renderer,
                           HEX_TO_SDL_COLOR(0xff0000ff));
    health_bar.w = static_cast<float>(level_screen_limit.w *
                                      (static_cast<float>(player_ship.health) /
                                       static_cast<float>(player.health)));

    SDL_RenderFillRect(main_sdl_session.renderer, &health_bar);

    // render overheat bars
    {
      if (overheat_counter >= 100) {
        overheat_mode = true;
      }
      SDL_SetRenderDrawColor(main_sdl_session.renderer,
                             SDL_COLOR_RGBA(overheat_bar_color));
      static Uint32 last_cooldown = 0;
      if (overheat_counter > 0 && SDL_GetTicks() - last_cooldown > fire_delay) {
        overheat_counter -= cooldown_per_shot;
        last_cooldown = SDL_GetTicks();
        if (overheat_mode) {
          static bool overheat_color = true;
          if (overheat_color) {
            overheat_bar_color = {HEX_TO_SDL_COLOR(0xff0000ff)};
            overheat_color = false;
          } else {
            overheat_bar_color = {HEX_TO_SDL_COLOR(0xf78600ff)};
            overheat_color = true;
          }
        }
      }

      if (overheat_mode && overheat_counter < 50) {
        overheat_bar_color = {HEX_TO_SDL_COLOR(0xf78600ff)};
        overheat_mode = false;
      }

      std::remove_const_t<decltype(overheat_block)> overheat_block_sized{};
      SDL_FPoint origin = {
          static_cast<float>(level_screen_limit.x),
          static_cast<float>(level_screen_limit.y + level_screen_limit.h + 2)};
      for (Uint8 i = 0; i < overheat_counter / 10; i++) {
        multiply_FPoint_array(
            overheat_block_sized.data(), overheat_block.data(), origin,
            (mode->h - (level_screen_limit.y + level_screen_limit.h)) - 3,
            overheat_block.size());
        SDL_RenderLines(main_sdl_session.renderer, overheat_block_sized.data(),
                        overheat_block_sized.size());
        origin.x +=
            (mode->h - (level_screen_limit.y + level_screen_limit.h)) - 3;
      }
    }

    SDL_RenderPresent(main_sdl_session.renderer);

    enemy_spawn_rate *=
        0.99995f; // increase enemy spawn rate reaches peak in about 30mins

    const Uint64 frameTime = SDL_GetTicksNS() - frameStart;
    if (frameTime < TARGET_FRAME_TIME_NS) {
      const Uint64 sleepTime = TARGET_FRAME_TIME_NS - frameTime;
      if (sleepTime >= 2'000'000) {
        SDL_DelayNS(sleepTime - 2'000'000);
      }
      while (SDL_GetTicksNS() - frameStart < TARGET_FRAME_TIME_NS) {
      }
    }

    deltaTime = (SDL_GetTicksNS() - lastFrameTime) / 1e9f;
    lastFrameTime = SDL_GetTicksNS();
  }

  SDL_DestroyTexture(player_ship.texture);
  return std::make_tuple(score, true);
}