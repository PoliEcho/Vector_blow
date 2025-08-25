#pragma once
#include "types.hpp"
#include <SDL3/SDL_rect.h>
projectile spawn_projectile(SDL_FPoint position, float size_multiplier,
                            Angle angle, float speed, char *texture_file_name,
                            SDL_FRect *target, projectile_type type);
void step_projectile(projectile &p);
