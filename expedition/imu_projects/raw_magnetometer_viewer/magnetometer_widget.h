#pragma once

#include <memory>

#include <QMutex>
#include <QQueue>
#include <QOpenGLWidget>
#include <QOpenGLFunctions>

#include "../../../helper/buffer.h"
#include "../../../helper/texture.h"
#include "../../../helper/program.h"

#include "work_thread.h"

class MagnetometerWidget : public QOpenGLWidget, private QOpenGLFunctions {
	Q_OBJECT
public:
	MagnetometerWidget(QWidget* parent);
	void setMutexAndQueue(QMutex& m, QQueue<MagnetometerData>& q);
	void clearRingBuffers();
public slots:
	void angleXChanged(int x);
	void angleYChanged(int y);
	void angleZChanged(int z);
	void shiftCenter(int status);
private:
	void initializeGL() override;
	void paintGL() override;
	void resizeGL(int width, int height) override;

	Eigen::Matrix3f rotMatrix();
	void drawAxes(const Eigen::Matrix3f& rot, const Eigen::Vector3f &center);

	std::unique_ptr<opengl::Program> program;
	std::unique_ptr<opengl::Buffer<Eigen::Vector3f, GL_ARRAY_BUFFER>> ring_buffer;
	std::unique_ptr<opengl::Buffer<Eigen::Vector3f, GL_ARRAY_BUFFER>> axis_buffer;
	float time_offset = 0.0f;
	int ring_index;
	int ring_buffer_size;
	float x_rad = 0.0f;
	float y_rad = 0.0f;
	float z_rad = 0.0f;
	bool shift_center = false;
	Eigen::Vector3f min_point = { 1.0f, 1.0f, 1.0f };
	Eigen::Vector3f max_point = {-1.0f, -1.0f, -1.0f};
  QMutex* mutex = nullptr;
  QQueue<MagnetometerData>* queue = nullptr;
};
