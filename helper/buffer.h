#pragma once

#include <vector>
#include <type_traits>

#include <Eigen/Core>
#include <Eigen/Geometry>
#include <QOpenGLFunctions>

namespace opengl {

template <typename T, GLenum target>
class Buffer : private QOpenGLFunctions {
public:
  Buffer(const GLenum usage, const size_t max_items);
  Buffer(const GLenum usage, const std::vector<T>& data);
  ~Buffer();

  size_t size() const;
  size_t capacity() const { return capacity_; };

  void bind();
  void unbind();

  void load(const std::vector<T>& data);
  void append(const std::vector<T>& data);
  void append(const T* data, const size_t count);

  void load(const size_t start, const size_t size, const T* data);
  void clear();

  GLuint operator()() const { return buffer_; }

private:
  Buffer() = delete;
  Buffer(const Buffer&) = delete;

  size_t size_;
  mutable GLuint buffer_;
  const size_t capacity_;
};

template <typename T, GLenum target>
Buffer<T, target>::Buffer(const GLenum usage, const std::vector<T>& data)
  : size_(data.size()),
    capacity_(data.size()) {
  initializeOpenGLFunctions();
  glGenBuffers(1, &buffer_);
  glBindBuffer(target, buffer_);
  glBufferData(target, sizeof(T) * data.size(), data.data(), usage);
  glBindBuffer(target, 0);
}

template <typename T, GLenum target>
Buffer<T, target>::Buffer(const GLenum usage, const size_t max_items)
  : size_(0),
    capacity_(max_items) {
  initializeOpenGLFunctions();
  glGenBuffers(1, &buffer_);
  glBindBuffer(target, buffer_);
  glBufferData(target, sizeof(T) * max_items, nullptr, usage);
  glBindBuffer(target, 0);
}

template <typename T, GLenum target>
Buffer<T, target>::~Buffer() {
  glDeleteBuffers(1, &buffer_);
}

template <typename T, GLenum target>
size_t Buffer<T, target>::size() const { return size_; }

template <typename T, GLenum target>
void Buffer<T, target>::bind() {
  glBindBuffer(target, buffer_);
}

template <typename T, GLenum target>
void Buffer<T, target>::unbind() {
  glBindBuffer(target, 0);
}

template <typename T, GLenum target>
void Buffer<T, target>::load(const std::vector<T>& data) {
  glBindBuffer(target, buffer_);
  glBufferSubData(target, 0, data.size() * sizeof(T), data.data());
  glBindBuffer(target, 0);
  size_ = data.size();
}

template <typename T, GLenum target>
void Buffer<T, target>::append(const std::vector<T>& data) {
  glBindBuffer(target, buffer_);
  glBufferSubData(target, size_ * sizeof(T), data.size() * sizeof(T), data.data());
  glBindBuffer(target, 0);
  size_ += data.size();
}

template <typename T, GLenum target>
void Buffer<T, target>::append(const T* data, const size_t count) {
    glBindBuffer(target, buffer_);
    glBufferSubData(target, size_ * sizeof(T), count * sizeof(T), data);
    glBindBuffer(target, 0);
    size_ += count;
}

template <typename T, GLenum target>
void Buffer<T, target>::load(const size_t start, const size_t size, const T* data) {
  glBindBuffer(target, buffer_);
  glBufferSubData(target, start * sizeof(T), size * sizeof(T), data);
  glBindBuffer(target, 0);
}

template <typename T, GLenum target>
void Buffer<T, target>::clear() {
    size_ = 0;
}

}
