#include "obj_reader.h"
#include "./../../../../helper/buffer.h"

namespace opengl {

struct Group {
  Group(const ObjReader::Group& group);

  bool visible;
  std::string name;
  std::string material;
  Buffer<Eigen::Vector3f, GL_ARRAY_BUFFER> vertex_buffer;
  Buffer<Eigen::Vector3f, GL_ARRAY_BUFFER> normal_buffer;
};

struct ObjData {
  ObjData(const ObjReader::Data& data);

  Eigen::Vector3f max_v;
  std::map<std::string, Group> group_map;
  std::map<std::string, ObjReader::Material> mat_map;
};

}
