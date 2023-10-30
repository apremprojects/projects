#pragma once
#include <QImage>

struct GraphParams {
  size_t width;
  size_t height;
  float x_start;
  float x_end;
  size_t x_ticks;
  float y_start;
  float y_end;
  size_t y_ticks;
  float sampling_period;
  QString title;
};

QImage createGraphTemplate(const GraphParams& params);
