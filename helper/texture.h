#pragma once

#include <vector>
#include <string>
#include <type_traits>
#include <QOpenGLFunctions>

namespace opengl {

class Texture2D : private QOpenGLFunctions {
public:
  explicit Texture2D(GLint ifmt, GLenum fmt, GLenum type, GLsizei w, GLsizei h, const void* data = 0, bool pyramid = false);
  ~Texture2D();

  void load(const void* pData);

  //void load(const Buffer<>& buffer);
  //void load(const Buffer<>& buffer, size_t offset);

  void bind(GLenum tex);
  void unbind(GLenum tex);

  GLsizei width() const { return width_; };
  GLsizei height() const { return height_; };

  GLenum type() const { return type_; };
  GLenum format() const { return format_; };
  GLint internalFormat() const { return internal_fmt_; };
  bool isPyramid() const { return pyramid_; };

  GLuint operator()() const { return texture_; };

private:
  Texture2D() = delete;
  Texture2D(const Texture2D&) = delete;
  Texture2D& operator=(const Texture2D&) = delete;

  void init(const void* data);

  GLenum type_;
  bool pyramid_;
  GLenum format_;
  GLsizei width_;
  GLsizei height_;
  GLuint texture_;
  GLint internal_fmt_;
};

}
