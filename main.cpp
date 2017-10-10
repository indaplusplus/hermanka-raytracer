#include <iostream>
#include <vector>
#include <fstream>
#include "Circle.h"

//#define DEBUG

int image_width = 1400;
int image_height = 900;
double image_distance_from_eye = 100;
double pixel_size = 1;
Vec3 eye_position(0, 0, 0);
Vec3 forward_direction(0, 0, 1);
Vec3 up_direction(0, 1, 0);
Vec3 right_direction(1, 0, 0);
Vec3 background_color(0,0,0);

const double shadow_intensity = 0.3;
const int MaxRecursionDepth = 15;

int n_circles;
int n_lights;
std::vector<Circle> circles;
std::vector<Vec3> light_sources;
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
      out << std::min(255, int(std::round(255 * pos.x))) << " "
          << std::min(255, int(std::round(255 * pos.y))) << " "
          << std::min(255, int(std::round(255 * pos.z)));
    }
    out << std::endl;
  }
}

void read_input() {
  //reads circles from an file on the format:
  //(everything integers)
  //image_width image_height
  //eye_position: x y z
  //number of light sources: m
  //m lines of light sources: x y z
  //number of circles: n
  //n lines of circle information: x y z radius r g b reflection_index
  std::ifstream in(input_file);
  if (!in.is_open()) {
    std::cerr << "couldn't find input file" << std::endl;
    exit(1);
  }

  //I'm going to assume that no one will give an invalid input
  in >> image_width >> image_height;
  output_map.assign(image_height, std::vector<Vec3>(image_width, Vec3()));

  in >> eye_position.x >> eye_position.y >> eye_position.z;

  in >> n_lights;
  for (int _ = 0; _ < n_lights; ++_) {
    int x, y, z;
    in >> x >> y >> z;
    light_sources.emplace_back(x, y ,z);
  }

  in >> n_circles;
  for (int _ = 0; _ < n_circles; ++_) {
    double x, y, z, radius;
    double r, g, b;
    double reflection_index;
    in >> x >> y >> z >> radius >> r >> g >> b >> reflection_index;
    circles.emplace_back(Vec3(x, y, z), radius, Vec3(r/255, g/255, b/255), reflection_index);
  }
}

void print_debug() {
  //Vec3 dir = (Vec3(100, 100, image_distance_from_eye) - eye_position).normalize();
  for (Circle &circle : circles) {
    std::cerr << circle.reflection_index << std::endl;
    //Vec3 intersect = circle.closest_intersection_with_ray(eye_position, dir);
    //std::cerr << intersect.x << " " << intersect.y << " " << intersect.z << std::endl;
  }
}

Vec3 trace(Vec3 origin, Vec3 direction, int depth, Circle prev) {
  Vec3 intersect_point = Vec3::Null;
  Circle intersect_circle = Circle::Null;
  for (Circle &circle : circles) {
    if (circle == prev) continue;
    Vec3 intersection = circle.first_intersection(origin, direction);
    if (!(intersection == Vec3::Null) && (intersect_point == Vec3::Null ||
         (intersect_point - origin).norm() > (intersection - origin).norm())) {
      intersect_point = intersection;
      intersect_circle = circle;
    }
  }

  if (intersect_point == Vec3::Null) return background_color;

  Vec3 normal = (intersect_point - intersect_circle.center).normalize();

  double light_intensity = 0;

  for (Vec3 &light : light_sources) {
    if ((intersect_circle.center - light).norm() < intersect_circle.radius) continue;
    bool blocked = 0;
    Vec3 light_dir = (light - intersect_point).normalize();
    for (Circle &circle : circles) {
      Vec3 intersection = circle.first_intersection(intersect_point, light_dir);
      if (!(intersection == Vec3::Null) &&
          (intersect_point - intersection).norm() < (intersect_point - light).norm()) {
        blocked = 1;
      }
    }

    if (blocked) continue;

    light_intensity += (normal * light_dir);
  }

  if (depth >= MaxRecursionDepth) return intersect_circle.color * light_intensity;

  Vec3 reflection_dir = (normal - direction * (normal * direction) * 2);

  return intersect_circle.color * light_intensity +
      trace(intersect_point + (reflection_dir*0.01), reflection_dir, depth + 1, intersect_circle) *
        intersect_circle.reflection_index;
}


Vec3 get_pixel_color(double row, double col) {
  double y = image_height/2.0 - row;
  double x = col - image_width/2.0;
  Vec3 pixel_position(x, y, 0); //= eye_position + forward_direction * image_distance_from_eye +
      //(right_direction * x + up_direction * y) * pixel_size;
  //std::cerr << pixel_position.x << " " << pixel_position.y << " " << pixel_position.z << std::endl;
  Vec3 direction = (pixel_position - eye_position).normalize();
  return trace(eye_position, direction, 0, Circle::Null);
}

void do_ray_trace() {
  for (int row = 0; row < image_height; ++row) {
    for (int col = 0; col < image_width; ++col) {
      output_map[row][col] = get_pixel_color(row, col);
    }
  }
}

int main(int argc, const char* argv[]) {
  std::vector <std::string> args(argv, argv + argc);
  if (argc < 3) {
    std::cerr << "Usage: "<< args[0] << " inputfile outputfile.ppm" << std::endl;
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