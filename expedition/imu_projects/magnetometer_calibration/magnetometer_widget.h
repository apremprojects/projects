#pragma once

#include <memory>

#include <QMutex>
#include <QQueue>
#include <QOpenGLWidget>
#include <QOpenGLFunctions>

#include "../../../helper/buffer.h"
#include "../../../helper/texture.h"
#include "../../../helper/program.h"
#include <vector>
#include "work_thread.h"

class MagnetometerWidget : public QOpenGLWidget, private QOpenGLFunctions {
	Q_OBJECT
public:
	MagnetometerWidget(QWidget* parent);
	void setData(const size_t start, const std::vector<Eigen::Vector3f> &mag_data, const Eigen::Vector3f &_center, const int _status);
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
	void drawAxes(const Eigen::Matrix3f& rot, const Eigen::Vector3f &center);

	Eigen::Vector3f center = Eigen::Vector3f::Zero();
	std::unique_ptr<opengl::Program> program;
	std::unique_ptr<opengl::Program> axis_program;
	std::unique_ptr<opengl::Buffer<Eigen::Vector3f, GL_ARRAY_BUFFER>> data_buffer;
	std::unique_ptr<opengl::Buffer<Eigen::Vector3f, GL_ARRAY_BUFFER>> axis_buffer;
	int status = 0;
	float x_rad = 0.0f;
	float y_rad = 0.0f;
	float z_rad = 0.0f;
};
