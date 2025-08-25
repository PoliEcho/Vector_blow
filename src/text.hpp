#pragma once
#include <SDL3/SDL_rect.h>
#include <string>
void multiply_FPoint_array(SDL_FPoint *dst, const SDL_FPoint *src,
                           SDL_FPoint origin, int multiplyer,
                           size_t num_points);
void vector_print(SDL_FPoint pos, int height, std::string &s);