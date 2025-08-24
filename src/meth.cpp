#include "types.hpp"
#include <cmath>
#include <random>

SDL_FPoint find_dst_point_with_angle_and_distance(SDL_FPoint start, Angle angle,
                                                  float distance) {
  float angle_degrees = static_cast<float>(angle);
  float angle_rad = (angle_degrees - 90.0f) * M_PI / 180.0f;

  float dx = std::cos(angle_rad) * distance;
  float dy = std::sin(angle_rad) * distance;

  SDL_FPoint result;
  result.x = start.x + dx;
  result.y = start.y + dy;
  return result;
}

int get_random_num(int min, int max) {
  static std::random_device dev;
  static std::mt19937 rng(dev());

  std::uniform_int_distribution<std::mt19937::result_type> dist(min, max);
  return dist(rng);
}
