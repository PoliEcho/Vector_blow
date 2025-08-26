#pragma once
#include <SDL3/SDL.h>
#include <SDL3/SDL_rect.h>
#include <SDL3/SDL_render.h>
#include <SDL3/SDL_stdinc.h>
#include <SDL3_image/SDL_image.h>
#include <cmath>
#include <iostream>

enum projectile_type { ALLY, FOE };

enum enemy_ai_type {
  RANDOM,
  FLYER,
  GUNNER,
  ACE,
  BOSS,
};

enum powerup_efect_type {
  TWO_X,
  BOOM,
  THREE_X,
  BEAM,
  FIVE_X,
};

struct Angle {
private:
  float value{0};
  void normalize() {
    value = std::fmod(value, 360);
    if (value < 0)
      value += 360; // Handle negative values
  }

public:
  // Constructors
  Angle(int val = 0) : value(static_cast<float>(val)) { normalize(); }
  Angle(float val) : value(val) { normalize(); }
  Angle(double val) : value(static_cast<float>(val)) { normalize(); }

  // Assignment operators
  Angle &operator=(int val) {
    value = static_cast<float>(val);
    normalize();
    return *this;
  }

  Angle &operator=(float val) {
    value = val;
    normalize();
    return *this;
  }

  Angle &operator=(double val) {
    value = static_cast<float>(val);
    normalize();
    return *this;
  }

  // Compound assignment
  Angle &operator+=(int rhs) {
    value += static_cast<float>(rhs);
    normalize();
    return *this;
  }

  Angle &operator-=(int rhs) {
    value -= static_cast<float>(rhs);
    normalize();
    return *this;
  }

  Angle &operator+=(float rhs) {
    value += rhs;
    normalize();
    return *this;
  }

  Angle &operator-=(float rhs) {
    value -= rhs;
    normalize();
    return *this;
  }

  // Binary operators for int
  Angle operator+(int rhs) const {
    Angle result;
    result.value = value + static_cast<float>(rhs);
    result.normalize();
    return result;
  }

  Angle operator-(int rhs) const {
    Angle result;
    result.value = value - static_cast<float>(rhs);
    result.normalize();
    return result;
  }

  Angle operator*(int rhs) const {
    Angle result;
    result.value = value * static_cast<float>(rhs);
    result.normalize();
    return result;
  }

  Angle operator/(int rhs) const {
    Angle result;
    result.value = value / static_cast<float>(rhs);
    result.normalize();
    return result;
  }

  // Binary operators for double
  Angle operator+(double rhs) const {
    Angle result;
    result.value = value + static_cast<float>(rhs);
    result.normalize();
    return result;
  }

  Angle operator-(double rhs) const {
    Angle result;
    result.value = value - static_cast<float>(rhs);
    result.normalize();
    return result;
  }

  Angle operator*(double rhs) const {
    Angle result;
    result.value = value * static_cast<float>(rhs);
    result.normalize();
    return result;
  }

  Angle operator/(double rhs) const {
    Angle result;
    result.value = value / static_cast<float>(rhs);
    result.normalize();
    return result;
  }

  // Binary operators for float
  Angle operator+(float rhs) const {
    Angle result;
    result.value = value + rhs;
    result.normalize();
    return result;
  }

  Angle operator-(float rhs) const {
    Angle result;
    result.value = value - rhs;
    result.normalize();
    return result;
  }

  Angle operator*(float rhs) const {
    Angle result;
    result.value = value * rhs;
    result.normalize();
    return result;
  }

  Angle operator/(float rhs) const {
    Angle result;
    result.value = value / rhs;
    result.normalize();
    return result;
  }

  // Type conversion
  operator int() const { return static_cast<int>(value); }
  operator double() const { return static_cast<double>(value); }
  operator float() const { return value; }

  // Stream output
  friend std::ostream &operator<<(std::ostream &os, const Angle &a) {
    return os << a.value;
  }
};

struct sdl_session {
  SDL_Window *window;
  SDL_Renderer *renderer;
};

struct ship_type {
  SDL_FRect rect;
  SDL_FPoint gun_offset;
  SDL_Texture *texture;
  int health;
};

struct enemy_type {
  ship_type ship;
  enemy_ai_type type;
  Uint32 last_shot;
  Uint32 reload_time;
  SDL_FPoint target;
  float size_multiplier;
};

struct projectile {
  SDL_FRect rect;
  SDL_Texture *texture;
  float speed;
  Angle angle = 0;
  bool guided;
  projectile_type type;
  SDL_FRect *target;
  int damage;
};

struct player_type {
  int health;
  int damage;
};

struct powerup_type {
  SDL_FRect rect;
  SDL_Texture *texture;
  powerup_efect_type type;
  Uint32 spawn_tick;
};

struct active_powerup {
  powerup_efect_type type;
  Uint64 activation_tick;
};
