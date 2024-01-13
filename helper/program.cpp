#include "program.h"
#include <glog/logging.h>

namespace opengl {

class strPtr {
public:
  strPtr(const std::string& str) :p(str.c_str()) {};
  operator const GLchar** () { return &p; };

private:
  const GLchar* p;
};

Program::Use::Use(Program& program) :program(program) {
  program.use();
}

Program::Use::~Use() {
  program.release();
}

Program::Program(const std::map<GLenum, std::string>& shaders)
    : program(0) {
  initializeOpenGLFunctions();
  program = glCreateProgram();
  for (const auto& item : shaders) {
    GLuint shader = glCreateShader(item.first);
    glShaderSource(shader, 1, strPtr(item.second), 0);
    compile(shader);
    glAttachShader(program, shader);
    glDeleteShader(shader);
  }
  glLinkProgram(program);
}

Program::~Program() {
  glDeleteProgram(program);
}

void Program::use() {
  glUseProgram(program);
}

void Program::release() {
  glUseProgram(0);
}

void Program::compile(const GLuint shader) {
  glCompileShader(shader);
  GLint status = 0;
  glGetShaderiv(shader, GL_COMPILE_STATUS, &status);
  if (status != GL_TRUE) {
    GLint length;
    glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &length);
    std::string str_err(length, 0);
    glGetShaderInfoLog(shader, length, nullptr, &str_err[0]);
    LOG(FATAL) << "Shader Error:\n\n" << str_err;
  }
}

void Program::setUniform(const char* name, const int value) {
    const GLuint loc = glGetUniformLocation(program, name);
    glUniform1i(loc, value);
}

void Program::setUniform(const char* name, const float value) {
  const GLuint loc = glGetUniformLocation(program, name);
  glUniform1f(loc, value);
}

void Program::setUniform(const char* name, const Eigen::Vector2f& v) {
  const GLint loc = glGetUniformLocation(program, name);
  glUniform2f(loc, v.x(), v.y());
}

void Program::setUniform(const char* name, const Eigen::Vector3f& v) {
  const GLint loc = glGetUniformLocation(program, name);
  glUniform3f(loc, v.x(), v.y(), v.z());
}
void Program::setUniform(const char* name, const Eigen::Vector4f& v) {
    const GLint loc = glGetUniformLocation(program, name);
    glUniform4f(loc, v.x(), v.y(), v.z(), v.w());
}

void Program::setUniform(const char* name, const Eigen::Quaternionf& q, const bool transpose) {
  setUniform(name, q.toRotationMatrix(), transpose);
}

void Program::setUniform(const char* name, const Eigen::Matrix3f& matrix, const bool transpose) {
  const GLuint loc = glGetUniformLocation(program, name);
  glUniformMatrix3fv(loc, 1, transpose, &matrix.coeff(0));
}

void Program::setUniform(const char* name, const Eigen::Matrix4f& matrix, const bool transpose) {
    const GLuint loc = glGetUniformLocation(program, name);
    glUniformMatrix4fv(loc, 1, transpose, &matrix.coeff(0));
}

void Program::setUniform(const char* name, const Eigen::Isometry3f& transform, const bool transpose) {
  const GLuint loc = glGetUniformLocation(program, name);
  glUniformMatrix4fv(loc, 1, transpose, &transform.matrix().coeff(0));
}

void Program::enableVertexAttribArray(const char* name) {
  const GLuint loc = glGetAttribLocation(program, name);
  glEnableVertexAttribArray(loc);
}

void Program::disableVertexAttribArray(const char* name) {
  const GLuint loc = glGetAttribLocation(program, name);
  glDisableVertexAttribArray(loc);
}

}
