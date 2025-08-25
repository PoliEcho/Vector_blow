#include "const.hpp"
#include "game_logic.hpp"
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
      SDL_CreateWindow("Vector Blow", mode->w, mode->h, SDL_WINDOW_FULLSCREEN);

  main_sdl_session.renderer =
      SDL_CreateRenderer(main_sdl_session.window, "gpu,vulcan");

  auto [score, win] = play_level(1);
  std::cout << "you got score: " << score
            << " and you: " << (win ? "WON" : "LOST") << "\n";

  SDL_DestroyRenderer(main_sdl_session.renderer);
  SDL_DestroyWindow(main_sdl_session.window);
  SDL_Quit();

  return 0;
}
