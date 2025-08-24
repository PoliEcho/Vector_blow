#include "main.hpp"
#include <SDL3/SDL_render.h>
#include <SDL3_image/SDL_image.h>
#include <array>

SDL_Texture *texture_from_SVG_file(const char *file_name,
                                   float size_multiplier) {
  SDL_IOStream *texture_file = SDL_IOFromFile(file_name, "r");
  std::array<int, 2> texture_base_size;
  {
    SDL_Surface *tmp_surface = IMG_LoadSVG_IO(texture_file);
    texture_base_size = {tmp_surface->w, tmp_surface->h};
    SDL_DestroySurface(tmp_surface);
  }
  SDL_SeekIO(texture_file, 0, SDL_IO_SEEK_SET); // reset to top of the file
  SDL_Surface *projectile_surface =
      IMG_LoadSizedSVG_IO(texture_file, texture_base_size[0] * size_multiplier,
                          texture_base_size[1] * size_multiplier);

  SDL_CloseIO(texture_file);

  SDL_Texture *texture = SDL_CreateTextureFromSurface(main_sdl_session.renderer,
                                                      projectile_surface);
  SDL_DestroySurface(projectile_surface);
  return texture;
}