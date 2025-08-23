#pragma once

#include "types.hpp"
#include <cmath>

SDL_FPoint find_dst_point_with_angle_and_distance(SDL_FPoint start, Angle angle,
                                                  float distance) {
  float angle_rad = (angle - 90) * M_PI / 180.0f;

  float dx = std::cos(angle_rad) * distance;
  float dy = std::sin(angle_rad) * distance;

  float calculated_distance = std::sqrt(dx * dx + dy * dy);

  SDL_FPoint result;

  if (calculated_distance > 0) {
    dx /= calculated_distance;
    dy /= calculated_distance;

    result.x = start.x + dx * distance;
    result.y = start.y + dy * distance;
  } else {
    result.x = start.x;
    result.y = start.y;
  }

  return result;
}
