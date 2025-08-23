#include "const.hpp"
#include <SDL3/SDL.h>
#include <SDL3_image/SDL_image.h>
#include <cmath>
#include <iostream>
#include <optional>

#ifndef TYPES_NS
#define TYPES_NS

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

  // Type conversion
  operator int() const { return value; }

  // Stream output
  friend std::ostream &operator<<(std::ostream &os, const Angle &a) {
    return os << a.value;
  }
};

struct basic_cords {
  float x;
  float y;
};

struct Entity {
  SDL_FRect position;
  SDL_FRect Tposition;
  bool gotoT = false;
  SDL_Texture *texture;
  SDL_Rect srcRect;
  float speed;
  Angle angle = 0;

  basic_cords Central_position(std::optional<float> x = std::nullopt,
                               std::optional<float> y = std::nullopt) {
    if (x != std::nullopt) {
      position.x = *x - position.w / 2;
    }
    if (y != std::nullopt) {
      position.y = *y - position.h / 2;
    }

    return {position.x + position.w / 2, position.y + position.h / 2};
  }
};

struct sdl_session {
  SDL_Window *window;
  SDL_Renderer *renderer;
};
#endif