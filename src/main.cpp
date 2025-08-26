#include "const.hpp"
#include "game_logic.hpp"
#include "macro.hpp"
#include "text.hpp"
#include "types.hpp"
#include <SDL3/SDL.h>
#include <SDL3/SDL_events.h>
#include <SDL3/SDL_iostream.h>
#include <SDL3/SDL_mouse.h>
#include <SDL3/SDL_rect.h>
#include <SDL3/SDL_render.h>
#include <SDL3/SDL_scancode.h>
#include <SDL3/SDL_stdinc.h>
#include <SDL3/SDL_surface.h>
#include <SDL3/SDL_timer.h>
#include <SDL3/SDL_video.h>
#include <SDL3_image/SDL_image.h>

sdl_session main_sdl_session;
const SDL_DisplayMode *mode;

SDL_Rect level_screen_limit;

player_type player = {100, 10};

int main(int argc, char *argv[]) {
  SDL_Init(SDL_INIT_VIDEO);

  mode = SDL_GetCurrentDisplayMode(SDL_GetPrimaryDisplay());
  level_screen_limit = {0, static_cast<int>(mode->h * LEVEL_UI_RATIO), mode->w,
                        static_cast<int>((mode->h - (mode->h * LEVEL_UI_RATIO) -
                                          mode->h * LEVEL_UI_RATIO))};

  main_sdl_session.window =
      SDL_CreateWindow(program_name, mode->w, mode->h, SDL_WINDOW_FULLSCREEN);

  main_sdl_session.renderer =
      SDL_CreateRenderer(main_sdl_session.window, "gpu,vulcan");
  bool running = true;
  Uint64 lastFrameTime = SDL_GetTicksNS();
  float deltaTime = 0.0f;

  constexpr int any_key_text_size = 20;

  const int any_key_middle_of_screen =
      (mode->w / 2) - (((((ARRAY_LENGHT(any_key) - 1) *
                          (any_key_text_size + any_key_text_size / 6))) -
                        any_key_text_size / 6) /
                       2);

  Uint32 last_blink = SDL_GetTicks();
  bool show_any_key = true;
  while (running) {
    const Uint64 frameStart = SDL_GetTicksNS();

    SDL_Event event;
    while (SDL_PollEvent(&event)) {
      switch (event.type) {
      case SDL_EVENT_QUIT:
        goto end_program;
        break;
      default:
        break;
      }
    }

    const bool *keystate = SDL_GetKeyboardState(NULL);
    if (keystate[SDL_SCANCODE_SPACE]) {
      running = false;
    }

    SDL_SetRenderDrawColor(main_sdl_session.renderer, 0, 0, 0, 255);
    SDL_RenderClear(main_sdl_session.renderer);

    SDL_SetRenderDrawColor(main_sdl_session.renderer,
                           HEX_TO_SDL_COLOR(0xffffffff));

    vector_print({0, static_cast<float>(mode->h / 2)}, 100, program_name);

    if (show_any_key) {
      vector_print({static_cast<float>(any_key_middle_of_screen),
                    static_cast<float>((mode->h / 2) * 1.5)},
                   any_key_text_size, any_key);
    }

    if (SDL_GetTicks() - last_blink > ANY_KEY_BLINK_DELAY) {
      show_any_key = show_any_key ? false : true;
      last_blink = SDL_GetTicks();
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

  {
    while (true) {
      auto [score, win, play_again] = play_level(1);
      if (!play_again) {
        break;
      }
    }
  }

end_program:
  SDL_DestroyRenderer(main_sdl_session.renderer);
  SDL_DestroyWindow(main_sdl_session.window);
  SDL_Quit();

  return 0;
}
