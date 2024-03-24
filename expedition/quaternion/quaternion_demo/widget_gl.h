#pragma once

#include <memory>

#include <QOpenGLWidget>
#include <QOpenGLFunctions>

#include "C:/Users/arjun/source/projects/helper/buffer.h"
#include "C:/Users/arjun/source/projects/helper/texture.h"
#include "C:/Users/arjun/source/projects/helper/program.h"

#include <Eigen/Core>

#include "quaternion.h"

class WidgetGL : public QOpenGLWidget, private QOpenGLFunctions {
  Q_OBJECT
public:
  WidgetGL(QWidget* parent);

  void set(const float theta, const Eigen::Vector3f& n);

private:
  void initializeGL() override;
  void paintGL() override;
  void resizeGL(int width, int height) override;

  Quaternion<float> q;

  std::unique_ptr<opengl::Program> program;
  std::unique_ptr<opengl::Buffer<Eigen::Vector3f, GL_ARRAY_BUFFER>> axis_buffer;
  std::unique_ptr<opengl::Buffer<Eigen::Vector3f, GL_ARRAY_BUFFER>> color_buffer;
};
