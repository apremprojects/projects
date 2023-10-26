#include "magnetometer_widget.h"

#include <QDebug>
#include <array>

using namespace Eigen;
using namespace opengl;

static const char vertex_shader[] = R"(
    #version 120
    attribute vec4 vertex;
    uniform vec3 center;
    uniform mat3 rot_mat;
    void main(void)
    {
        gl_Position = mat4(rot_mat) * (vertex - vec4(center, 0.0f));
    })";

static const char fragment_shader[] = R"(
    #version 120
    uniform vec3 color;
    void main(void)
    {
        gl_FragColor = vec4(color, 1.0f);
    })";

float degToRad(const int theta) {
    return (theta / 180.0f) * std::atan(1.0f) * 4.0f;
}

MagnetometerWidget::MagnetometerWidget(QWidget* parent)
  : QOpenGLWidget(parent),
    ring_index(0),
    ring_buffer_size(5 * 60 * 40) {
  QSurfaceFormat format;
  format.setRenderableType(QSurfaceFormat::OpenGL);
  setFormat(format);
}

void MagnetometerWidget::initializeGL() {
  initializeOpenGLFunctions();
  glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
  static const std::map<GLenum, std::string> shaders = {
      { GL_VERTEX_SHADER, vertex_shader },
      { GL_FRAGMENT_SHADER, fragment_shader}
  };
  program = std::make_unique<Program>(shaders);
  ring_buffer = std::make_unique<Buffer<Vector3f, GL_ARRAY_BUFFER>>(GL_DYNAMIC_DRAW, ring_buffer_size);
  const std::vector<Vector3f> axis_points{
      {0.0f, 0.0f, 0.0f},
      { 1.0f, 0.0f, 0.0f },
      { 0.0f, 1.0f, 0.0f },
      { 0.0f, 0.0f, 1.0f }
  };
  axis_buffer = std::make_unique<Buffer<Vector3f, GL_ARRAY_BUFFER>>(GL_STATIC_DRAW, axis_points);
}

void MagnetometerWidget::resizeGL(int width, int height) {
  glViewport(0, 0, width, height);
}

void MagnetometerWidget::paintGL() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    auto rot = rotMatrix();

    drawAxes(rot, Vector3f::Zero());
    Program::Use use(*program);
    program->setUniform("rot_mat", rot);
    Vector3f center = { 0, 0, 0 };
    if (shift_center) {
        center = (min_point + max_point) * 0.5f;
    }
    program->setUniform("center", center);
    program->setAttribute("vertex", *ring_buffer);
    program->enableVertexAttribArray("vertex");
    program->setUniform("color", Vector3f(1.0f, 1.0f, 1.0f));
    glDrawArrays(GL_POINTS, 0, ring_buffer_size);
    program->disableVertexAttribArray("vertex");
}

void MagnetometerWidget::angleXChanged(int x) {
    x_rad = degToRad(x);
    repaint();
}
void MagnetometerWidget::angleYChanged(int y) {
    y_rad = degToRad(y);
    repaint();
}
void MagnetometerWidget::angleZChanged(int z) {
    z_rad = degToRad(z);
    repaint();
}

Matrix3f MagnetometerWidget::rotMatrix() {
    Matrix3f mat_x, mat_y, mat_z;
    mat_x << 1, 0, 0,
        0, cos(x_rad), -sin(x_rad),
        0, sin(x_rad), cos(x_rad);
    mat_y << cos(y_rad), 0, -sin(y_rad),
        0, 1, 0,
        sin(y_rad), 0, cos(y_rad);
    mat_z << cos(z_rad), -sin(z_rad), 0,
        sin(z_rad), cos(z_rad), 0,
        0, 0, 1;
    return mat_x * mat_y * mat_z;
}

void MagnetometerWidget::shiftCenter(int status) {
    shift_center = status == Qt::Checked;
}

void MagnetometerWidget::drawAxes(const Matrix3f& rot, const Eigen::Vector3f& center) {
    static const std::array<Vector3f, 3> colors = {
        Vector3f(1.0f, 0.0f, 0.0f),
        Vector3f(0.0f, 1.0f, 0.0f),
        Vector3f(0.0f, 0.0f, 1.0f)
    };
    Program::Use use(*program);
    program->setUniform("rot_mat", rot);
    program->setUniform("center", center);
    program->setAttribute("vertex", *axis_buffer);
    program->enableVertexAttribArray("vertex");
    for (int i = 1; i < 4; i++) {
        program->setUniform("color", colors[i - 1]);
        const std::array<unsigned, 2> indices = { 0, i };
        glDrawElements(GL_LINE_STRIP, 2, GL_UNSIGNED_INT, indices.data());
    }
    program->disableVertexAttribArray("vertex");
}

void MagnetometerWidget::setData(const std::vector<Vector3f>& data) {
    makeCurrent();
    if (ring_index + data.size() > ring_buffer_size) {
        int free_space = ring_buffer_size - ring_index;
        ring_buffer->load(ring_index, free_space, data.data());
        ring_buffer->load(0, data.size() - free_space, data.data());
        ring_index = (ring_index + data.size()) % ring_buffer_size;
    }
    else {
        ring_buffer->load(ring_index, data.size(), data.data());
        ring_index = (ring_index + data.size()) % ring_buffer_size;
    }
    doneCurrent();
    for (const auto& v : data) {
        min_point = min_point.cwiseMin(v);
        max_point = max_point.cwiseMax(v);
    }
    repaint();
}