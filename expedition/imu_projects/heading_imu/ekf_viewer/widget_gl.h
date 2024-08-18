#pragma once

#include <memory>

#include <QMutex>
#include <QQueue>
#include <QTimer>
#include <QOpenGLWidget>
#include <QOpenGLFunctions>

#include "./../../../../helper/buffer.h"
#include "./../../../../helper/texture.h"
#include "./../../../../helper/program.h"
#include "./../obj_reader/obj_data_gl.h"

class WidgetGL : public QOpenGLWidget, private QOpenGLFunctions {
  Q_OBJECT
public:
  WidgetGL(QWidget* parent);

  void add(const Eigen::Quaternionf& q);

private:
  void initializeGL() override;
  void paintGL() override;
  void resizeGL(int width, int height) override;

  Eigen::Quaternionf q = Eigen::Quaternionf::Identity();

  std::unique_ptr<opengl::Program> program;
  std::unique_ptr<opengl::ObjData> obj_data;
};
