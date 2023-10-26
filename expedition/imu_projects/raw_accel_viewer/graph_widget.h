#pragma once

#include <memory>

#include <QMutex>
#include <QQueue>
#include <QOpenGLWidget>
#include <QOpenGLFunctions>

#include "../../../helper/buffer.h"
#include "../../../helper/texture.h"
#include "../../../helper/program.h"

#include "work_thread.h"

class GraphWidgetGL : public QOpenGLWidget, private QOpenGLFunctions {
  Q_OBJECT
public:
  GraphWidgetGL(QWidget* parent);
  void setMutexAndQueue(QMutex& m, QQueue<AccelData>& q);
  void clearRingBuffers();
private:
  void initializeGL() override;
  void paintGL() override;
  void resizeGL(int width, int height) override;

  void paintBackground();

  std::unique_ptr<opengl::Program> bg_program;
  std::unique_ptr<opengl::Texture2D> bg_texture;
  std::unique_ptr<opengl::Buffer<Eigen::Vector2f, GL_ARRAY_BUFFER>> bg_tex_coords;
  std::unique_ptr<opengl::Buffer<Eigen::Vector3f, GL_ARRAY_BUFFER>> bg_vertex_buffer;
  std::unique_ptr<opengl::Buffer<unsigned int, GL_ELEMENT_ARRAY_BUFFER>> bg_indices_buffer;

  std::unique_ptr<opengl::Program> program;
  std::unique_ptr<opengl::Buffer<float, GL_ARRAY_BUFFER>> x_buffer;
  std::unique_ptr<opengl::Buffer<float, GL_ARRAY_BUFFER>> y_buffer;
  std::unique_ptr<opengl::Buffer<float, GL_ARRAY_BUFFER>> z_buffer;
  std::unique_ptr<opengl::Buffer<float, GL_ARRAY_BUFFER>> t_buffer;

  size_t ring_index = 0;
  size_t ring_buffer_size = 0;
  float time_offset = 0.0f;
  bool clear_flag = false;

  QMutex* mutex = nullptr;
  QQueue<AccelData>* queue = nullptr;
};
