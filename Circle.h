//
// Created by Herman Karlsson on 2017-10-07.
//

#ifndef RAYTRACER_CIRCLE_H
#define RAYTRACER_CIRCLE_H

#include <utility>
#include "Vec3.h"

class Circle {
private:
  static constexpr double eps = 1e-10;
public:

  Vec3 center;
  double radius;
  Vec3 color;
  double reflection_index;

  static Circle Null;

  explicit Circle (Vec3 center = Vec3(), double radius = 1,
                   Vec3 color = Vec3(.7,.5,.9), double reflection_index = 0) :
      center(center), radius(radius), color(color), reflection_index(reflection_index) {}

  bool operator==(Circle);

  Vec3 first_intersection(Vec3, Vec3);
};

#endif //RAYTRACER_CIRCLE_H
