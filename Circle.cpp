//
// Created by Herman Karlsson on 2017-10-07.
//

#include <iostream>
#include "Circle.h"

Circle Circle::Null(Vec3::Null, -1e20, Vec3::Null);

bool Circle::operator==(Circle other) {
  return center == other.center && std::abs(radius - other.radius) < eps && color == other.color;
}

//returns {Null, Null} if no intersection exists
std::pair<Vec3, Vec3> Circle::intersection_with_ray(Vec3 origin, Vec3 direction) {
  //Magic formula
  double part_1 = -(direction*(origin - center));
  double part_2 = pow(direction*(origin - center), 2) - pow((origin - center).norm(), 2) + pow(radius, 2);

  if (part_2 < eps) return {Vec3::Null, Vec3::Null};

  double d1 = (part_1 + sqrt(part_2));
  double d2 = (part_1 - sqrt(part_2));

  return {d1 < eps ? Vec3::Null : origin + direction * d1,
          d2 < eps ? Vec3::Null : origin + direction * d2};
}

Vec3 Circle::closest_intersection_with_ray(Vec3 origin, Vec3 direction) {
  auto intersections = intersection_with_ray(origin, direction);
  return min_dist_point(intersections.first, intersections.second, origin);
}

Vec3 Circle::first_intersection(Vec3 origin, Vec3 direction) {
  double part_1 = -(direction*(origin - center));
  double part_2 = pow(direction*(origin - center), 2) - pow((origin - center).norm(), 2) + pow(radius, 2);

  if (part_2 < eps) return Vec3::Null;

  double d1 = (part_1 + sqrt(part_2));
  double d2 = (part_1 - sqrt(part_2));

  if (d2 > eps) return origin + direction * d2;
  if (d1 > eps) return origin + direction * d1;
  return Vec3::Null;
}

