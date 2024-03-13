#include "client.h"
#include <iostream>

#include "imu_data.h"

Client::Client(const QString& ip_addr, const int port)
     : QObject(nullptr),
       tcp_socket(this) {
  connect(&tcp_socket, SIGNAL(readyRead()), SLOT(readData()));
  tcp_socket.connectToHost(ip_addr, port);
}

void Client::readData() {
  QDataStream in(&tcp_socket);
  in.setVersion(QDataStream::Qt_4_0);
  in.startTransaction();
  ImuData data;
  in >> data;
  if (!in.commitTransaction())
    return;
  static int count = 0;
  std::cout << count++ << " : (" 
            << data.gyr.transpose() << "), ("
            << data.acc.transpose() << "), ("
            << data.mag.transpose() << ")" << std::endl;
}
