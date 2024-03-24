#include "obj_data_gl.h"

namespace opengl {

Group::Group(const ObjReader::Group& group)
  : visible(true),
    name(group.name),
    material(group.material),
    vertex_buffer(GL_STATIC_DRAW, group.vertex),
    normal_buffer(GL_STATIC_DRAW, group.normal) {}

ObjData::ObjData(const ObjReader::Data& data)
  : max_v(data.max_v) {
  for (auto& mat : data.materials) {
    mat_map.emplace(mat.name, mat);
  }
  for (auto& group : data.groups) {
    group_map.emplace(group.name, group);
  }
}

}
