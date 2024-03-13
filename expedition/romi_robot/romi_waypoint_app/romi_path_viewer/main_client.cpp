#include <QCoreApplication>

#include "client.h"
#include <iostream>

int main(int argc, char *argv[]) {
  if (argc < 3) {
    std::cout << "Insufficient args ..." << std::endl;
    return 0;
  }
  QCoreApplication app(argc, argv);
  Client client(QString(argv[1]), std::stoi(argv[2]));
  return app.exec();
}
