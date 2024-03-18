#pragma once

#include <iostream>
#include <Eigen/Core>

template <typename T>
struct Quaternion {
  Quaternion();
  Quaternion(const T w, const T x, const T y, const T z);

  static Quaternion<T> axisAngle(const T nx, const T ny, const T nz, const T theta);

  T x() const { return data[0]; } //i - imag
  T y() const { return data[1]; } //j - imag
  T z() const { return data[2]; } //k - imag
  T w() const { return data[3]; } //a - real

  T norm() const;
  Quaternion<T> inverse() const;
  Quaternion<T> conjugate() const;

  Quaternion<T> operator*(const T scalar) const;
  Quaternion<T> operator/(const T scalar) const;

  void normalize();
  Quaternion<T> normalized() const;

  T data[4];
};

// Forward declaration

template <typename T>
Quaternion<T> operator+(const Quaternion<T>& q1, const Quaternion<T>& q2);

template <typename T>
Quaternion<T> operator-(const Quaternion<T>& q1, const Quaternion<T>& q2);

template <typename T>
Quaternion<T> operator*(const Quaternion<T>& q1, const Quaternion<T>& q2);

template <typename T>
Quaternion<T> operator/(const Quaternion<T>& q1, const Quaternion<T>& q2);

template <typename T>
Eigen::Matrix<T, 3, 1> operator*(const Quaternion<T>& q, const Eigen::Matrix<T, 3, 1>& p);

template <typename T>
std::ostream& operator<< (std::ostream& stream, const Quaternion<T>& q);

// Implementations

template <typename T>
Quaternion<T>::Quaternion() {
  data[0] = data[1] = data[2] = 0.0;
  data[3] = 1.0;
}

template <typename T>
Quaternion<T>::Quaternion(const T w, const T x, const T y, const T z) {
  data[0] = x;
  data[1] = y;
  data[2] = z;
  data[3] = w;
}


template <typename T>
T Quaternion<T>::norm() const {
	return sqrt(w**2 + x**2 + y**2 + z**2);
}

template <typename T>
Quaternion<T> Quaternion<T>::axisAngle(const T nx, const T ny, const T nz, const T theta) {
	Eigen::Vector<T, 3> u{nx, ny, nz};
	u.normalize();
	return Quaternion<T>(cos(theta / 2), u[0] * sin(theta / 2), u[1] * sin(theta / 2), u[2] * sin(theta / 2));
}

template <typename T>
Quaternion<T> Quaternion<T>::inverse() const {
	return conjugate() / (w() * w() + x() * x() + y() * y() + z() * z());
}

template <typename T>
Quaternion<T> Quaternion<T>::conjugate() const {
	return Quaternion<T>(w(), -x(), -y(), -z());
}

template <typename T>
Quaternion<T> Quaternion<T>::operator*(const T scalar) const {
	return Quaternion<T>(scalar * w(), scalar * x(), scalar * y(), scalar * z());
}

template <typename T>
Quaternion<T> Quaternion<T>::operator/(const T scalar) const {
	return Quaternion<T>(w() / scalar, x() / scalar, y() / scalar, z() / scalar);
}

template <typename T>
void Quaternion<T>::normalize() {
	T n = norm();
	data[0] = data[0] / n;
	data[1] = data[1] / n;
	data[2] = data[2] / n;
	data[3] = data[3] / n;
}

template <typename T>
Quaternion<T> Quaternion<T>::normalized() const {
	return Quaternion<T>(w(), x(), y(), z()) / norm();
}

template <typename T>
Quaternion<T> operator+(const Quaternion<T>& q1, const Quaternion<T>& q2) {
  return Quaternion<T>(q1.w()+q2.w(), q1.x()+q2.x(), q1.y()+q2.y(), q1.z()+q2.z());
}

template <typename T>
Quaternion<T> operator-(const Quaternion<T>& q1, const Quaternion<T>& q2) {
  return Quaternion<T>(q1.w()-q2.w(), q1.x()-q2.x(), q1.y()-q2.y(), q1.z()-q2.z());
}

template <typename T>
std::ostream& operator<< (std::ostream& stream, const Quaternion<T>& q) {
  stream << q.w() << "+" << q.x() << "i+" << q.y() << "j+" << q.z() << "k";
  return stream;
}

template <typename T>
Quaternion<T> operator*(const Quaternion<T>& q1, const Quaternion<T>& q2) {
	const T a = q1.w(), b = q1.x(), c = q1.y(), d = q1.z(), e = q2.w(), f = q2.x(), g = q2.y(), h = q2.z();
	return { a * e - b * f - c * g - d * h,
			a * f + b * e + c * h - d * g,
			a * g - b * h + c * e + d * f,
			a * h + b * g - c * f + d * e };
}

template <typename T>
Quaternion<T> operator/(const Quaternion<T>& q1, const Quaternion<T>& q2) {
	return q1 * q2.inverse();
}

template <typename T>
Eigen::Matrix<T, 3, 1> operator*(const Quaternion<T>& q, const Eigen::Matrix<T, 3, 1>& p) {
	Quaternion<T> ans = q * Quaternion<T>(0, p[0], p[1], p[2]) * q.inverse();
	return { ans.x(), ans.y(), ans.z() };
}