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

  static Circle Null;

  explicit Circle (Vec3 center = Vec3(), double radius = 1, Vec3 color = Vec3(.7,.5,.9)) :
      center(center), radius(radius), color(color) {}

  bool operator==(Circle);

  std::pair<Vec3, Vec3> intersection_with_ray(Vec3, Vec3);

  Vec3 closest_intersection_with_ray(Vec3, Vec3);
};

#endif //RAYTRACER_CIRCLE_H
