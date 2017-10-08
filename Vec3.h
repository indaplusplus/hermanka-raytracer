//
// Created by Herman Karlsson on 2017-10-07.
//

#ifndef RAYTRACER_VEC3_H
#define RAYTRACER_VEC3_H

#include <cmath>
#include <cstdlib>

class Vec3 {
private:
  static constexpr double eps = 1e-16;
public:
  double x;
  double y;
  double z;

  Vec3(double x = 0, double y = 0, double z = 0) : x(x), y(y), z(z) {}

  double norm();
  Vec3 normalize();

  Vec3 operator+ (Vec3);
  Vec3 operator- ();
  Vec3 operator- (Vec3);
  double operator* (Vec3);
  Vec3 operator* (double);
  Vec3 operator/ (double);
  bool operator== (Vec3);

  static Vec3 Null;

};

Vec3 min_dist_point(Vec3, Vec3, Vec3);


#endif //RAYTRACER_VEC3_H
