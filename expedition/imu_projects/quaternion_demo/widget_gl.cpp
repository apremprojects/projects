#include "widget_gl.h"

#include <QDebug>
#include <QKeyEvent>
#include <QMouseEvent>

#include <array>
#include <iostream>

using namespace opengl;

using Eigen::Vector3f;

static const char vertex_shader[] = R"(
    #version 120
    attribute vec3 vertex;
    attribute vec3 color;
    varying vec3 out_color;
    uniform vec4 q;
    vec4 inverse_q(vec4 q) {
      vec4 ans = vec4(-q.x, -q.y, -q.z, q.w);
      return ans / (q.w * q.w + q.x * q.x + q.y * q.y + q.z * q.z);
    }
    vec4 mult_q(vec4 q1, vec4 q2) {
	  vec4 q;
      q.w = q1.w * q2.w - q1.x * q2.x - q1.y * q2.y - q1.z * q2.z;
      q.x = q1.w * q2.x + q1.x * q2.w + q1.y * q2.z - q1.z * q2.y;
	  q.y = q1.w * q2.y - q1.x * q2.z + q1.y * q2.w + q1.z * q2.x;
	  q.z = q1.w * q2.z + q1.x * q2.y - q1.y * q2.x + q1.z * q2.w;
      return q;
    }
    void main(void)
    {
        out_color = color;
        vec4 p = mult_q(mult_q(q, vec4(vertex, 0)), inverse_q(q));
        gl_Position = vec4(p.x, p.y, p.z, 1.0f);
    })";

static const char fragment_shader[] = R"(
    #version 120
    varying vec3 out_color;
    void main(void)
    {
        gl_FragColor = vec4(out_color, 1.0f);
    })";

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
  const std::vector<Vector3f> axis_points = {{0.0f, 0.0f, 0.0f}, {0.75f, 0.0f, 0.0f}, 
                                             {0.0f, 0.0f, 0.0f}, {0.0f, 0.75f, 0.0f}, 
                                             {0.0f, 0.0f, 0.0f}, {0.0f, 0.0f, 0.75f}};
  axis_buffer = std::make_unique<Buffer<Vector3f, GL_ARRAY_BUFFER>>(GL_STATIC_DRAW, axis_points);
  const std::vector<Vector3f> colors = {{1.0f, 0.0f, 0.0f}, {1.0f, 0.0f, 0.0f}, 
                                        {0.0f, 1.0f, 0.0f}, {0.0f, 1.0f, 0.0f}, 
                                        {0.0f, 0.0f, 1.0f}, {0.0f, 0.0f, 1.0f}};
  color_buffer = std::make_unique<Buffer<Vector3f, GL_ARRAY_BUFFER>>(GL_STATIC_DRAW, colors);
}

void WidgetGL::resizeGL(int width, int height) {
  glViewport(0, 0, width, height);
}

void WidgetGL::paintGL() {
  glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
  Program::Use use(*program);
  program->setUniform("q", q.x(), q.y(), q.z(), q.w());
  program->setAttribute("vertex", *axis_buffer);
  program->setAttribute("color", *color_buffer);
  program->enableVertexAttribArray("vertex");
  program->enableVertexAttribArray("color");
  glDrawArrays(GL_LINES, 0, axis_buffer->size());
  program->disableVertexAttribArray("color");
  program->disableVertexAttribArray("vertex");
}

void WidgetGL::set(const float theta, const Vector3f& n) {
  q = Quaternion<float>::axisAngle(n.x(), n.y(), n.z(), theta);
  repaint();
}
