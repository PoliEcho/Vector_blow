#pragma once
#include "types.hpp"
#include <SDL3/SDL_rect.h>
SDL_FPoint find_dst_point_with_angle_and_distance(SDL_FPoint start, Angle angle,
                                                  float distance);
int get_random_num(int min, int max);
int get_weighted_random(int min, int max);
float get_weighted_random_float(float min, float max);