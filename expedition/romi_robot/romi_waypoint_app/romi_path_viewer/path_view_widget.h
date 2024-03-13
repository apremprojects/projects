#pragma once

#include <memory>

#include <QMutex>
#include <QQueue>
#include <QTimer>
#include <QOpenGLWidget>
#include <QOpenGLFunctions>

#include "C:/Users/arjun/source/projects/helper/buffer.h"
#include "C:/Users/arjun/source/projects/helper/texture.h"
#include "C:/Users/arjun/source/projects/helper/program.h"

class PathViewWidget : public QOpenGLWidget, private QOpenGLFunctions {
  Q_OBJECT
public:
  PathViewWidget(QWidget* parent);

  void clear();
  void add(const float yaw, const std::vector<Eigen::Vector2f>& points);

signals:
  void newWayPoint(const int, const int);

private:
  void initializeGL() override;
  void paintGL() override;
  void resizeGL(int width, int height) override;
  void keyPressEvent(QKeyEvent* event) override;
  void mousePressEvent(QMouseEvent* event) override;

  void paintBackground();

  float scale_factor = 1.0f;

  Eigen::Matrix3f matrix = Eigen::Matrix3f::Identity();

  std::unique_ptr<opengl::Program> bg_program;
  std::unique_ptr<opengl::Texture2D> bg_texture;
  std::unique_ptr<opengl::Buffer<Eigen::Vector2f, GL_ARRAY_BUFFER>> bg_tex_coords;
  std::unique_ptr<opengl::Buffer<Eigen::Vector3f, GL_ARRAY_BUFFER>> bg_vertex_buffer;
  std::unique_ptr<opengl::Buffer<unsigned int, GL_ELEMENT_ARRAY_BUFFER>> bg_indices_buffer;

  std::unique_ptr<opengl::Program> program;
  std::unique_ptr<opengl::Buffer<Eigen::Vector2f, GL_ARRAY_BUFFER>> wp_buffer;
  std::unique_ptr<opengl::Buffer<Eigen::Vector2f, GL_ARRAY_BUFFER>> traj_buffer;
  std::unique_ptr<opengl::Buffer<Eigen::Vector2f, GL_ARRAY_BUFFER>> head_buffer;
};
