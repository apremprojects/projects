#include "texture.h"
#include <glog/logging.h>

namespace opengl {

Texture2D::Texture2D(GLint ifmt, GLenum fmt, GLenum type, GLsizei w, GLsizei h, const void* data, bool pyramid)
    : type_(type),
      pyramid_(pyramid),
      format_(fmt),
      width_(w),
      height_(h),
      texture_(0),
      internal_fmt_(ifmt) {
  initializeOpenGLFunctions();
  glGenTextures(1, &texture_);
  init(data);
}

Texture2D::~Texture2D() {
  glDeleteTextures(1, &texture_);
}

void Texture2D::init(const void* data) {
  glActiveTexture(GL_TEXTURE0);
  glBindTexture(GL_TEXTURE_2D, texture_);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, (pyramid_ == false) ? GL_LINEAR : GL_LINEAR_MIPMAP_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);
  glTexImage2D(GL_TEXTURE_2D, 0, internal_fmt_, width_, height_, 0, format_, type_, data);
  if (pyramid_) {
    glGenerateMipmap(GL_TEXTURE_2D);
  }
  glBindTexture(GL_TEXTURE_2D, 0);
}

void Texture2D::bind(GLenum tex) {
  glActiveTexture(tex);
  glBindTexture(GL_TEXTURE_2D, texture_);
}

void Texture2D::unbind(GLenum tex) {
  glActiveTexture(tex);
  glBindTexture(GL_TEXTURE_2D, 0);
}

void Texture2D::load(const void* data) {
  glActiveTexture(GL_TEXTURE0);
  glBindTexture(GL_TEXTURE_2D, texture_);
  glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, width_, height_, format_, type_, data);
  if (pyramid_) {
    glGenerateMipmap(GL_TEXTURE_2D);
  }
  glBindTexture(GL_TEXTURE_2D, 0);
}

/*void Texture2D::load(const Ogl::Buffer2D& buffer) {
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texture_);
    buffer.bind();
    glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, width_, height_, format_, type_, 0);
    if (pyramid_) {
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    buffer.unbind();
    glBindTexture(GL_TEXTURE_2D, 0);
}

void Texture2D::load(const Ogl::Buffer2D& buffer, size_t offset) {
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texture_);
    buffer.bind();
    glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, width_, height_, format_, type_, (const GLvoid*)offset);
    if (pyramid) {
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    buffer.unbind();
    glBindTexture(GL_TEXTURE_2D, 0);
}*/

}
