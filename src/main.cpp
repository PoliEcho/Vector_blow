#include "const.hpp"
#include "types.hpp"
#include <SDL3/SDL.h>
#include <SDL3/SDL_events.h>
#include <SDL3/SDL_iostream.h>
#include <SDL3/SDL_render.h>
#include <SDL3/SDL_scancode.h>
#include <SDL3/SDL_stdinc.h>
#include <SDL3/SDL_surface.h>
#include <SDL3/SDL_video.h>
#include <SDL3_image/SDL_image.h>
#include <cstring>

sdl_session main_sdl_session;

#define WIDTH_RATIO 15
#define HEIGHT_RATIO 41.53846153846154
#define MIN_INPUT_DELAY 0

int main(int argc, char *argv[]) {
  SDL_Init(SDL_INIT_VIDEO);

  const SDL_DisplayMode *mode =
      SDL_GetCurrentDisplayMode(SDL_GetPrimaryDisplay());

  main_sdl_session.window =
      SDL_CreateWindow("Vector Blow", mode->w, mode->h, SDL_WINDOW_FULLSCREEN);

  main_sdl_session.renderer =
      SDL_CreateRenderer(main_sdl_session.window, "gpu,vulcan");

  SDL_Texture *player_ship_texture = nullptr;
  {
    SDL_IOStream *texture_file = SDL_IOFromFile("assets/player_ship.svg", "r");
    SDL_Surface *player_ship_surface = IMG_LoadSizedSVG_IO(
        texture_file,
        static_cast<int>(static_cast<double>(mode->w) /
                         static_cast<double>(WIDTH_RATIO)),
        static_cast<int>(static_cast<double>(mode->h) /
                         static_cast<double>(HEIGHT_RATIO)));

    SDL_CloseIO(texture_file);

    player_ship_texture = SDL_CreateTextureFromSurface(
        main_sdl_session.renderer, player_ship_surface);
    SDL_DestroySurface(player_ship_surface);
  }

  SDL_FRect player_ship_rect = {
      0, 0,
      static_cast<float>(static_cast<double>(mode->w) /
                         static_cast<double>(WIDTH_RATIO)),
      static_cast<float>(static_cast<double>(mode->h) /
                         static_cast<double>(HEIGHT_RATIO))};

  bool running = true;
  Uint64 lastFrameTime = SDL_GetTicksNS();
  float deltaTime = 0.0f;

  Uint8 player_ship_speed = 1;
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

    const bool *keystate = SDL_GetKeyboardState(NULL);
    static Uint32 last_toggle_direction[4];
    std::memset(last_toggle_direction, 0, sizeof(last_toggle_direction));
    if ((keystate[SDL_SCANCODE_DOWN] || keystate[SDL_SCANCODE_S]) &&
        (SDL_GetTicks() - last_toggle_direction[0] > MIN_INPUT_DELAY)) {
      player_ship_rect.y += player_ship_speed;
      last_toggle_direction[0] = SDL_GetTicks();
    }
    if ((keystate[SDL_SCANCODE_UP] || keystate[SDL_SCANCODE_W]) &&
        (SDL_GetTicks() - last_toggle_direction[1] > MIN_INPUT_DELAY)) {
      player_ship_rect.y -= player_ship_speed;
      last_toggle_direction[1] = SDL_GetTicks();
    }
    if ((keystate[SDL_SCANCODE_RIGHT] || keystate[SDL_SCANCODE_D]) &&
        (SDL_GetTicks() - last_toggle_direction[2] > MIN_INPUT_DELAY)) {
      player_ship_rect.x += player_ship_speed;
      last_toggle_direction[2] = SDL_GetTicks();
    }
    if ((keystate[SDL_SCANCODE_LEFT] || keystate[SDL_SCANCODE_A]) &&
        (SDL_GetTicks() - last_toggle_direction[3] > MIN_INPUT_DELAY)) {
      player_ship_rect.x -= player_ship_speed;
      last_toggle_direction[3] = SDL_GetTicks();
    }

    SDL_SetRenderDrawColor(main_sdl_session.renderer, 0, 0, 0, 255);
    SDL_RenderClear(main_sdl_session.renderer);

    SDL_RenderTexture(main_sdl_session.renderer, player_ship_texture, nullptr,
                      &player_ship_rect);

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

  SDL_DestroyTexture(player_ship_texture);
  SDL_DestroyRenderer(main_sdl_session.renderer);
  SDL_DestroyWindow(main_sdl_session.window);
  SDL_Quit();

  return 0;
}
