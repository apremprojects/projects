#include "widget_gl.h"

#include <QDebug>
#include <QKeyEvent>
#include <QMouseEvent>

#include <array>
#include <iostream>

#define DEG_TO_RAD(theta) (theta / 45.0f * std::atan(1.0))

using namespace Eigen;
using namespace opengl;

static const char vertex_shader[] = R"(
    #version 120
    attribute vec3 vertex;
    attribute vec3 normal;
    uniform vec4 q;
	uniform float sf;
    varying vec3 out_coord;
    varying vec3 out_normal;
	vec4 mult_q(vec4 q1, vec4 q2) {
	  vec4 q;
      q.w = q1.w * q2.w - q1.x * q2.x - q1.y * q2.y - q1.z * q2.z;
      q.x = q1.w * q2.x + q1.x * q2.w + q1.y * q2.z - q1.z * q2.y;
	  q.y = q1.w * q2.y - q1.x * q2.z + q1.y * q2.w + q1.z * q2.x;
	  q.z = q1.w * q2.z + q1.x * q2.y - q1.y * q2.x + q1.z * q2.w;
      return q;
    }
	vec4 inverse_q(vec4 q) {
      vec4 ans = vec4(-q.x, -q.y, -q.z, q.w);
      return ans / (q.w * q.w + q.x * q.x + q.y * q.y + q.z * q.z);
    }
    void main(void)
    {
		vec4 q_inv = inverse_q(q);
		//vec4 out_vq = mult_q(mult_q(q, vec4(vertex, 0.0f)),q_inv);
		//vec4 out_nq = mult_q(mult_q(q, vec4(normal, 0.0f)),q_inv);
		vec4 out_vq = mult_q(q, mult_q(vec4(vertex, 0.0f),q_inv));
		vec4 out_nq = mult_q(q, mult_q(vec4(normal, 0.0f),q_inv));
		out_coord = vec3(out_vq.x, out_vq.y, out_vq.z) * sf;
		out_normal = vec3(out_nq.x, out_nq.y, out_nq.z) * sf;
        gl_Position = vec4(out_coord, 1.0f);
    })";

static const char fragment_shader[] = R"(
    #version 120
    varying vec3 out_coord;
    varying vec3 out_normal;
    uniform vec3 d_color;
    uniform vec3 light_pos = vec3(0.0, 0.0, -10.0f);
    void main(void)
    {
        gl_FragColor = vec4((clamp(dot(normalize(light_pos - out_coord), normalize(out_normal)), 0.0f, 1.0f) + 0.3) * d_color, 1.0f);
    })";

WidgetGL::WidgetGL(QWidget* parent)
	: QOpenGLWidget(parent) {
	QSurfaceFormat format;
	format.setRenderableType(QSurfaceFormat::OpenGL);
	format.setDepthBufferSize(32);
	setFormat(format);
}

void WidgetGL::initializeGL() {
	initializeOpenGLFunctions();
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);
	glDepthMask(GL_TRUE);

	static const std::map<GLenum, std::string> shaders = {
		{ GL_VERTEX_SHADER, vertex_shader },
		{ GL_FRAGMENT_SHADER, fragment_shader} };

	program = std::make_unique<Program>(shaders);
}

void WidgetGL::resizeGL(int width, int height) {
	glViewport(0, 0, width, height);
}

void WidgetGL::paintGL() {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	if (!obj_data) {
		return;
	}

	const float sf = 1.0f / (2.0f * obj_data->max_v.maxCoeff());
	//const Matrix3f rot = q.toRotationMatrix() * sf;
	for (const auto& item : obj_data->group_map) {
		if (!item.second.visible) {
			continue;
		}
		auto& group = item.second;
		const auto itr = obj_data->mat_map.find(group.material);
		const auto material = (itr == obj_data->mat_map.end()) ? ObjReader::Material() : itr->second;
		// Render the group
		Program::Use use(*program);
		program->setUniform("q", q.w(), q.x(), q.y(), q.z());
		program->setUniform("sf", sf);
		program->setUniform("d_color", material.Kd);
		program->setAttribute("vertex", group.vertex_buffer);
		program->setAttribute("normal", group.normal_buffer);
		program->enableVertexAttribArray("vertex");
		program->enableVertexAttribArray("normal");
		glDrawArrays(GL_TRIANGLES, 0, group.vertex_buffer.size());
		program->disableVertexAttribArray("vertex");
		program->disableVertexAttribArray("normal");
	}
}

void WidgetGL::set(const ObjReader::Data& data) {
	makeCurrent();
	obj_data = std::make_unique<opengl::ObjData>(data);
	doneCurrent();
	repaint();
}

void WidgetGL::setVisibility(const std::string& name, const bool visible) {
	auto itr = obj_data->group_map.find(name);
	if (itr != obj_data->group_map.end()) {
		if (itr != obj_data->group_map.end()) {
			itr->second.visible = visible;
			repaint();
		}
	}
}

void WidgetGL::mousePressEvent(QMouseEvent* event) {
	//qDebug() << "PRESSDOWN";
	m_flag = true;
	old_v.x() = static_cast<float>(event->x()) / static_cast<float>(width() / 2.0f) - 1.0f;
	old_v.y() = 1.0f - static_cast<float>(event->y()) / static_cast<float>(height() / 2.0f);
	old_v.z() = -static_cast<float>(sqrt(abs(2 - (old_v.x() * old_v.x()) - (old_v.y() * old_v.y()))));
}

void WidgetGL::mouseReleaseEvent(QMouseEvent* event) {
	//qDebug() << "Release";
	m_flag = false;
}

void WidgetGL::mouseMoveEvent(QMouseEvent* event) {
	if (!m_flag) {
		return;
	}
	new_v.x() = static_cast<float>(event->x()) / static_cast<float>(width() / 2.0f) - 1.0f;
	new_v.y() = 1.0f - static_cast<float>(event->y()) / static_cast<float>(height() / 2.0f);
	new_v.z() = -static_cast<float>(sqrt(abs(2 - (new_v.x() * new_v.x()) - (new_v.y() * new_v.y()))));
	//qDebug() << "New_v -> " << new_v.x() << ", " << new_v.y() << ", " << new_v.z();
	Vector3f axis = old_v.cross(new_v).normalized();
	//qDebug() << "axis -> " << axis.x() << ", " << axis.y() << ", " << axis.z();
	float theta = acos(old_v.normalized().dot(new_v.normalized()));
	//qDebug() << "Theta -> " << theta;
	float cos_theta = cos(theta);
	float sin_theta = sin(theta);
	if (theta > DEG_TO_RAD(0.1f)) {
		q = Eigen::AngleAxisf(theta, axis) * q;
		//qDebug() << "Quaternion -> " << q.w() << " + " << q.x() << "i + " << q.y() << "j + " << q.z() << "k";
		old_v = new_v;
	}
	repaint();
}