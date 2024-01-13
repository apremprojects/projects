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
    varying vec3 color;
    uniform int status;
    void main(void)
    {
        vec4 p = mat4(rot_mat) * (vertex - vec4(center, 0.0f));
        gl_Position = p;
        if (p.x > 0) {
            //0, 1, 2, 3
            if (p.y > 0) {
                //0, 1
                if (p.z > 0) {
                    //0
                    color = ((status&1) == 1) ? vec3(1.0, 0.0, 0.0): vec3(0,0,0); //Red
                }
                else {
                    //1
                    color = ((status&2) == 2) ? vec3(0.0, 1.0, 0.0): vec3(0,0,0); //Green
                }
            }
            else {
                //2,3
                if (p.z > 0) {
                    //2
                    color = ((status&4) == 4) ? vec3(0.0, 0.0, 1.0): vec3(0,0,0); //Blue
                }
                else {
                    //3
                    color = ((status&8) == 8) ? vec3(1.0, 1.0, 0.0): vec3(0,0,0);//Red Green
                }
            }
        }
        else {
            //4, 5, 6, 7
            if (p.y > 0) {
                //4, 5
                if (p.z > 0) {
                    //4
                    color = ((status&16) == 16) ? vec3(1.0, 0.0, 1.0): vec3(0,0,0); //Red Blue
                }
                else {
                    //5
                    color = ((status&32) == 32) ? vec3(0.0, 1.0, 1.0): vec3(0,0,0); //Green Blue
                }
            }
            else {
                //6,7
                if (p.z > 0) {
                    //6
                    color = ((status&64) == 64) ? vec3(1.0, 1.0, 1.0): vec3(0,0,0); //White
                }
                else {
                    //7
                    color = ((status&128) == 128) ? vec3(0.5, 0.5, 0.5): vec3(0,0,0); //Gray
                }
            }
        }
    })";

static const char axis_vertex_shader[] = R"(
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
    varying vec3 color;
    void main(void)
    {
        gl_FragColor = vec4(color, 1.0f);
    })";

static const char axis_fragment_shader[] = R"(
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
  : QOpenGLWidget(parent) {
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
  static const std::map<GLenum, std::string> axis_shaders = {
      { GL_VERTEX_SHADER, axis_vertex_shader },
      { GL_FRAGMENT_SHADER, axis_fragment_shader}
  };
  program = std::make_unique<Program>(shaders);
  axis_program = std::make_unique<Program>(axis_shaders);
  data_buffer = std::make_unique<Buffer<Vector3f, GL_ARRAY_BUFFER>>(GL_DYNAMIC_DRAW, 10000);
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
    program->setUniform("center", center);
    program->setUniform("status", status);
    program->setAttribute("vertex", *data_buffer);
    program->enableVertexAttribArray("vertex");
    program->setUniform("color", Vector3f(1.0f, 1.0f, 1.0f));
    glDrawArrays(GL_POINTS, 0, data_buffer->size());
    program->disableVertexAttribArray("vertex");
}

void MagnetometerWidget::setData(const size_t start, const std::vector<Eigen::Vector3f>& mag_data, const Eigen::Vector3f& _center, const int _status) {
    makeCurrent();
    data_buffer->append(mag_data.data() + start, mag_data.size() - start);
    center = _center;
    status = _status;
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

void MagnetometerWidget::drawAxes(const Matrix3f& rot, const Eigen::Vector3f& center) {
    static const std::array<Vector3f, 3> colors = {
        Vector3f(1.0f, 0.0f, 0.0f),
        Vector3f(0.0f, 1.0f, 0.0f),
        Vector3f(0.0f, 0.0f, 1.0f)
    };
    Program::Use use(*axis_program);
    axis_program->setUniform("rot_mat", rot);
    axis_program->setUniform("center", center);
    axis_program->setAttribute("vertex", *axis_buffer);
    axis_program->enableVertexAttribArray("vertex");
    for (int i = 1; i < 4; i++) {
        axis_program->setUniform("color", colors[i - 1]);
        const std::array<unsigned, 2> indices = { 0, i };
        glDrawElements(GL_LINE_STRIP, 2, GL_UNSIGNED_INT, indices.data());
    }
    axis_program->disableVertexAttribArray("vertex");
}