#pragma once

#include <memory>

#include <QOpenGLWidget>
#include <QOpenGLFunctions>

#include "./../../../helper/buffer.h"
#include "./../../../helper/texture.h"
#include "./../../../helper/program.h"
#include "./../obj_file/obj_data_gl.h"

#include <Eigen/Geometry>

class WidgetGL : public QOpenGLWidget, private QOpenGLFunctions {
  Q_OBJECT
public:
  WidgetGL(QWidget* parent);

  void set(const ObjReader::Data& obj_data);
  void setVisibility(const std::string& name, const bool visible);

private slots:
  void xAngleChanged(int angle);
  void yAngleChanged(int angle);
  void zAngleChanged(int angle);

private:
  void initializeGL() override;
  void paintGL() override;
  void resizeGL(int width, int height) override;

  float x_rad = 0.0f;
  float y_rad = 0.0f;
  float z_rad = 0.0f;

  std::unique_ptr<opengl::Program> program;
  std::unique_ptr<opengl::ObjData> obj_data;
};
