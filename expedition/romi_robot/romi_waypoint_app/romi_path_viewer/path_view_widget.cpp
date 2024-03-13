#include "path_view_widget.h"

#include <QImage>
#include <QPainter>
#include <QDebug>
#include <QKeyEvent>

#include <array>

using namespace Eigen;
using namespace opengl;

static const char vertex_shader[] = R"(
    #version 120
    attribute vec2 coord;
    uniform mat3 mat;
    void main(void)
    {
        vec3 new_pos = mat*vec3(coord.xy, 1.0f);
        gl_Position = vec4(new_pos.xy, 0.0f, 1.0f);
    })";

static const char fragment_shader[] = R"(
    #version 120
    uniform vec3 color;
    void main(void)
    {
        gl_FragColor = vec4(color, 1.0f);
    })";

static const char bg_vertex_shader[] = R"(
    #version 120
    attribute vec2 vertex;
    attribute vec2 inTc;
    varying vec2 tc;
    void main(void) {
        gl_Position = vec4(vertex, 0.0f, 1.0f);
        tc = inTc;
    })";

static const char bg_fragment_shader[] = R"(
    #version 120
    varying vec2 tc;
    uniform sampler2D tex;
    void main(void) {
        gl_FragColor = texture2D(tex, tc).bgra;
    })";

QImage createBackground(const size_t width, const size_t height) {
  QImage image(width, height, QImage::Format_ARGB32);
  QPainter painter(&image);
  painter.fillRect(QRect(0, 0, width, height), QColor(0xFF, 0xFF, 0xFF));
  painter.setPen(QPen(QColor(0, 0, 0)));
  painter.drawLine(0, height/2, width, height/2);
  painter.drawLine(width/2, 0, width/2, height);
  painter.drawArc(QRect(width / 2 - 100, height / 2 - 100, 200, 200), 0, 360*16);
  painter.drawArc(QRect(width / 2 - 200, height / 2 - 200, 400, 400), 0, 360 * 16);
  painter.drawArc(QRect(width / 2 - 300, height / 2 - 300, 600, 600), 0, 360 * 16);
  painter.drawArc(QRect(width / 2 - 400, height / 2 - 400, 800, 800), 0, 360 * 16);
  return image;
}

PathViewWidget::PathViewWidget(QWidget* parent)
  : QOpenGLWidget(parent) {
  QSurfaceFormat format;
  format.setRenderableType(QSurfaceFormat::OpenGL);
  setFormat(format);
}

void PathViewWidget::initializeGL() {
  initializeOpenGLFunctions();
  glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

  static const std::map<GLenum, std::string> shaders = {
      { GL_VERTEX_SHADER, vertex_shader },
      { GL_FRAGMENT_SHADER, fragment_shader} };

  static const std::map<GLenum, std::string> bg_shaders = {
      { GL_VERTEX_SHADER, bg_vertex_shader },
      { GL_FRAGMENT_SHADER, bg_fragment_shader} };

  {
    const std::vector<Vector2f> coords = {
      {0.0f, 1.0f},
      {1.0f, 1.0f},
      {1.0f, 0.0f},
      {0.0f, 0.0f} };
    bg_tex_coords = std::make_unique<Buffer<Vector2f, GL_ARRAY_BUFFER>>(GL_STATIC_DRAW, coords);
    const std::vector<Vector3f> points = {
      {-1.0f, -1.0f, 0.0f},
      {+1.0f, -1.0f, 0.0f},
      {+1.0f, +1.0f, 0.0f},
      {-1.0f, +1.0f, 0.0f}
    };
    bg_vertex_buffer = std::make_unique<Buffer<Vector3f, GL_ARRAY_BUFFER>>(GL_STATIC_DRAW, points);
    const std::vector<unsigned int> indices = { 0, 1, 3, 2 };
    bg_indices_buffer = std::make_unique<Buffer<unsigned int, GL_ELEMENT_ARRAY_BUFFER>>(GL_STATIC_DRAW, indices);
  }
  program = std::make_unique<Program>(shaders);
  bg_program = std::make_unique<Program>(bg_shaders);

  wp_buffer = std::make_unique<Buffer<Vector2f, GL_ARRAY_BUFFER>>(GL_DYNAMIC_DRAW, 100000);
  traj_buffer = std::make_unique<Buffer<Vector2f, GL_ARRAY_BUFFER>>(GL_DYNAMIC_DRAW, 100000);
  const std::vector<Vector2f> head_points = { 
    {-0.07f, 0.07f}, {0.07f, 0.07f}, {0.14f, 0.0f}, {0.07f, -0.07f}, {-0.07f, -0.07f} };
  head_buffer = std::make_unique<Buffer<Vector2f, GL_ARRAY_BUFFER>>(GL_STATIC_DRAW, head_points);
}

