#pragma once
#include <SDL3/SDL.h>
#include <SDL3/SDL_rect.h>
#include <SDL3_image/SDL_image.h>
#include <cmath>
#include <iostream>

struct Angle {
private:
  float value{0};

  void normalize() {
    value = std::fmod(value, 360);
    if (value < 0)
      value += 360; // Handle negative values
  }

public:
  // Constructor
  Angle(int val = 0) : value(val) { normalize(); }

  // Assignment operator
  Angle &operator=(int val) {
    value = val;
    normalize();
    return *this;
  }

  // Compound assignment
  Angle &operator+=(int rhs) {
    value += rhs;
    normalize();
    return *this;
  }

  Angle &operator-=(int rhs) {
    value -= rhs;
    normalize();
    return *this;
  }
  // Binary operators for int
  Angle operator+(int rhs) const {
    return Angle(static_cast<int>(value) + rhs);
  }

  Angle operator-(int rhs) const {
    return Angle(static_cast<int>(value) - rhs);
  }

  Angle operator*(int rhs) const {
    return Angle(static_cast<int>(value * rhs));
  }

  Angle operator/(int rhs) const {
    return Angle(static_cast<int>(value / rhs));
  }

  // Binary operators for double
  Angle operator+(double rhs) const {
    return Angle(static_cast<int>(value + rhs));
  }

  Angle operator-(double rhs) const {
    return Angle(static_cast<int>(value - rhs));
  }

  Angle operator*(double rhs) const {
    return Angle(static_cast<int>(value * rhs));
  }

  Angle operator/(double rhs) const {
    return Angle(static_cast<int>(value / rhs));
  }

  // Binary operators for float
  Angle operator+(float rhs) const {
    return Angle(static_cast<int>(value + rhs));
  }

  Angle operator-(float rhs) const {
    return Angle(static_cast<int>(value - rhs));
  }

  Angle operator*(float rhs) const {
    return Angle(static_cast<int>(value * rhs));
  }

  Angle operator/(float rhs) const {
    return Angle(static_cast<int>(value / rhs));
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

struct player_ship_type {
  SDL_FRect rect;
  SDL_FPoint gun_offset;
  SDL_Texture *texture;
};

struct projectile {
  SDL_FRect rect;
  SDL_Texture *texture;
  float speed;
  Angle angle = 0;
  bool guided;
  SDL_FRect *target;
};
