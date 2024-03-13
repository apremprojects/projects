#pragma once

#include <QDataStream>
#include <Eigen/Core>

struct ImuData {
  Eigen::Vector3i gyr;
  Eigen::Vector3i acc;
  Eigen::Vector3i mag;
};

inline QDataStream& operator<<(QDataStream& out, const ImuData& data) {
  out << data.gyr.x() << data.gyr.y() << data.gyr.z();
  out << data.acc.x() << data.acc.y() << data.acc.z();
  out << data.mag.x() << data.mag.y() << data.mag.z();
  return out;
}

inline QDataStream& operator>>(QDataStream& in, ImuData& data) {
  in >> data.gyr.x() >> data.gyr.y() >> data.gyr.z();
  in >> data.acc.x() >> data.acc.y() >> data.acc.z();
  in >> data.mag.x() >> data.mag.y() >> data.mag.z();
  return in;
}
