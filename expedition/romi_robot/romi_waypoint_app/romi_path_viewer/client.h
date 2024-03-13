#pragma once

#include <QObject>
#include <QtNetwork>

class Client : public QObject {
  Q_OBJECT
public:
  explicit Client(const QString& ip_addr, const int port);

private slots:
  void readData();

private:
  QTcpSocket tcp_socket;
};
