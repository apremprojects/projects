#include "quaternion.h"

#include <cassert>
#include <iostream>

#include <Eigen/Geometry>

#define TO_RAD(theta) ((theta/45.0)*std::atan(1.0f))

int main(int argc, char **argv) {
  {
    Quaternion<float> q1(1.0f, 2.0f, 3.0f, 4.0f);
    Quaternion<float> q2(5.0f, 6.0f, 7.0f, 8.0f);
    const auto q = q1 + q2;
    assert(q.w() == 6.0f);
    assert(q.x() == 8.0f);
    assert(q.y() == 10.0f);
    assert(q.z() == 12.0f);
    std::cout << "Test quaternion addition passed\n";
  }
  {
    Eigen::Quaternionf q1_eig(0.1f, 0.2f, 0.3f, 0.4f);
    Eigen::Quaternionf q2_eig(0.5f, 0.6f, 0.7f, 0.8f);
    Quaternion<float> q1(0.1f, 0.2f, 0.3f, 0.4f);
    Quaternion<float> q2(0.5f, 0.6f, 0.7f, 0.8f);

    const auto q_eig = q1_eig*q2_eig;
    const auto q = q1*q2;
    assert(q_eig.w() == q.w());
    assert(q_eig.x() == q.x());
    assert(q_eig.y() == q.y());
    assert(q_eig.z() == q.z());
    std::cout << "Test quaternion multiplication passed\n";
  }
  {
    const auto q_eig = Eigen::Quaternionf(0.5f, 0.6f, 0.7f, 0.8f).inverse();
    const auto q = Quaternion<float>(0.5f, 0.6f, 0.7f, 0.8f).inverse();  
    assert(q_eig.w() == q.w());
    assert(q_eig.x() == q.x());
    assert(q_eig.y() == q.y());
    assert(q_eig.z() == q.z());
    std::cout << "Test quaternion inverse passed\n";
  }
  {
    Quaternion<float> q1(0.1f, 0.2f, 0.3f, 0.4f);
    Quaternion<float> q2(0.5f, 0.6f, 0.7f, 0.8f);
    Eigen::Quaternionf q1_eig(0.1f, 0.2f, 0.3f, 0.4f);
    Eigen::Quaternionf q2_eig(0.5f, 0.6f, 0.7f, 0.8f);

    const auto q = q1/q2;
    const auto q_eig = q1_eig*(q2_eig.inverse());

    assert(std::abs(q_eig.w() - q.w()) < 0.0001f);
    assert(std::abs(q_eig.x() - q.x()) < 0.0001f);
    assert(std::abs(q_eig.y() - q.y()) < 0.0001f);
    assert(std::abs(q_eig.z() - q.z()) < 0.0001f);
    std::cout << "Test quaternion division passed\n";
  }
  {
    Eigen::Vector3f vec(0.4f, 0.3f, 0.7f);
    const auto normal = vec.normalized();
    const Eigen::Quaternionf q_eig(Eigen::AngleAxisf(TO_RAD(33.0f), normal));
    const auto q = Quaternion<float>::axisAngle(normal.x(), normal.y(), normal.z(), TO_RAD(33.0f));

    assert(std::abs(q_eig.w() - q.w()) < 0.0001f);
    assert(std::abs(q_eig.x() - q.x()) < 0.0001f);
    assert(std::abs(q_eig.y() - q.y()) < 0.0001f);
    assert(std::abs(q_eig.z() - q.z()) < 0.0001f);
    std::cout << "Test quaternion axis angle passed\n";
  }
  {
    const float theta = TO_RAD(33.0f);
    const auto normal = Eigen::Vector3f(0.4f, 0.3f, 0.7f).normalized();
    const Eigen::Quaternionf q_eig(Eigen::AngleAxisf(theta, normal));
    const auto q = Quaternion<float>::axisAngle(normal.x(), normal.y(), normal.z(), theta);

    const Eigen::Vector3f point(20.0f, 13.0f, 17.0f);
    const auto point_eig_t = q_eig*point;
    const auto point_t = q*point;

    assert(std::abs(point_eig_t.x() - point_t.x()) < 0.00001f);
    assert(std::abs(point_eig_t.y() - point_t.y()) < 0.00001f);
    assert(std::abs(point_eig_t.z() - point_t.z()) < 0.00001f);

    std::cout << "Test quaternion rotate point passed\n";
  }
  return 0;
}
