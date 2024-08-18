#include "obj_reader.h"

#include <fstream>
#include <iostream>
#include <filesystem>

#include <Eigen/Geometry>

namespace ObjReader {

using Eigen::Vector2f;
using Eigen::Vector3f;
using Eigen::Vector3d;

void parseFaces(const char* str, std::vector<unsigned> &faces) {
  std::string s;
  std::vector<unsigned> indices;
  while (*str != '\0') {
    if (*str == ' ') {
      if (!s.empty()) {
        indices.push_back(std::stoi(s));
      }
      s.clear();
    } else {
      s.push_back(*str);
    }
    ++str;
  }
  if (!s.empty()) {
    indices.push_back(std::stoi(s));
  }
  if (indices.size() > 2) {
    for (int i = 0; i < static_cast<int>(indices.size()-2); i++) {
      faces.push_back(indices[0]-1);
      for (size_t j = i+1; j < i+3; j++) {
        faces.push_back(indices[j]-1);
      }
    }
  }
}

std::list<ObjReader::Material> parseMaterialFile(const std::string& file_path) {
  std::ifstream file(file_path);
  if (!file.is_open()) {
    std::cout << "File: " << file_path << " failed\n";
    return std::list<ObjReader::Material>();
  }
  std::string line;
  line.reserve(200);
  std::list<Material> materials;

  while (std::getline(file, line)) {
    if (line.empty()) {
      continue;
    }
    if (line[0] == '#') {
      continue;
    }
    if (line.find("newmtl") == 0) {
      Material mat;
      mat.name = line.substr(7);
      materials.emplace_back(std::move(mat));
      continue;
    } else if (line.find("Ka") == 0) {
      auto& mat = materials.back();
      size_t count = sscanf(line.c_str(), "Ka %f %f %f", &mat.Ka.x(), &mat.Ka.y(), &mat.Ka.z());
      if (count != 3) {
        std::cout << "Failed to parse Ka";
        exit(0);
      }
      continue;
    } else if (line.find("Kd") == 0) {
      auto& mat = materials.back();
      size_t count = sscanf(line.c_str(), "Kd %f %f %f", &mat.Kd.x(), &mat.Kd.y(), &mat.Kd.z());
      if (count != 3) {
        std::cout << "Failed to parse Kd";
        exit(0);
      }
      continue;      
    } else if (line.find("illum") == 0) {
      auto& mat = materials.back();
      size_t count = sscanf(line.c_str(), "illum %f", &mat.illum);
      if (count != 1) {
        std::cout << "Failed to parse illum";
        exit(0);
      }
      continue;      
    } else if (line[0] == 'd') {
      auto& mat = materials.back();
      size_t count = sscanf(line.c_str(), "d %f", &mat.d);
      if (count != 1) {
        std::cout << "Failed to parse d";
        exit(0);
      }
      continue;      
    }
  }
  return materials;
}

std::unique_ptr<Data> load(const std::string& file_path) {
  std::filesystem::path fpath(file_path);
  const std::string file_dir_path = fpath.parent_path().string();
  auto obj_data = std::make_unique<Data>();

  std::ifstream file(file_path);
  if (!file.is_open()) {
    return obj_data;
  }

  std::string line;
  line.reserve(200);

  std::vector<Vector3f> vertex;

  Vector3d sum = Vector3d::Zero();

  while (std::getline(file, line)) {
    if (line.size() < 2) {
      continue;
    }
    if (line[0] == '#') {
      continue;
    }
    if (line[0] == 'v' && line[1] == ' ') {
      Vector3f v;
      sscanf(line.c_str(), "v %f %f %f", &v.x(), &v.y(), &v.z());
      vertex.emplace_back(v);
      obj_data->max_v = obj_data->max_v.cwiseMax(v.cwiseAbs());
      sum += v.cast<double>();
    } else if (line[0] == 'f' && line[1] == ' ') {
      std::vector<unsigned> vi;
      parseFaces(line.c_str()+2, vi);
      auto& group = obj_data->groups.back();
      for (size_t i = 0; i < vi.size(); i += 3) {
        const Vector3f A = vertex[vi[i]];
        const Vector3f B = vertex[vi[i+1]];
        const Vector3f C = vertex[vi[i+2]];
        const Vector3f n = (B-A).cross(C-A).normalized();
        group.vertex.push_back(A);
        group.vertex.push_back(B);
        group.vertex.push_back(C);
        group.normal.push_back(n);
        group.normal.push_back(n);
        group.normal.push_back(n);
      }
    } else if (line[0] == 'o') {
      obj_data->name = line.substr(2);
    } else if (line[0] == 'g') {
      Group group;
      group.name = line.substr(2);
      obj_data->groups.emplace_back(std::move(group));
    } else if (line.find("mtllib") == 0) {
      const std::string file_path = std::filesystem::path(file_dir_path).append(line.substr(7)).string();
      obj_data->materials = parseMaterialFile(file_path);
    } else if (line.find("usemtl") == 0) {
      auto& group = obj_data->groups.back();
      group.material = line.substr(7);
    } else {
      std::cout << line << "\n";
    }
  }
  const Vector3f center = (sum/vertex.size()).cast<float>();
  for (auto& group : obj_data->groups) {
    for (auto& v : group.vertex) {
      v -= center;
    }
  }
  return obj_data;
}

}
