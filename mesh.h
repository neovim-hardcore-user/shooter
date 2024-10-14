#pragma once
#include "matrix_vector.h"
#include "rasterizer.h"


struct static_mesh {
  int triangle_count;
  int vertex_count;

  vec4** transformed_vertices;
  vec3** vertices;

  short unsigned int*** triangles;

  short unsigned int color;

  void transform_vertices(f_matrix matrix) {
    for (int x = 0; x < vertex_count; ++x) {
      auto tv = transformed_vertices[x];
      *tv = matrix * *vertices[x];

      int64_t inv_w = (int64_t(1) << 32) / tv->w;

      tv->x = (tv->x * inv_w >> 32) + 100;
      tv->y = (tv->y * inv_w >> 32) + 75;
      //tv->z = (int64_t(tv->z) << 16) / (tv->w);
    }
  }

  void render_triangles(short unsigned int** fb) {
    for (int x = 0; x < triangle_count; ++x) {
      draw_flat_triangle(
        *transformed_vertices[*faces[x][0]], 
        *transformed_vertices[*faces[x][1]], 
        *transformed_vertices[*faces[x][2]], 
        color, 
        fb
      );
    }
  }

  void render_mesh(f_matrix camera_matrix, short unsigned int** fb) {
    transform_vertices(camera_matrix);

    render_triangles(fb);
  }
};


//struct mesh