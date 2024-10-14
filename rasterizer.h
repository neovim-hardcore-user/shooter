#pragma once
#include "matrix_vector.h"
#include <ESP32Lib.h>


inline void draw_flat_triangle(vec4 v1, vec4 v2, vec4 v3, uint16_t c, short unsigned int** fb) {
  if (v1.y > v2.y) std::swap(v1, v2);
  if (v1.y > v3.y) std::swap(v1, v3);
  if (v2.y > v3.y) std::swap(v2, v3);

  int x_start = v1.x << 16;
  int x_end = x_start;

  int d21 = v2.y - v1.y;
  int d31 = v3.y - v1.y;
  int d32 = v3.y - v2.y;

  int slope1 = (d31 != 0) ? ((v3.x - v1.x) << 16) / d31 : 0;
  int slope2 = (d21 != 0) ? ((v2.x - v1.x) << 16) / d21 : 0;
  int slope3 = (d32 != 0) ? ((v3.x - v2.x) << 16) / d32 : 0;

  int y = v1.y;
  for (; y < v2.y; ++y) {
    int x_min = x_start >> 16;
    int x_max = x_end >> 16;

    if (x_min > x_max) std::swap(x_min, x_max);


    short unsigned int* fby = fb[y];
    for (int x = x_min; x < x_max; ++x) {
      fby[x ^ 1] = c;
    }

    x_start += slope2;
    x_end += slope1;
  }

  x_start = v2.x << 16;
  for (; y < v3.y; ++y) {
    int x_min = x_start >> 16;
    int x_max = x_end >> 16;

    if (x_min > x_max) std::swap(x_min, x_max);
    
    short unsigned int* fby = fb[y];
    for (int x = x_min; x < x_max; ++x) {
      fby[x ^ 1] = c;
    }

    x_start += slope3;
    x_end += slope1;
  }
}

