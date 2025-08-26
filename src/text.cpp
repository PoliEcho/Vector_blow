#include "main.hpp"
#include <SDL3/SDL_oldnames.h>
#include <SDL3/SDL_rect.h>
#include <SDL3/SDL_render.h>
#include <array>
#include <cstddef>
#include <cstring>
#include <string>

constexpr std::array<std::array<SDL_FPoint, 12>, 36> characters = {{
    {{{0.0f, 0.0f},
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
      {1.0f, 1.0f}}}, // nine
    {{{0.0f, 1.0f},
      {0.25f, 0.5f},
      {0.5f, 0.0f},
      {0.75f, 0.5f},
      {1.0f, 1.0f},
      {0.75f, 0.5f},
      {0.25f, 0.5f}}}, // A
    {{{0.0f, 0.0f},
      {0.8536f, 0.0},
      {1.0f, 0.1464f},
      {1.0f, 0.3536f},
      {0.8536f, 0.5f},
      {0.0f, 0.5f},
      {0.8536f, 0.5f},
      {1.0f, 0.6464f},
      {1.0f, 0.8536f},
      {0.8536f, 1.0f},
      {0.0f, 1.0f},
      {0.0f, 0.0f}}}, // B
    {{{1.0f, 0.0f},
      {0.25f, 0.0f},
      {0.0f, 0.25f},
      {0.0f, 0.75f},
      {0.25f, 1.0f},
      {1.0f, 1.0f}}}, // C
    {{{0.0f, 0.0f},
      {0.0f, 1.0f},
      {0.75f, 1.0f},
      {1.0f, 0.75f},
      {1.0f, 0.25f},
      {0.75f, 0.0f},
      {0.0f, 0.0f}}}, // D
    {{{1.0f, 0.0f},
      {0.0f, 0.0f},
      {0.0f, 0.5f},
      {1.0f, 0.5f},
      {0.0f, 0.5f},
      {0.0f, 1.0f},
      {1.0f, 1.0f}}}, // E
    {{{1.0f, 0.0f},
      {0.0f, 0.0f},
      {0.0f, 0.5f},
      {1.0f, 0.5f},
      {0.0f, 0.5f},
      {0.0f, 1.0f}}}, // F
    {{{1.0f, 0.0f},
      {0.0f, 0.0f},
      {0.0f, 1.0f},
      {1.0f, 1.0f},
      {1.0f, 0.5f},
      {0.5f, 0.5f}}}, // G
    {{{0.0f, 0.0f},
      {0.0f, 1.0f},
      {0.0f, 0.5f},
      {1.0f, 0.5f},
      {1.0f, 0.0f},
      {1.0f, 1.0f}}}, // H
    {{{0.0f, 0.0f},
      {1.0f, 0.0f},
      {0.5f, 0.0f},
      {0.5f, 1.0f},
      {0.0f, 1.0f},
      {1.0f, 1.0f}}}, // I
    {{{0.75f, 0.0f},
      {1.0f, 0.0f},
      {1.0f, 0.75f},
      {0.75f, 1.0f},
      {0.0f, 1.0f},
      {0.0f, 0.75f}}}, // J
    {{{0.0f, 0.0f},
      {0.0f, 0.5f},
      {1.0f, 0.0f},
      {0.0f, 0.5f},
      {1.0f, 1.0f},
      {0.0f, 0.5f},
      {0.0f, 1.0f}}},                             // K
    {{{0.0f, 0.0f}, {0.0f, 1.0f}, {1.0f, 1.0f}}}, // L
    {{{0.0f, 1.0f},
      {0.0f, 0.0f},
      {0.5f, 0.5f},
      {1.0f, 0.0f},
      {1.0f, 1.0f}}},                                           // M
    {{{0.0f, 1.0f}, {0.0f, 0.0f}, {1.0f, 1.0f}, {1.0f, 0.0f}}}, // N
    {{{0.0f, 1.0f},
      {0.0f, 0.0f},
      {0.8536f, 0.0f},
      {1.0f, 0.1464f},
      {1.0f, 0.3536f},
      {0.8536f, 0.5f},
      {0.0f, 0.5f}}}, // P
    {{{0.0f, 0.0f},
      {1.0f, 0.0f},
      {1.0f, 1.0f},
      {0.75f, 0.75f},
      {1.0f, 1.0f},
      {0.0f, 1.0f},
      {0.0f, 0.0f}}}, // Q
    {{{0.0f, 1.0f},
      {0.0f, 0.0f},
      {0.8536f, 0.0f},
      {1.0f, 0.1464f},
      {1.0f, 0.3536f},
      {0.8536f, 0.5f},
      {0.0f, 0.5f},
      {1.0f, 1.0f}}}, // R
    {{{1.0f, 0.0f},
      {0.0f, 0.0f},
      {0.0f, 0.5f},
      {1.0f, 0.5f},
      {1.0f, 1.0f},
      {0.0f, 1.0f}}}, // S
    {{{0.0f, 0.0f},
      {0.5f, 0.0f},
      {0.5f, 1.0f},
      {0.5f, 0.0f},
      {1.0f, 0.0f}}},                                           // T
    {{{0.0f, 0.0f}, {0.0f, 1.0f}, {1.0f, 1.0f}, {1.0f, 0.0f}}}, // U
    {{{0.0f, 0.0f}, {0.5f, 1.0f}, {1.0f, 0.0f}}},               // V
    {{{0.0f, 0.0f},
      {0.0f, 1.0f},
      {0.5f, 0.5f},
      {1.0f, 1.0f},
      {1.0f, 0.0f}}}, // W
    {{{0.0f, 0.0f},
      {1.0f, 1.0f},
      {0.5f, 0.5f},
      {1.0f, 0.0f},
      {0.0f, 1.0f}}}, // X
    {{{0.0f, 0.0f},
      {0.5f, 0.5f},
      {0.5f, 1.0f},
      {0.5f, 0.5f},
      {1.0f, 0.0f}}},                                          // Y
    {{{0.0f, 0.0f}, {1.0f, 0.0f}, {0.0f, 1.0f}, {1.0f, 1.0f}}} // Z
}};

