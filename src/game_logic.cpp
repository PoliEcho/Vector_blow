#include "const.hpp"
#include "enemies.hpp"
#include "macro.hpp"
#include "main.hpp"
#include "meth.hpp"
#include "projectiles.hpp"
#include "text.hpp"
#include "types.hpp"
#include <SDL3/SDL_rect.h>
#include <SDL3/SDL_render.h>
#include <SDL3/SDL_scancode.h>
#include <SDL3/SDL_stdinc.h>
#include <SDL3/SDL_timer.h>
#include <cstring>
#include <string>
#include <tuple>
#include <vector>

// returns true on win and false on lose  and score
std::tuple<Uint64, bool> play_level(float score_multiplyer) {
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

    return {player_ship_rect, gun_offset, player_ship_texture};
  }();
  bool running = true;
  bool paused = false;
  Uint64 lastFrameTime = SDL_GetTicksNS();
  float deltaTime = 0.0f;

  Uint8 player_ship_speed = 1;
  std::vector<projectile> projectiles;
  std::vector<enemy_type> enemies;
  while (running) {
    const Uint64 frameStart = SDL_GetTicksNS();

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
      }
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
          SDL_GetTicks() - last_fire > MIN_INPUT_DELAY_FIRE) {
        projectiles.push_back(
            spawn_projectile({player_ship.rect.x + player_ship.gun_offset.x,
                              player_ship.rect.y + player_ship.gun_offset.y},
                             1, 90, NORMAL_PROJECTILE_SPEED,
                             "assets/basic_projectile.svg", nullptr, ALLY));
        last_fire = SDL_GetTicks();
      }
    }

    // chance to spawn enemy every frame
    if (get_random_num(0, 500) == 0) {
      enemies.push_back(
          spawn_enemy(static_cast<enemy_ai_type>(get_random_num(RANDOM, 1)),
                      get_random_num(200, 1000)));
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
              // TODO play explosion or something
              score += ((e.type + 1) * 10) * score_multiplyer;
              enemies.erase(enemies.begin() + (&e - enemies.data()));
              projectiles.erase(projectiles.begin() +
                                (&p - projectiles.data()));
              goto skip_projectile_step;
            }
          }
        } else {
          if (SDL_HasRectIntersectionFloat(&p.rect, &player_ship.rect)) {
            // TODO add hit points
            return std::make_tuple(score, false);
          }
        }
        step_projectile(p);
      skip_projectile_step:
      }
    }
    for (enemy_type &e : enemies) {
      step_enemy(e, player_ship, projectiles);
    }

    SDL_RenderTexture(main_sdl_session.renderer, player_ship.texture, nullptr,
                      &player_ship.rect);

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

    SDL_RenderPresent(main_sdl_session.renderer);

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