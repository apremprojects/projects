#include "graph_widget.h"
#include "./../graph_image/graph_image.h"
#include <QDebug>
#include <array>
#include <cmath>

#define M_PI (4.0f * std::atan(1.0f))

using namespace Eigen;
using namespace opengl;

static const char vertex_shader[] = R"(
    #version 120
    attribute float x_coord;
    attribute float y_coord;
    uniform float x_offset;
    uniform float scale_factor_x;
    uniform float scale_factor_y;
    void main(void)
    {
        float x = -1.0f + scale_factor_x*(x_coord - x_offset);
        gl_Position = vec4(x, scale_factor_y*y_coord, 0.0f, 1.0f);
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
    attribute vec4 vertex;
    attribute vec2 inTc;
    varying vec2 tc;
    void main(void) {
        gl_Position = vertex;
        tc = inTc;
    })";

static const char bg_fragment_shader[] = R"(
    #version 120
    varying vec2 tc;
    uniform sampler2D tex;
    void main(void) {
        gl_FragColor = texture2D(tex, tc).bgra;
    })";

GraphWidget::GraphWidget(QWidget* parent)
    : QOpenGLWidget(parent),
    ring_index(0),
    ring_buffer_size(60*40){
  QSurfaceFormat format;
  format.setRenderableType(QSurfaceFormat::OpenGL);
  setFormat(format);
}

void GraphWidget::initializeGL() {
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

  // Acceleration measurement range is +/- 16g.
  const auto image = createGraphTemplate(params);
  program = std::make_unique<Program>(shaders);
  bg_program = std::make_unique<Program>(bg_shaders);
  bg_texture = std::make_unique<Texture2D>(GL_RGBA, GL_RGBA, GL_UNSIGNED_BYTE, image.width(), image.height(), image.bits());
  x_buffer = std::make_unique<Buffer<float, GL_ARRAY_BUFFER>>(GL_DYNAMIC_DRAW, ring_buffer_size);
  y_buffer = std::make_unique<Buffer<float, GL_ARRAY_BUFFER>>(GL_DYNAMIC_DRAW, ring_buffer_size);
  z_buffer = std::make_unique<Buffer<float, GL_ARRAY_BUFFER>>(GL_DYNAMIC_DRAW, ring_buffer_size);
  t_buffer = std::make_unique<Buffer<float, GL_ARRAY_BUFFER>>(GL_DYNAMIC_DRAW, ring_buffer_size);
}

void GraphWidget::resizeGL(int width, int height) {
  glViewport(0, 0, width, height);
  params.width = width;
  params.height = height;
  params.x_ticks = ((width * 24) / 963) + 1;
  const auto image = createGraphTemplate(params);
  bg_texture = std::make_unique<Texture2D>(GL_RGBA, GL_RGBA, GL_UNSIGNED_BYTE, image.width(), image.height(), image.bits());
}

void GraphWidget::paintBackground() {
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

void GraphWidget::setParams(const GraphParams& _params) {
    params = _params;
}

void GraphWidget::paintGL() {
  {
    if (clear_flag) {
        std::vector<float> data(ring_buffer_size, 0.0f);
        qDebug() << x_buffer->size();
        ring_index = 0;
        x_buffer->load(data);
        y_buffer->load(data);
        z_buffer->load(data);
        t_buffer->load(data);
        clear_flag = false;
        qDebug() << "Clearing Ring Buffer...";
    }
  }
  paintBackground();

  const std::array<Buffer<float, GL_ARRAY_BUFFER>*, 3> buffers = {
    x_buffer.get(), y_buffer.get(), z_buffer.get()
  };

  const std::array<Vector3f, 3> colors = { Vector3f(1.0f, 0.0f, 0.0f), Vector3f(0.0f, 1.0f, 0.0f), Vector3f(0.0f, 0.0f, 1.0f) };
  std::array<unsigned int, 2> indices = { ring_buffer_size - 1, 0 };
  for (size_t i = 0; i < buffers.size(); i++) {
    Program::Use use(*program);
    program->setAttribute("x_coord", *t_buffer);
    program->setAttribute("y_coord", *buffers[i]);
    program->setUniform("x_offset", time_offset);
    program->setUniform("scale_factor_x", 2000.0f / (ring_buffer_size * 25));
    program->setUniform("scale_factor_y", 1.0f/(params.y_end));
    program->setUniform("color", colors[i]);
    program->enableVertexAttribArray("x_coord");
    program->enableVertexAttribArray("y_coord");
    // Lines start at ring_index. Draw from ring_index to ring_buffer_size-1 
    glDrawArrays(GL_LINE_STRIP, ring_index, ring_buffer_size-ring_index);
    // Draw line from index ring_buffer_size-1 to 0
    glDrawElements(GL_LINE_STRIP, 2, GL_UNSIGNED_INT, indices.data());
    // Draw lines from 0 to ring_index-1
    glDrawArrays(GL_LINE_STRIP, 0, ring_index);
    program->disableVertexAttribArray("x_coord");
    program->disableVertexAttribArray("y_coord");
  }
}

void GraphWidget::setData(const GraphData& data) {
    if (data.size() == 0) {
        return;
    }
    makeCurrent();
    if (ring_index + data.size() > ring_buffer_size) {
        int free_space = ring_buffer_size - ring_index;
        x_buffer->load(ring_index, free_space, data.x.data());
        x_buffer->load(0, data.size() - free_space, data.x.data());
        y_buffer->load(ring_index, free_space, data.y.data());
        y_buffer->load(0, data.size() - free_space, data.y.data());
        z_buffer->load(ring_index, free_space, data.z.data());
        z_buffer->load(0, data.size() - free_space, data.z.data());
        t_buffer->load(ring_index, free_space, data.t.data());
        t_buffer->load(0, data.size() - free_space, data.t.data());
        ring_index = (ring_index + data.x.size()) % ring_buffer_size;
    }
    else {
        x_buffer->load(ring_index, data.x.size(), data.x.data());
        y_buffer->load(ring_index, data.y.size(), data.y.data());
        z_buffer->load(ring_index, data.z.size(), data.z.data());
        t_buffer->load(ring_index, data.t.size(), data.t.data());
        ring_index = (ring_index + data.x.size()) % ring_buffer_size;
    }
    time_offset = data.t.back() - 60.0f;
    doneCurrent();
    repaint();
}

void GraphWidget::clearRingBuffers() {
    clear_flag = true;
    repaint();
}