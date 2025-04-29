#pragma once
#include "matrix_vector.h"
#include <ESP32Lib.h>


inline void draw_flat_triangle(vec4 fv1, vec4 fv2, vec4 fv3, uint16_t c, short unsigned int** fb) {
  int_vec2 v1 = fv1.to_int();
  int_vec2 v2 = fv2.to_int();
  int_vec2 v3 = fv3.to_int();
  
  if (v1.y > v2.y) std::swap(v1, v2);
  if (v1.y > v3.y) std::swap(v1, v3);
  if (v2.y > v3.y) std::swap(v2, v3);

  int x_start = v1.x << 16;
  int x_end = x_start;

  int d31 = v3.y - v1.y;
  if (d31 == 0) return;

  int slope1 = ((v3.x - v1.x) << 16) / d31;
  int slope2 = ((v2.x - v1.x) << 16) / std::max(1, v2.y - v1.y);
  int slope3 = ((v3.x - v2.x) << 16) / std::max(1, v3.y - v2.y);


  for (int y = v1.y; y < v2.y; ++y) {
    int x_min = x_start >> 16;
    int x_max = x_end >> 16;

    if (x_min > x_max) std::swap(x_min, x_max);

    for (int x = x_min; x < x_max; ++x) {
      fb[y][x ^ 1] = c;// + fby[x ^ 1];
    }

    x_start += slope2;
    x_end += slope1;
  }

  x_start = v2.x << 16;
  for (int y = v2.y; y < v3.y; ++y) {
    int x_min = x_start >> 16;
    int x_max = x_end >> 16;

    if (x_min > x_max) std::swap(x_min, x_max);
    
    for (int x = x_min; x < x_max; ++x) {
      fb[y][x ^ 1] = c;// + fby[x ^ 1];
    }

    x_start += slope3;
    x_end += slope1;
  }
}

