#include <Eigen/Core>
#include <Eigen/Geometry>

class QuaternionEkf {
public:
  QuaternionEkf(const Eigen::Vector3f& r_norm_ned, 
                const Eigen::Vector3f& var_w, 
                const Eigen::Vector3f& var_a, 
                const Eigen::Vector3f& var_m);

  Eigen::Quaternionf predict(const Eigen::Quaternionf& q, 
                             const Eigen::Vector3f& w, 
                             const float dt);

  Eigen::Quaternionf update(const Eigen::Quaternionf& q, 
                            const Eigen::Vector3f& a,
                            const Eigen::Vector3f& m);

private:
  const Eigen::Vector3f g;
  const Eigen::Vector3f r;
  const Eigen::Matrix3f sigma_w;
  const Eigen::Matrix<float, 6, 6> R;
  Eigen::Matrix4f P = Eigen::Matrix4f::Identity();
};
