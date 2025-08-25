#include "main.hpp"
#include <SDL3/SDL_oldnames.h>
#include <SDL3/SDL_rect.h>
#include <SDL3/SDL_render.h>
#include <array>
#include <cstddef>
#include <cstring>
#include <string>

constexpr std::array<std::array<SDL_FPoint, 9>, 10> characters = {
    {{{{0.0f, 0.0f},
       {1.0f, 0.0f},
       {1.0f, 1.0f},
       {0.0f, 1.0f},
       {0.0f, 0.0f}}},               // zero
     {{{0.5f, 0.0f}, {0.5f, 1.0f}}}, // one
     {{{0.0f, 0.0f},
       {1.0f, 0.0f},
       {1.0f, 0.5f},
       {0.0f, 0.5f},
       {0.0f, 1.0f},
       {1.0f, 1.0f}}}, // two
     {{{0.0f, 0.0f},
       {1.0f, 0.0f},
       {1.0f, 0.5f},
       {0.0f, 0.5f},
       {1.0f, 0.5f},
       {1.0f, 1.0f},
       {0.0f, 1.0f}}}, // three
     {{{0.0f, 0.0f},
       {0.0f, 0.5f},
       {1.0f, 0.5f},
       {1.0f, 0.0f},
       {1.0f, 0.5f},
       {1.0f, 1.0f}}}, // four
     {{{1.0f, 0.0f},
       {0.0f, 0.0f},
       {0.0f, 0.5f},
       {1.0f, 0.5f},
       {1.0f, 1.0f},
       {0.0f, 1.0f}}}, // five
     {{{0.0f, 0.0f},
       {0.0f, 0.5f},
       {1.0f, 0.5f},
       {1.0f, 1.0f},
       {0.0f, 1.0f},
       {0.0f, 0.5f}}},                             // six
     {{{0.0f, 0.0f}, {1.0f, 0.0f}, {1.0f, 1.0f}}}, // seven
     {{{0.0f, 0.0f},
       {1.0f, 0.0f},
       {1.0f, 0.5f},
       {0.0f, 0.5f},
       {0.0f, 0.0f},
       {0.0f, 1.0f},
       {1.0f, 1.0f},
       {1.0f, 0.5f}}}, // eight
     {{{0.0f, 0.0f},
       {0.0f, 0.5f},
       {1.0f, 0.5f},
       {1.0f, 0.0f},
       {0.0f, 0.0f},
       {1.0f, 0.0f},
       {1.0f, 1.0f}}}}}; // nine

void multiply_FPoint_array(SDL_FPoint *dst, const SDL_FPoint *src,
                           SDL_FPoint origin, int multiplyer,
                           size_t num_points) {
  for (size_t i = 0; i < num_points; i++) {
    dst[i].x = origin.x + (src[i].x * multiplyer);
    dst[i].y = origin.y + (src[i].y * multiplyer);
  }
}

void vector_print(SDL_FPoint pos, int height, std::string &s) {
  std::array<SDL_FPoint, 9> copybuffer;
  int point_count = 0;
  for (size_t i = 0; i < s.length(); i++) {
    switch (s.data()[i]) {
    case '0':
      point_count = 5;
      multiply_FPoint_array(copybuffer.data(), characters[0].data(), pos,
                            height, point_count);

      break;
    case '1':
      point_count = 2;
      multiply_FPoint_array(copybuffer.data(), characters[1].data(), pos,
                            height, point_count);
      break;
    case '2':
      point_count = 6;
      multiply_FPoint_array(copybuffer.data(), characters[2].data(), pos,
                            height, point_count);
      break;
    case '3':
      point_count = 7;
      multiply_FPoint_array(copybuffer.data(), characters[3].data(), pos,
                            height, point_count);
      break;
    case '4':
      point_count = 6;
      multiply_FPoint_array(copybuffer.data(), characters[4].data(), pos,
                            height, point_count);
      break;
    case '5':
      point_count = 6;
      multiply_FPoint_array(copybuffer.data(), characters[5].data(), pos,
                            height, point_count);
      break;
    case '6':
      point_count = 6;
      multiply_FPoint_array(copybuffer.data(), characters[6].data(), pos,
                            height, point_count);
      break;
    case '7':
      point_count = 3;
      multiply_FPoint_array(copybuffer.data(), characters[7].data(), pos,
                            height, point_count);
      break;
    case '8':
      point_count = 8;
      multiply_FPoint_array(copybuffer.data(), characters[8].data(), pos,
                            height, point_count);
      break;
    case '9':
      point_count = 7;
      multiply_FPoint_array(copybuffer.data(), characters[9].data(), pos,
                            height, point_count);
      break;
    }
    SDL_RenderLines(main_sdl_session.renderer, copybuffer.data(), point_count);
    pos.x = pos.x + height + height / 6;
  }
}