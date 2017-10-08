//
// Created by Herman Karlsson on 2017-10-07.
//

#include "Vec3.h"

Vec3 Vec3::Null(-1e20,-1e20,-1e20);

Vec3 Vec3::operator+(Vec3 other) { return Vec3(x + other.x, y + other.y, z + other.z); }
Vec3 Vec3::operator-() { return Vec3(-x, -y, -z); }
Vec3 Vec3::operator-(Vec3 other) { return *this + -other; }
double Vec3::operator*(Vec3 other) { return x * other.x + y * other.y + z * other.z; }
Vec3 Vec3::operator*(double k) { return Vec3(k * x, k * y, k * z); }
Vec3 Vec3::operator/(double k) { return *this * (1/k); }
bool Vec3::operator==(Vec3 other) {
  return std::abs(x - other.x) < eps && std::abs(y - other.y) < eps && std::abs(z - other.z) < eps;
}

double Vec3::norm() {
  return sqrt(x * x + y * y + z * z);
}

Vec3 Vec3::normalize() {
  return *this / this->norm();
}

Vec3 Vec3::reflect_as_normal(Vec3 normal) {
  return *this - normal * 2 * (*this * normal);
}

Vec3 min_dist_point(Vec3 to_1, Vec3 to_2, Vec3 from) {
  if (to_1 == Vec3::Null) return to_2;
  if (to_2 == Vec3::Null) return to_1;
  return (to_1 - from).norm() < (to_2 - from).norm() ? to_1 : to_2;
}
