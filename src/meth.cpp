#include "types.hpp"
#include <cmath>
#include <random>

[[gnu::const]] SDL_FPoint
find_dst_point_with_angle_and_distance(SDL_FPoint start, Angle angle,
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

  std::uniform_int_distribution<int> dist(min, max);
  return dist(rng);
}

int get_weighted_random(
    int min, int max) { // smaller numbers  much more likely than higer numbers
  int range = max - min + 1;
  int total_weight = range * (range + 1) / 2;

  int random_weight = get_random_num(0, total_weight - 1);

  int cumulative_weight = 0;
  for (int i = 0; i < range; i++) {
    int current_number = min + i;
    int weight = range - i;

    cumulative_weight += weight;
    if (random_weight < cumulative_weight) {
      return current_number;
    }
  }

  return max;
}

#define WEIGHTED_PRECISION 0.05f
float get_weighted_random_float(float min, float max) {
  int steps = (int)((max - min) / WEIGHTED_PRECISION) + 1;

  int total_weight = steps * (steps + 1) / 2;
  int random_weight = get_random_num(0, total_weight - 1);

  int cumulative_weight = 0;
  for (int i = 0; i < steps; i++) {
    int weight = steps - i;
    cumulative_weight += weight;
    if (random_weight < cumulative_weight) {
      return min + (float)i * WEIGHTED_PRECISION;
    }
  }

  return max;
}
