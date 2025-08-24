#pragma once
#include "types.hpp"
#include <vector>
enemy_type spawn_enemy(enemy_ai_type ai_type, Uint32 reload_time);
void step_enemy(enemy_type &e, ship_type &player,
                std::vector<projectile> &projectiles);