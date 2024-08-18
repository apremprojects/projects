#include "widget_gl.h"

#include <QImage>
#include <QPainter>
#include <QDebug>
#include <QKeyEvent>

#include <array>

using namespace Eigen;
using namespace opengl;

static const char vertex_shader[] = R"(
    #version 120
    attribute vec3 vertex;
    attribute vec3 normal;
    uniform vec3 a_color;
    uniform mat3 rot;
    varying vec3 out_coord;
    varying vec3 out_normal;
    varying vec3 out_color;
    void main(void)
    {
        out_coord = rot*vertex;
        out_color = a_color;
        out_normal = rot*normal;
        gl_Position = vec4(out_coord, 1.0f);
    })";

static const char fragment_shader[] = R"(
    #version 120
    varying vec3 out_coord;
    varying vec3 out_normal;
    varying vec3 out_color;
    uniform vec3 d_color;
    uniform vec3 light_pos = vec3(0.0, 0.0, -10.0f);
    void main(void)
    {
        vec3 light_dir = normalize(light_pos - out_coord);
        float li = clamp(dot(normalize(out_normal), light_dir), 0.0f, 1.0f);
        vec3 color = out_color + (0.3+li)*d_color;
        gl_FragColor = vec4(color, 1.0f);
    })";

WidgetGL::WidgetGL(QWidget* parent)
  : QOpenGLWidget(parent) {
  QSurfaceFormat format;
  format.setRenderableType(QSurfaceFormat::OpenGL);
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
  const std::string path = "./../../../heading_imu/enclosure/enclosure_imu_board.obj";
  auto data = ObjReader::load(path);
  qDebug() << "HERE -> " << data->groups.size() << "\n";
  obj_data = std::make_unique<opengl::ObjData>(*data);
}

void WidgetGL::resizeGL(int width, int height) {
  glViewport(0, 0, width, height);
}

void WidgetGL::paintGL() {
  const float sf = 1.0f / (2.0f * obj_data->max_v.maxCoeff());
  const Matrix3f rot = q.toRotationMatrix() * sf;
  for (const auto& item : obj_data->group_map) {
    auto& group = item.second;
    const auto itr = obj_data->mat_map.find(group.material);
    const auto material = (itr == obj_data->mat_map.end()) ? ObjReader::Material() : itr->second;

    Program::Use use(*program);
    program->setUniform("rot", rot);
    program->setUniform("a_color", material.Ka);
    program->setUniform("d_color", material.Kd);

    program->setAttribute("vertex", group.vertex_buffer);
    program->setAttribute("normal", group.normal_buffer);
    program->enableVertexAttribArray("vertex");
    program->enableVertexAttribArray("normal");
    glDrawArrays(GL_TRIANGLES, 0, group.vertex_buffer.size());
    program->disableVertexAttribArray("vertex");
    program->disableVertexAttribArray("normal");
  }
}

void WidgetGL::add(const Eigen::Quaternionf& new_q) {
  makeCurrent();
  q = new_q;
  doneCurrent();
  repaint();
}

