#pragma once

#include <cmath>
#include <numbers>
#include <QMutex>
#include <QQueue>
#include <QOpenGLWidget>
#include <QOpenGLFunctions>

#include "../../../helper/buffer.h"
#include "../../../helper/texture.h"
#include "../../../helper/program.h"
#include <vector>
#include "work_thread.h"
#define _USE_MATH_DEFINES

class MagnetometerWidget : public QOpenGLWidget, private QOpenGLFunctions {
	Q_OBJECT
public:
	MagnetometerWidget(QWidget* parent);
	void setData(const std::vector<Eigen::Vector3f> &mag_data);
	void clearRingBuffers();
public slots:
	void angleXChanged(int x);
	void angleYChanged(int y);
	void angleZChanged(int z);
private:
	void initializeGL() override;
	void paintGL() override;
	void resizeGL(int width, int height) override;

	Eigen::Matrix3f rotMatrix();
	Eigen::Matrix3f rotMatrix(const float x, const float y, const float z);
	void drawAxes(const Eigen::Matrix3f& rot, const Eigen::Vector3f &center);
	int ring_index;
	int ring_buffer_size;
	Eigen::Vector3f center = Eigen::Vector3f::Zero();
	std::unique_ptr<opengl::Program> program;
	std::unique_ptr<opengl::Buffer<Eigen::Vector3f, GL_ARRAY_BUFFER>> ring_buffer;
	std::unique_ptr<opengl::Buffer<Eigen::Vector3f, GL_ARRAY_BUFFER>> line_buffer;
	std::unique_ptr<opengl::Buffer<Eigen::Vector3f, GL_ARRAY_BUFFER>> axis_buffer;
	int status = 0;
	float x_rad = 0.0f;
	float y_rad = 0.0f;
	float z_rad = 0.0f;
};
