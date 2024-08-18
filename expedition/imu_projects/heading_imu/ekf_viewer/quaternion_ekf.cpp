#include "quaternion_ekf.h"

#include <iostream>

using Eigen::Matrix;
using Eigen::Vector;
using Eigen::Vector3f;
using Eigen::Vector4f;
using Eigen::Matrix3f;
using Eigen::Matrix4f;
using Eigen::Quaternionf;

Matrix3f toSkewSymMatrix(const Vector3f& v) {
    Matrix3f m{
        {0, -v.z(), v.y()},
        {v.z(), 0, -v.x()},
        {-v.y(), v.x(), 0}
    };
    return m;
}

Matrix4f toF(const Vector3f& w, const float dt) {//review later
  Matrix4f F = Matrix4f::Identity();
  F.block<1, 3>(0, 1) = -(dt / 2.0) * w;
  F.block<3, 1>(1, 0) = (dt / 2.0) * w;
  F(1, 2) = (dt / 2.0) * w.z();
  F(1, 3) = -(dt / 2.0) * w.y();
  F(2, 3) = (dt / 2.0) * w.x();
  F(2, 1) = -(dt / 2.0) * w.z();
  F(3, 1) = (dt / 2.0) * w.y();
  F(3, 2) = -(dt / 2.0) * w.x();
  return F;
}

Matrix<float, 4, 3> toW(const Quaternionf& q, const float dt) {
    Matrix<float, 4, 3> W{
        {-q.x(), -q.y(), -q.z()},
        {q.w(), -q.z(), q.y()},
        {q.z(), q.w(), -q.x()},
        {-q.y(), q.x(), q.w()}
    };
    W *= dt / 2.0;
    return W;
}

Matrix<float, 6, 4> toH(const Quaternionf& q, const Vector3f& g, const Vector3f& r) {
    Matrix<float, 6, 4> H = Matrix<float, 6, 4>::Zero();
    //6 rows, 4 columns
    Matrix<float, 3, 1> u_g = toSkewSymMatrix(g) * q.vec();
    Matrix<float, 3, 1> u_r = toSkewSymMatrix(r) * q.vec();
    //std::cout << "here -> " << u_g << "l\n";// << ", " << u_r << "\n";

    H.block<3, 1>(0, 0) = u_g;
    //std::cout << "H -> " << H << "\n";
    H.block<3, 1>(3, 0) = u_r;
    H.block<3, 3>(0, 1) = toSkewSymMatrix(u_g + q.w() * g) + (q.vec().dot(g) * Matrix3f::Identity()) - (g * q.vec().transpose());
    H.block<3, 3>(3, 1) = toSkewSymMatrix(u_r + q.w() * r) + (q.vec().dot(r) * Matrix3f::Identity()) - (r * q.vec().transpose());
    return H;
}

Matrix<float, 6, 6> toR(const Vector3f& var_a, const Vector3f& var_m) {
    Matrix<float, 6, 6> R = Matrix<float, 6, 6>::Zero();
    R.diagonal() << var_a, var_m;
    return R;
}

QuaternionEkf::QuaternionEkf(const Vector3f& r_norm_ned, 
                             const Vector3f& var_w, 
                             const Vector3f& var_a, 
                             const Vector3f& var_m)
  : g(0.0f, 0.0f, 1.0f),
    r(r_norm_ned),
    sigma_w(var_w.asDiagonal()),
    R(toR(var_a, var_m)) {}

Quaternionf QuaternionEkf::predict(const Quaternionf& q, const Vector3f& w, const float dt) {
    const Quaternionf dq = (q * Quaternionf(0, 0.5 * w.x() * dt, 0.5 * w.y() * dt, 0.5 * w.z() * dt));
    Quaternionf q_pred(q.w() + dq.w(), q.x() + dq.x(), q.y() + dq.y(), q.z() + dq.z());
    q_pred.normalize();
    const Matrix4f F = toF(w, dt);
    const Matrix<float, 4, 3> W = toW(q, dt);
    const Matrix4f Q = W * sigma_w * W.transpose();
    P = F * P * F.transpose() + Q;

    /*
    std::cout << "q_pred -> " << q_pred << "\n";
    std::cout << "F -> " << F << "\n";
    std::cout << "W -> " << W << "\n";
    std::cout << "Q -> " << Q << "\n";
    std::cout << "P -> " << P << "\n";
    */

    return q_pred;
}

Quaternionf QuaternionEkf::update(const Quaternionf& q, const Vector3f& a, const Vector3f& m) {
    //return q;
    Eigen::Matrix<float, 6, 1> z;
    z << a, m;

    Matrix<float, 6, 1> h;
    h << q.conjugate() * g, q.conjugate() * r;

    const Matrix<float, 6, 4> H = toH(q, g, r);
    const Matrix<float, 6, 6> S = H * P * H.transpose() + R;
    const Matrix<float, 4, 6> K = P * H.transpose() * S.inverse(); //bad
    //const Matrix3f C = q.toRotationMatrix();
    const auto v = z - h; //bad
    P = (Matrix4f::Identity() - K * H) * P;//bad

    const Matrix<float, 4, 1> dq = K * v;
    const Quaternionf q_new = Quaternionf(
        q.w() + dq(0, 0),
        q.x() + dq(1, 0),
        q.y() + dq(2, 0),
        q.z() + dq(3, 0)
    ).normalized();

    /*
    std::cout << "S_inv -> " << S.inverse() << "\n";
    std::cout << "z -> " << z << "\n";
    std::cout << "H -> " << H << "\n";
    std::cout << "S -> " << S << "\n";
    std::cout << "P -> " << P << "\n";
    std::cout << "R -> " << R << "\n";
    std::cout << "v -> " << v << "\n";
    std::cout << "K -> " << K << "\n";
    std::cout << "qn -> " << q_new << "\n";
    */

    return q_new;
}
