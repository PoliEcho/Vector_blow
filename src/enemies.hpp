#pragma once
#include "types.hpp"
#include <vector>
enemy_type spawn_enemy(enemy_ai_type ai_type, Uint32 reload_time,
                       float multiplier);
void step_enemy(enemy_type &e, ship_type &player_ship,
                std::vector<projectile> &projectiles,
                std::vector<powerup_type> &powerups);