void multiply_FPoint_array(SDL_FPoint *dst, const SDL_FPoint *src,
                           SDL_FPoint origin, int multiplyer,
                           size_t num_points) {
  for (size_t i = 0; i < num_points; i++) {
    dst[i].x = origin.x + (src[i].x * multiplyer);
    dst[i].y = origin.y + (src[i].y * multiplyer);
  }
}

void vector_print(SDL_FPoint pos, int height, std::string s) {
  std::transform(s.begin(), s.end(), s.begin(),
                 [](unsigned char c) { return std::tolower(c); });
  std::array<SDL_FPoint, 12> copybuffer;
  int point_count = 0;
  for (size_t i = 0; i < s.length(); i++) {
    switch (s.data()[i]) {
    case '0':
    case 'o': // O has same sprite as 0
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
    case 'a':
      point_count = 7;
      multiply_FPoint_array(copybuffer.data(), characters[10].data(), pos,
                            height, point_count);
      break;
    case 'b':
      point_count = 12;
      multiply_FPoint_array(copybuffer.data(), characters[11].data(), pos,
                            height, point_count);
      break;
    case 'c':
      point_count = 6;
      multiply_FPoint_array(copybuffer.data(), characters[12].data(), pos,
                            height, point_count);
      break;
    case 'd':
      point_count = 7;
      multiply_FPoint_array(copybuffer.data(), characters[13].data(), pos,
                            height, point_count);
      break;
    case 'e':
      point_count = 7;
      multiply_FPoint_array(copybuffer.data(), characters[14].data(), pos,
                            height, point_count);
      break;
    case 'f':
      point_count = 6;
      multiply_FPoint_array(copybuffer.data(), characters[15].data(), pos,
                            height, point_count);
      break;
    case 'g':
      point_count = 6;
      multiply_FPoint_array(copybuffer.data(), characters[16].data(), pos,
                            height, point_count);
      break;
    case 'h':
      point_count = 6;
      multiply_FPoint_array(copybuffer.data(), characters[17].data(), pos,
                            height, point_count);
      break;
    case 'i':
      point_count = 6;
      multiply_FPoint_array(copybuffer.data(), characters[18].data(), pos,
                            height, point_count);
      break;
    case 'j':
      point_count = 6;
      multiply_FPoint_array(copybuffer.data(), characters[19].data(), pos,
                            height, point_count);
      break;
    case 'k':
      point_count = 7;
      multiply_FPoint_array(copybuffer.data(), characters[20].data(), pos,
                            height, point_count);
      break;
    case 'l':
      point_count = 3;
      multiply_FPoint_array(copybuffer.data(), characters[21].data(), pos,
                            height, point_count);
      break;
    case 'm':
      point_count = 5;
      multiply_FPoint_array(copybuffer.data(), characters[22].data(), pos,
                            height, point_count);
      break;
    case 'n':
      point_count = 4;
      multiply_FPoint_array(copybuffer.data(), characters[23].data(), pos,
                            height, point_count);
      break;
    case 'p':
      point_count = 7;
      multiply_FPoint_array(copybuffer.data(), characters[24].data(), pos,
                            height, point_count);
      break;
    case 'q':
      point_count = 7;
      multiply_FPoint_array(copybuffer.data(), characters[25].data(), pos,
                            height, point_count);
      break;
    case 'r':
      point_count = 8;
      multiply_FPoint_array(copybuffer.data(), characters[26].data(), pos,
                            height, point_count);
      break;
    case 's':
      point_count = 6;
      multiply_FPoint_array(copybuffer.data(), characters[27].data(), pos,
                            height, point_count);
      break;
    case 't':
      point_count = 5;
      multiply_FPoint_array(copybuffer.data(), characters[28].data(), pos,
                            height, point_count);
      break;
    case 'u':
      point_count = 4;
      multiply_FPoint_array(copybuffer.data(), characters[29].data(), pos,
                            height, point_count);
      break;
    case 'v':
      point_count = 3;
      multiply_FPoint_array(copybuffer.data(), characters[30].data(), pos,
                            height, point_count);
      break;
    case 'w':
      point_count = 5;
      multiply_FPoint_array(copybuffer.data(), characters[31].data(), pos,
                            height, point_count);
      break;
    case 'x':
      point_count = 5;
      multiply_FPoint_array(copybuffer.data(), characters[32].data(), pos,
                            height, point_count);
      break;
    case 'y':
      point_count = 5;
      multiply_FPoint_array(copybuffer.data(), characters[33].data(), pos,
                            height, point_count);
      break;
    case 'z':
      point_count = 4;
      multiply_FPoint_array(copybuffer.data(), characters[34].data(), pos,
                            height, point_count);
      break;
    }
    SDL_RenderLines(main_sdl_session.renderer, copybuffer.data(), point_count);
    pos.x = pos.x + height + height / 6;
  }
}