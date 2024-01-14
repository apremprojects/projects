#include "magnetometer_widget.h"

#include <QDebug>
#include <array>

using namespace Eigen;
using namespace opengl;

static const char vertex_shader[] = R"(
    #version 130
    attribute vec4 vertex;
    uniform vec3 center;
    uniform mat3 rot_mat;
    void main(void)
    {
        gl_Position = mat4(rot_mat) * (vertex - vec4(center, 0.0f));
    })";

static const char fragment_shader[] = R"(
    #version 130
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
    ring_buffer_size(10000) {
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
  const std::vector<Vector3f> line_points{
      {0.0f, 0.0f, 0.0f},
      { 0.0f, 0.0f, 0.0f }
  };
  axis_buffer = std::make_unique<Buffer<Vector3f, GL_ARRAY_BUFFER>>(GL_STATIC_DRAW, axis_points);
  line_buffer = std::make_unique<Buffer<Vector3f, GL_ARRAY_BUFFER>>(GL_DYNAMIC_DRAW, line_points);
}

void MagnetometerWidget::resizeGL(int width, int height) {
  glViewport(0, 0, width, height);
}

void MagnetometerWidget::paintGL() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    auto rot = rotMatrix();

    drawAxes(rot, Vector3f::Zero());
    {
        Program::Use use(*program);
        program->setUniform("rot_mat", rot);
        program->setUniform("center", center);
        program->setUniform("status", status);
        program->setAttribute("vertex", *ring_buffer);
        program->enableVertexAttribArray("vertex");
        program->setUniform("color", Vector3f(1.0f, 1.0f, 1.0f));
        glDrawArrays(GL_POINTS, 0, ring_buffer_size);
        program->disableVertexAttribArray("vertex");
    }
    {
        Program::Use use(*program);
        program->setUniform("rot_mat", rot);
        program->setUniform("center", center);
        program->setUniform("status", status);
        program->setAttribute("vertex", *line_buffer);
        program->enableVertexAttribArray("vertex");
        program->setUniform("color", Vector3f(0.0f, 1.0f, 1.0f));
        glDrawArrays(GL_LINES, 0, 2);
        program->disableVertexAttribArray("vertex");
    }
}

void MagnetometerWidget::setData(const std::vector<Eigen::Vector3f>& mag_data) {
    makeCurrent();
    for(const auto &i: mag_data){
        ring_buffer->load(ring_index, 1, &i);
        ring_index = (ring_index + 1) % ring_buffer_size;
    }
    if (!mag_data.empty()) {
        line_buffer->load(1, 1, &mag_data.back());
    }
    doneCurrent();
    repaint();
}

void MagnetometerWidget::clearRingBuffers() {
    repaint();
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

Matrix3f MagnetometerWidget::rotMatrix(const float x, const float y, const float z) {
    Matrix3f mat_x, mat_y, mat_z;
    mat_x << 1, 0, 0,
        0, cos(x), -sin(x),
        0, sin(x), cos(x);
    mat_y << cos(y), 0, -sin(y),
        0, 1, 0,
        sin(y), 0, cos(y);
    mat_z << cos(z), -sin(z), 0,
        sin(z), cos(z), 0,
        0, 0, 1;
    return mat_x * mat_y * mat_z;
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