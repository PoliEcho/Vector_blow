#pragma once

#define FIND_CENTER(rect) {rect.x + (rect.w / 2), rect.y + (rect.h / 2)}

#define HEX_TO_SDL_COLOR(hex)                                                  \
  ((hex >> 24) & 0xFF), ((hex >> 16) & 0xFF), ((hex >> 8) & 0xFF), (hex & 0xFF)

#define ANGLE_BETWEEN_POINTS_DEG(p1, p2)                                       \
  (atan2(((p2).y - (p1).y), ((p2).x - (p1).x)) * 180.0 / M_PI)

#define SDL_COLOR_RGBA(color) (color).r, (color).g, (color).b, (color).a

#define ARRAY_LENGHT(arr) (sizeof(arr) / sizeof((arr)[0]))
