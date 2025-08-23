#include "macro.hpp"
#include "main.hpp"
#include "meth.hpp"
#include "types.hpp"
#include <SDL3/SDL.h>
#include <SDL3/SDL_rect.h>
#include <SDL3/SDL_render.h>
#include <SDL3/SDL_surface.h>
#include <SDL3_image/SDL_image.h>
#include <array>
void step_projectile(projectile p);

projectile spawn_projectile(SDL_FPoint position, float size_multiplier,
                            Angle angle, float speed, char *texture_file_name,
                            SDL_FRect *target) {

  projectile p;
  SDL_IOStream *texture_file = SDL_IOFromFile(texture_file_name, "r");
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

  p.texture = SDL_CreateTextureFromSurface(main_sdl_session.renderer,
                                           projectile_surface);
  SDL_DestroySurface(projectile_surface);

  p.angle = angle;
  p.rect = {position.x, position.y, static_cast<float>(p.texture->w),
            static_cast<float>(p.texture->h)};
  p.speed = speed;
  if (target == nullptr) {
    p.target = nullptr;
    p.guided = false;
  } else {
    p.target = target;
    p.guided = true;
  }

  return p;
}

void step_projectile(projectile &p) {
  SDL_FPoint center = FIND_CENTER(p.rect);

  SDL_FPoint dst =
      find_dst_point_with_angle_and_distance(center, p.angle, p.speed);

  p.rect.x = dst.x - (p.rect.w / 2);
  p.rect.y = dst.y - (p.rect.h / 2);
  center = FIND_CENTER(p.rect);
  SDL_RenderTexture(main_sdl_session.renderer, p.texture, nullptr, &p.rect);
  // SDL_RenderTextureRotated(main_sdl_session.renderer, p.texture, nullptr,
  // &p.rect, p.angle, &center, SDL_FLIP_NONE);
}