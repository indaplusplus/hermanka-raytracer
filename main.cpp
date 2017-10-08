#include <iostream>
#include <vector>
#include <fstream>
#include "Circle.h"

//#define DEBUG

double image_width = 4000;
double image_height = 2800;
const double ImageZ = 0;
Vec3 eye_origin(image_width/2, image_height/2, -1000);
Vec3 light_source(100, 100, -2200);
const double shadow_intensity = 0.3;
const int MaxRecursionDepth = 2;

int n_circles;
std::vector<Circle> circles;
std::string input_file;
std::string output_file;

std::vector<std::vector<Vec3>> output_map(image_height, std::vector<Vec3>(image_width));

void write_image() {
  std::ofstream out(output_file);
  out << "P3" << std::endl;
  out << image_width << " " << image_height << std::endl;
  out << 255 << std::endl;

  for (auto & row : output_map) {
    for (int col = 0; col < image_width; ++col) {
      Vec3& pos = row[col];
      if (col > 0) out << " ";
      out << int(std::round(255 * pos.x)) << " "
          << int(std::round(255 * pos.y)) << " "
          << int(std::round(255 * pos.z));
    }
    out << std::endl;
  }
}

void read_input() {
  //reads circles from an file on the format:
  //(everything integers)
  //image_width image_height
  //light_source: x y z
  //eye_origin: x y z
  //number of circles: n
  //n rows of circle information: x y z radius r g b reflection_index
  std::ifstream in(input_file);
  if (!in.is_open()) {
    std::cerr << "couldn't find input file" << std::endl;
    exit(1);
  }

  //I'm going to assume that no one will give an invalid input
  in >> image_width >> image_height;
  output_map.assign(image_height, std::vector<Vec3>(image_width, Vec3()));
  in >> light_source.x >> light_source.y >> light_source.z;
  in >> eye_origin.x >> eye_origin.y >> eye_origin.z;

  in >> n_circles;
  for (int _ = 0; _ < n_circles; ++_) {
    double x, y, z, radius;
    double r, g, b;
    double reflection_index;
    in >> x >> y >> z >> radius >> r >> g >> b >> reflection_index;
    circles.emplace_back(Vec3(x, y, z), radius, Vec3(r/255, g/255, b/255));
  }
}

void print_debug() {
  Vec3 dir = (Vec3(100, 100, ImageZ) - eye_origin).normalize();
  for (Circle &circle : circles) {
    Vec3 intersect = circle.closest_intersection_with_ray(eye_origin, dir);
    std::cerr << intersect.x << " " << intersect.y << " " << intersect.z << std::endl;
  }
}


Vec3 trace_reflection(Vec3 origin, Vec3 direction, int depth) {
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

  Vec3 light_dir = (light_source - bounce_point).normalize();
  bool is_in_shadow = false;

  for (Circle &circle : circles) {
    auto intersection = circle.closest_intersection_with_ray(bounce_point, light_dir);
    if (intersection == Vec3::Null) continue;
    if ((intersection - bounce_point).norm() > (light_source - bounce_point).norm()) continue;
    is_in_shadow = true;
  }

  Vec3 normal = (bounce_point - best_circle.center).normalize();
  Vec3 color_from_light;

  if (is_in_shadow) {
    color_from_light = best_circle.color * shadow_intensity;
  } else {
    color_from_light = best_circle.color * (normal * light_dir + 1)/2;
  }

  if (depth >= MaxRecursionDepth || best_circle.reflection_index < 1e-10)
    return color_from_light;

  std::cerr << "lol" << std::endl;
  Vec3 reflection_vec = direction.reflect_as_normal(normal);
  return color_from_light * (1 - best_circle.reflection_index) +
         trace_reflection(origin, reflection_vec, depth+1) * best_circle.reflection_index;
}

Vec3 get_pixel_color(double x, double y) {
  Vec3 origin = eye_origin;
  Vec3 direction = (Vec3(x, y, ImageZ) - origin).normalize();
  return trace_reflection(origin, direction, 0);
}

void do_ray_trace() {
  for (int row = 0; row < image_height; ++row) {
    for (int col = 0; col < image_width; ++col) {
      output_map[image_height - row - 1][col] = get_pixel_color(col, row);
    }
  }
}

int main(int argc, const char* argv[]) {
  std::vector <std::string> args(argv, argv + argc);
  if (argc < 3) {
    std::cerr <<"Usage: "<< args[0] << " inputfile outputfile.ppm" << std::endl;
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