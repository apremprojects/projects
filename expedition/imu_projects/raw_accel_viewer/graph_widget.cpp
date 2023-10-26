#include "graph_widget.h"
#include "./../graph_image/graph_image.h"

#include <QDebug>

#include <array>

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


static constexpr float g = 9.08f;

GraphWidgetGL::GraphWidgetGL(QWidget* parent)
  : QOpenGLWidget(parent),
    ring_index(0),
    ring_buffer_size(60*40) {
  QSurfaceFormat format;
  format.setRenderableType(QSurfaceFormat::OpenGL);
  setFormat(format);
}

void GraphWidgetGL::initializeGL() {
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
  const GraphParams acc_graph_params = { 48 * 40, 320, 0.0, 60, 24, -16 * g, 16 * g, 16, 25 };
  const auto image = createGraphTemplate(acc_graph_params);
  program = std::make_unique<Program>(shaders);
  bg_program = std::make_unique<Program>(bg_shaders);
  bg_texture = std::make_unique<Texture2D>(GL_RGBA, GL_RGBA, GL_UNSIGNED_BYTE, image.width(), image.height(), image.bits());
  x_buffer = std::make_unique<Buffer<float, GL_ARRAY_BUFFER>>(GL_DYNAMIC_DRAW, ring_buffer_size);
  y_buffer = std::make_unique<Buffer<float, GL_ARRAY_BUFFER>>(GL_DYNAMIC_DRAW, ring_buffer_size);
  z_buffer = std::make_unique<Buffer<float, GL_ARRAY_BUFFER>>(GL_DYNAMIC_DRAW, ring_buffer_size);
  t_buffer = std::make_unique<Buffer<float, GL_ARRAY_BUFFER>>(GL_DYNAMIC_DRAW, ring_buffer_size);
}

void GraphWidgetGL::resizeGL(int width, int height) {
  glViewport(0, 0, width, height);
}

void GraphWidgetGL::paintBackground() {
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

void GraphWidgetGL::paintGL() {
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
    if (queue == nullptr || mutex == nullptr) {
      return;
    }
    QMutexLocker lock(mutex);
    while (!queue->empty()) {
      const auto acc_data = queue->front();
      queue->pop_front();
      x_buffer->load(ring_index, 1, &acc_data.acc.x());
      y_buffer->load(ring_index, 1, &acc_data.acc.y());
      z_buffer->load(ring_index, 1, &acc_data.acc.z());
      t_buffer->load(ring_index, 1, &acc_data.time_stamp);
      ring_index = (ring_index + 1) % ring_buffer_size;
      time_offset = acc_data.time_stamp - 60.0f;
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
    program->setUniform("scale_factor_y", 1.0f/(16*g));
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

void GraphWidgetGL::setMutexAndQueue(QMutex& m, QQueue<AccelData>& q) {
  mutex = &m;
  queue = &q;
}

void GraphWidgetGL::clearRingBuffers() {
    clear_flag = true;
    repaint();
}