void PathViewWidget::resizeGL(int width, int height) {
  const int size = std::min(width, height);
  const int x = (width-size)/2;
  const int y = (height-size)/2;
  glViewport(x, y, size, size);
  const auto image = createBackground(size, size);
  bg_texture = std::make_unique<Texture2D>(GL_RGBA, GL_RGBA, GL_UNSIGNED_BYTE, size, size, image.bits());
}

void PathViewWidget::paintBackground() {
  if (!bg_texture) {
    return;
  }
  Program::Use use(*bg_program);
  bg_texture->bind(GL_TEXTURE0);
  bg_program->setUniform("tex", 0);
  bg_program->setAttribute("vertex", *bg_vertex_buffer);
  bg_program->setAttribute("inTc", *bg_tex_coords);
  bg_program->enableVertexAttribArray("vertex");
  bg_program->enableVertexAttribArray("inTc");
  bg_indices_buffer->bind();
  glDrawElements(GL_TRIANGLE_STRIP, bg_indices_buffer->size(), GL_UNSIGNED_INT, nullptr);
  bg_program->disableVertexAttribArray("vertex");
  bg_program->disableVertexAttribArray("inTc");
  bg_indices_buffer->unbind();
}

void PathViewWidget::paintGL() {
  paintBackground();
  {
    Program::Use use(*program);
    program->setAttribute("coord", *wp_buffer);
    program->setUniform("color", Vector3f(0.0f, 0.0f, 1.0f));
    const Matrix3f mat = Matrix3f::Identity()*scale_factor;
    program->setUniform("mat", mat);
    program->enableVertexAttribArray("coord");
    glDrawArrays(GL_LINE_STRIP, 0, wp_buffer->size());
    program->disableVertexAttribArray("coord");
  }
  {
    Program::Use use(*program);
    program->setAttribute("coord", *traj_buffer);
    program->setUniform("color", Vector3f(1.0f, 0.0f, 0.0f));
    const Matrix3f mat = Matrix3f::Identity()*scale_factor;
    program->setUniform("mat", mat);
    program->enableVertexAttribArray("coord");
    glDrawArrays(GL_LINE_STRIP, 0, traj_buffer->size());
    program->disableVertexAttribArray("coord");
  }
  {
    Program::Use use(*program);
    program->setAttribute("coord", *head_buffer);
    program->setUniform("color", Vector3f(1.0f, 0.0f, 1.0f));
    const Matrix3f mat = matrix*scale_factor;
    program->setUniform("mat", mat);
    program->enableVertexAttribArray("coord");
    static const std::vector<unsigned short> indices = {0, 1, 4, 1, 2, 3, 1, 3, 4};
    glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_SHORT, indices.data());
    program->disableVertexAttribArray("coord");
  }
}

void PathViewWidget::keyPressEvent(QKeyEvent* event) {
  switch (event->key()) {
    case Qt::Key_Up:
      scale_factor += 0.25;
      break;
    case Qt::Key_Down:
      scale_factor -= 0.25;
      break;
  }
  repaint();
}

void PathViewWidget::add(const float yaw, const std::vector<Vector2f>& points) {
  makeCurrent();
  traj_buffer->append(points);
  doneCurrent();
  matrix(0, 0) = cos(yaw);
  matrix(0, 1) = -sin(yaw);
  matrix(1, 0) = sin(yaw);
  matrix(1, 1) = cos(yaw);
  matrix(0, 2) = points.back().x();
  matrix(1, 2) = points.back().y();
  matrix(2, 2) = 1.0f;
  repaint();
}

void PathViewWidget::mousePressEvent(QMouseEvent* event) {
  if (event->button() == Qt::LeftButton) {
    Eigen::Vector2d p;
    p.x() = static_cast<double>(event->x())/static_cast<double>(width()/2.0) - 1.0;
    p.y() = 1.0 - static_cast<double>(event->y())/static_cast<double>(height()/2.0);
    emit newWayPoint(p.x() * 1000.0, p.y() * 1000.0);

    std::vector<Eigen::Vector2f> points;
    if (wp_buffer->size() == 0) {
      points.emplace_back(matrix(0, 2), matrix(1, 2));
    }
    points.emplace_back(p.cast<float>());
    makeCurrent();
    wp_buffer->append(points);
    doneCurrent();
    repaint();
  }
}

void PathViewWidget::clear() {
  makeCurrent();
  wp_buffer->clear();
  traj_buffer->clear();
  doneCurrent();
  repaint();
}
