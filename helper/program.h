#pragma once

#include <map>
#include <string>
#include <type_traits>
#include <QOpenGLFunctions>

#include <Eigen/Core>
#include <Eigen/Geometry>

#include "buffer.h"

namespace opengl {

class Program : private QOpenGLFunctions {
public:
  Program(const std::map<GLenum, std::string>& srcs);
  ~Program();

  void use();
  void release();

  void enableVertexAttribArray(const char* name);
  void disableVertexAttribArray(const char* name);

  void setUniform(const char* name, const float value);
  void setUniform(const char* name, const Eigen::Vector2f& v);
  void setUniform(const char* name, const Eigen::Vector3f& v);
  void setUniform(const char* name, const Eigen::Vector4f& v);
  void setUniform(const char* name, const Eigen::Quaternionf& q, const bool transose = false);
  void setUniform(const char* name, const Eigen::Matrix3f& matrix, const bool transpose = false);
  void setUniform(const char* name, const Eigen::Matrix4f& matrix, const bool transpose = false);
  void setUniform(const char* name, const Eigen::Isometry3f& transform, const bool transpose = false);

  template <typename T, int N>
  void setAttribute(const char* name, const std::vector<Eigen::Matrix<T, N, 1>>& attrib_data);

  template <typename T, int N, GLenum target>
  void setAttribute(const char* name, const Buffer<Eigen::Matrix<T, N, 1>, target>& buffer);

  template <typename T, GLenum target>
  void setAttribute(const char* name, const Buffer<T, target>& buffer);

  class Use {
  public:
      Use(Program& program);
      ~Use();
  private:
      Program& program;
  };

private:
  Program() = delete;
  Program(const Program&) = delete;
  Program& operator=(const Program&) = delete;

  void compile(const GLuint shader);

  template <typename T>
  GLenum type() const {
    if (std::is_same<T, int>::value) return GL_INT;
    if (std::is_same<T, float>::value) return GL_FLOAT;
    return GL_FLOAT;
  }

  GLuint program;
};

template <typename T, int N>
void Program::setAttribute(const char* name, const std::vector<Eigen::Matrix<T, N, 1>>& attrib_data) {
  const GLuint pos = glGetAttribLocation(program, name);
  glVertexAttribPointer(pos, N, type<T>(), GL_FALSE, sizeof(Eigen::Matrix<T, N, 1>), attrib_data.data());
}

template <typename T, int N, GLenum target>
void Program::setAttribute(const char* name, const Buffer<Eigen::Matrix<T, N, 1>, target>& buffer) {
  const GLuint pos = glGetAttribLocation(program, name);
  glBindBuffer(target, buffer());
  glVertexAttribPointer(pos, N, type<T>(), GL_FALSE, sizeof(Eigen::Matrix<T, N, 1>), 0);
  glBindBuffer(target, 0);
}

template <typename T, GLenum target>
void Program::setAttribute(const char* name, const Buffer<T, target>& buffer) {
  const GLuint pos = glGetAttribLocation(program, name);
  glBindBuffer(target, buffer());
  glVertexAttribPointer(pos, 1, type<T>(), GL_FALSE, sizeof(T), 0);
  glBindBuffer(target, 0);
}

}
