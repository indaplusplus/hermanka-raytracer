#include <iostream>
#include <vector>
#include <fstream>
#include "Circle.h"

//#define DEBUG

const double OutputWidth = 4000;
const double OutputHeight = 2800;
const double ImageZ = 0;
const Vec3 EyeOrigin(OutputWidth/2, OutputHeight/2, -1000);
Vec3 light_source(100, 100, -2200);
const double light_intensity = 0.7;
const double shadow_intensity = 0.4;

std::vector<Circle> circles;
std::string input_file;
std::string output_file;

std::vector<std::vector<Vec3>> output_map(OutputHeight, std::vector<Vec3>(OutputWidth));

void write_image() {
  std::ofstream out(output_file);
  out << "P3" << std::endl;
  out << OutputWidth << " " << OutputHeight << std::endl;
  out << 255 << std::endl;

  for (auto & row : output_map) {
    for (int col = 0; col < OutputWidth; ++col) {
      Vec3& pos = row[col];
      if (col > 0) out << " ";
      out << int(255 * pos.x) << " " << int(255 * pos.y) << " " << int(255 * pos.z);
    }
    out << std::endl;
  }
}

void read_input() {
  //reads circles from an file on the format
  //first row: number of circles - n
  //n rows: x, y, z, r, [rgb] [transparency]
  std::ifstream in(input_file);
  if (!in.is_open()) {
    //TODO: some nice error message
    exit(1);
  }

  //I'm going to assume that no one will give an invalid input
  int n_circles;
  in >> n_circles;
  for (int _ = 0; _ < n_circles; ++_) {
    double x, y, z, radius;
    in >> x >> y >> z >> radius;
    circles.emplace_back(Vec3(x, y, z), radius);
  }
}

void print_debug() {
  Vec3 dir = (Vec3(100, 100, ImageZ) - EyeOrigin).normalize();
  for (Circle &circle : circles) {
    Vec3 intersect = circle.closest_intersection_with_ray(EyeOrigin, dir);
    std::cerr << intersect.x <<" " << intersect.y << " " << intersect.z << std::endl;
  }
}


Vec3 get_pixel_color(double y, double x) {
  Vec3 origin = EyeOrigin;
  Vec3 direction = (Vec3(x, y, ImageZ) - origin).normalize();
  Vec3 bounce_point = Vec3::Null;
  Circle best_circle = Circle::Null;
  double best_dist = 1e20;

  for (Circle &circle : circles) {
    auto intersection = circle.closest_intersection_with_ray(origin, direction);
    if (intersection == Vec3::Null) continue;
    double dist = (intersection - origin).norm();
    if (dist < best_dist) {
      bounce_point = intersection;
      best_circle = circle;
      best_dist = dist;
    }
  }

  if (best_circle == Circle::Null) return Vec3(0,0,0);

  //std::cerr << best_circle.radius << std::endl;

  Vec3 light_dir = (light_source - bounce_point).normalize();
  bool is_in_shadow = false;

  for (Circle &circle : circles) {
    auto intersection = circle.closest_intersection_with_ray(bounce_point, light_dir);
    if (intersection == Vec3::Null) continue;
    is_in_shadow = true;
  }

  if (is_in_shadow) {
    return best_circle.color * shadow_intensity;
  } else {
    Vec3 normal = (bounce_point - best_circle.center).normalize();
    return best_circle.color * (normal * light_dir + 1)/2;
  }
}

void do_ray_trace() {
  for (int row = 0; row < OutputHeight; ++row) {
    for (int col = 0; col < OutputWidth; ++col) {
      output_map[row][col] = get_pixel_color(row, col);
    }
  }
}

int main(int argc, const char* argv[]) {
  std::vector <std::string> args(argv, argv + argc);
  if (argc < 3) {
    //TODO: some nice error message
    exit(1);
  }

  input_file = args[1];
  output_file = args[2];

  read_input();
  do_ray_trace();
  write_image();

#ifdef DEBUG
  print_debug();
#endif
}