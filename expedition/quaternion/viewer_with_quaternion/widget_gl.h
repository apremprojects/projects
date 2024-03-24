#pragma once

#include <memory>

#include <QOpenGLWidget>
#include <QOpenGLFunctions>

#include "./../../../helper/buffer.h"
#include "./../../../helper/texture.h"
#include "./../../../helper/program.h"
#include "./../obj_file/obj_data_gl.h"

#include <Eigen/Geometry>

class WidgetGL : public QOpenGLWidget, private QOpenGLFunctions {
	Q_OBJECT
public:
	WidgetGL(QWidget* parent);

	void set(const ObjReader::Data& obj_data);
	void setVisibility(const std::string& name, const bool visible);
private:
	void initializeGL() override;
	void paintGL() override;
	void resizeGL(int width, int height) override;
	void mousePressEvent(QMouseEvent* event) override;
	void mouseReleaseEvent(QMouseEvent* event) override;
	void mouseMoveEvent(QMouseEvent* event) override;
	Eigen::Vector3f old_v, new_v;
	Eigen::Quaternionf q = Eigen::Quaternionf::Identity();
	bool m_flag = false;

	std::unique_ptr<opengl::Program> program;
	std::unique_ptr<opengl::ObjData> obj_data;
};
