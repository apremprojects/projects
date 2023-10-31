#pragma once
#include <QImage>

struct GraphParams {
  float x_start;
  float x_end;
  size_t x_ticks;
  float y_start;
  float y_end;
  size_t y_ticks;
  float sampling_period;
  QString title;
};

QImage createGraphTemplate(const size_t width, const size_t height, const GraphParams& params);
