#include "graph_image.h"

#include <QPen>
#include <QPainter>

QImage createGraphTemplate(const size_t width, const size_t height, const GraphParams& params) {
  char str[100];
  QImage image(width, height, QImage::Format_ARGB32);
  QPainter painter(&image);
  painter.fillRect(QRect(0, 0, width, height), QColor(0, 0, 0));
  painter.setPen(QPen(QColor(0xFF, 0xFF, 0xFF)));
  painter.drawLine(0, height / 2, width, height / 2);
  painter.drawLine(0, 0, 0, height);
  size_t tick_width = width / params.x_ticks;
  for (size_t i = 1; i < params.x_ticks; i++) {
    const int x = tick_width * i;
    const float x_val = params.x_start + i * (params.x_end - params.x_start) / params.x_ticks;
    sprintf(str, "%1.2f", x_val);
    painter.drawLine(x, height / 2 - 4, x, height / 2 + 4);
    painter.drawText(QRect(x - 25, height / 2 + 4, 50, 18), str, QTextOption(Qt::AlignHCenter | Qt::AlignVCenter));
  }
  size_t tick_height = height / params.y_ticks;
  for (size_t i = 1; i < params.y_ticks; i++) {
    const int y = height - tick_height * i;
    const float y_val = params.y_start + i * (params.y_end - params.y_start) / params.y_ticks;
    sprintf(str, "%1.2f", y_val);
    if (y_val != 0.0f) painter.drawLine(0, y, 8, y);
    if (y_val != 0.0f) painter.drawText(QRect(5, y - 9, 50, 18), str, QTextOption(Qt::AlignHCenter | Qt::AlignVCenter));
  }
  painter.drawText(image.rect(), params.title, QTextOption(Qt::AlignHCenter | Qt::AlignTop));
  return image;
}
