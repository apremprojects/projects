#include "quaternion_ekf.h"

#include <iostream>

using Eigen::Vector3f;
using Eigen::Quaternionf;

#define TO_RAD(theta_deg) ((std::atan(1.0f) / 45.0f) * theta_deg)

int main(void) {
  const float mag_inc_angle = TO_RAD(12.0f + (58.0f / 60.0f));
  const float mag_dec_angle = -TO_RAD(60.0f + (53.0f / 60.0f));

  const Vector3f r_norm_ned(cos(mag_dec_angle) * cos(mag_inc_angle), 
                            cos(mag_dec_angle) * sin(mag_inc_angle), 
                            sin(mag_dec_angle));
  QuaternionEkf ekf_q(r_norm_ned, 
                      Vector3f(0.1f, 0.2f, 0.3f), 
                      Vector3f(0.1f, 0.2f, 0.3f), 
                      Vector3f(0.1f, 0.2f, 0.3f));

  ekf_q.update(Quaternionf::Identity(), Vector3f(1.0f, 0.0f, 0.0f), Vector3f(1.0f, 0.0f, 0.0f));
  
  std::cout << r_norm_ned.transpose() << "\n";
  std::cout << std::sqrt(std::pow(r_norm_ned.x(), 2.0f)+std::pow(r_norm_ned.z(), 2.0f)) << "\n";

  Quaternionf q(1.0f, 2.0f, 3.0f, 4.0f);
  std::cout << "-----> " << q.coeffs().transpose() << "\n";

  return 0;
}
