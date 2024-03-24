#include "widget_gl.h"

#include <QDebug>
#include <QKeyEvent>
#include <QMouseEvent>

#include <array>
#include <iostream>

using namespace Eigen;
using namespace opengl;

static const char vertex_shader[] = R"(
    #version 120
    attribute vec3 vertex;
    attribute vec3 normal;
    uniform mat3 rot;
    varying vec3 out_coord;
    varying vec3 out_normal;
    void main(void)
    {
        out_coord = rot*vertex;
        out_normal = rot*normal;
        gl_Position = vec4(out_coord, 1.0f);
    })";

static const char fragment_shader[] = R"(
    #version 120
    varying vec3 out_coord;
    varying vec3 out_normal;
    uniform vec3 d_color;
    uniform vec3 light_pos = vec3(0.0, 0.0, -10.0f);
    void main(void)
    {
        vec3 light_dir = normalize(light_pos - out_coord);
        float li = clamp(dot(normalize(out_normal), light_dir), 0.0f, 1.0f);
        vec3 color = (0.3 + li)*d_color;
        gl_FragColor = vec4(color, 1.0f);
    })";

#define DEG_TO_RADIAN(theta) ((4.0f * std::atan(1.0f) / 180.0f) * static_cast<float>(theta))

WidgetGL::WidgetGL(QWidget* parent)
  : QOpenGLWidget(parent) {
  QSurfaceFormat format;
  format.setRenderableType(QSurfaceFormat::OpenGL);
  format.setDepthBufferSize(32);
  setFormat(format);
}

void WidgetGL::initializeGL() {
  initializeOpenGLFunctions();
  glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

  glEnable(GL_DEPTH_TEST);
  glDepthFunc(GL_LESS);
  glDepthMask(GL_TRUE);

  static const std::map<GLenum, std::string> shaders = {
      { GL_VERTEX_SHADER, vertex_shader },
      { GL_FRAGMENT_SHADER, fragment_shader} };

  program = std::make_unique<Program>(shaders);
}

void WidgetGL::resizeGL(int width, int height) {
  glViewport(0, 0, width, height);
}

void WidgetGL::paintGL() {
  glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
  if (!obj_data) {
    return;
  }
  
  const float sf = 1.0f/(2.0f*obj_data->max_v.maxCoeff());
  Matrix3f rot(AngleAxisf(x_rad, Vector3f::UnitX()) *
               AngleAxisf(y_rad, Vector3f::UnitY()) *
               AngleAxisf(z_rad, Vector3f::UnitZ()));
  rot *= sf;
  for (const auto& item : obj_data->group_map) {
    if (!item.second.visible) {
      continue;
    }
    const auto& group = item.second;
    const auto itr = obj_data->mat_map.find(group.material);
    const auto material = (itr == obj_data->mat_map.end())?ObjReader::Material():itr->second;

    // Render group
    Program::Use use(*program);
    program->setUniform("rot", rot);
    program->setUniform("d_color", material.Kd);
    qDebug() << material.Kd.x() << ", " << material.Kd.y() << ", " << material.Kd.z() << "\n";
    program->setAttribute("vertex", group.vertex_buffer);
    program->setAttribute("normal", group.normal_buffer);
    program->enableVertexAttribArray("vertex");
    program->enableVertexAttribArray("normal");
    glDrawArrays(GL_TRIANGLES, 0, group.vertex_buffer.size());
    program->disableVertexAttribArray("vertex");
    program->disableVertexAttribArray("normal");
  }
}

void WidgetGL::set(const ObjReader::Data& data) {
  makeCurrent();
  obj_data = std::make_unique<opengl::ObjData>(data);
  doneCurrent();
  repaint();
}

void WidgetGL::setVisibility(const std::string& name, const bool visible) {
  auto itr = obj_data->group_map.find(name);
  if (itr != obj_data->group_map.end()) {
    itr->second.visible = visible;
    repaint();
  }
}

void WidgetGL::xAngleChanged(int angle) {
  x_rad = DEG_TO_RADIAN(angle);
  repaint();
}

void WidgetGL::yAngleChanged(int angle) {
  y_rad = DEG_TO_RADIAN(angle);
  repaint();
}

void WidgetGL::zAngleChanged(int angle) {
  z_rad = DEG_TO_RADIAN(angle);
  repaint();
}
