#pragma once

#include <memory>

#include <QMutex>
#include <QQueue>
#include <QOpenGLWidget>
#include <QOpenGLFunctions>

#include "../../../helper/buffer.h"
#include "../../../helper/texture.h"
#include "../../../helper/program.h"
#include "./../graph_image/graph_image.h"

#include "work_thread.h"

static constexpr float g = 9.08f;

struct GraphData {
	GraphData(const size_t capacity){
		x.reserve(capacity);
		y.reserve(capacity);
		z.reserve(capacity);
		t.reserve(capacity);
	}
	void clear() {
		x.resize(0);
		y.resize(0);
		z.resize(0);
		t.resize(0);
	}
	size_t size() const {
		return x.size();
	}
	std::vector<float> x;
	std::vector<float> y;
	std::vector<float> z;
	std::vector<float> t;
};

class GraphWidget : public QOpenGLWidget, private QOpenGLFunctions {
  Q_OBJECT
public:
  GraphWidget(QWidget* parent);
  void clearRingBuffers();
  void setData(const GraphData& data);
  void setParams(const GraphParams& _params);
private:
  void initializeGL() override;
  void paintGL() override;
  void resizeGL(int width, int height) override;

  void paintBackground();
  GraphParams params = { 48 * 40, 320, 0.0, 60, 24, 0, 0, 16, 25, "Default Title"};
  std::unique_ptr<opengl::Program> bg_program;
  std::unique_ptr<opengl::Texture2D> bg_texture;
  std::unique_ptr<opengl::Buffer<Eigen::Vector2f, GL_ARRAY_BUFFER>> bg_tex_coords;
  std::unique_ptr<opengl::Buffer<Eigen::Vector3f, GL_ARRAY_BUFFER>> bg_vertex_buffer;
  std::unique_ptr<opengl::Buffer<unsigned int, GL_ELEMENT_ARRAY_BUFFER>> bg_indices_buffer;

  std::unique_ptr<opengl::Program> program;
  std::unique_ptr<opengl::Buffer<float, GL_ARRAY_BUFFER>> x_buffer;
  std::unique_ptr<opengl::Buffer<float, GL_ARRAY_BUFFER>> y_buffer;
  std::unique_ptr<opengl::Buffer<float, GL_ARRAY_BUFFER>> z_buffer;
  std::unique_ptr<opengl::Buffer<float, GL_ARRAY_BUFFER>> t_buffer;

  size_t ring_index = 0;
  size_t ring_buffer_size = 0;
  float time_offset = 0.0f;
  bool clear_flag = false;
};
