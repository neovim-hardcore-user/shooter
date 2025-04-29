#include <ESP32Lib.h>
#include <Ressources/Font6x8.h>
#include "matrix_vector.h"
#include "rasterizer.h"
#include "ranger.h"
#include "mesh.h"


VGA14Bit vga;


static uint frame_time = 0;
static uint current_time = 0;
static float delta_time = 0;


f_matrix perspective;
f_matrix transform;






void setup() {
  vga.setFrameBufferCount(2);
  vga.init(vga.MODE200x150, vga.VGABlackEdition);

  vga.setTextColor(vga.RGB(0, 255, 0));

  Serial.begin(115200);
  while (!Serial) {;}

  perspective.setTranslate(100.0, 75.0, 0.0);
  perspective.multPerspective(120.0, 5.0, 10.0, 200);
}

float pitch = 0;
float yaw = 0;

float scale = 0;

float scale_x = 0;
float scale_y = 0;
float scale_z = 0;
float translate_x = 0;

byte keys = 0;



void loop() {
  /*if (Serial.available() >= 9) {
    byte buffer[9];
    Serial.readBytes(buffer, 9);

    int receivedInts[2];

    memcpy(&receivedInts[0], &buffer[0], 4);
    memcpy(&receivedInts[1], &buffer[4], 4);

    keys = buffer[8];


    yaw -= receivedInts[0] * 0.003f;
    pitch += receivedInts[1] * 0.003f;
  }*/



  uint time = millis();
  frame_time = time - current_time;
  current_time = time;


  yaw += frame_time * 0.0003f;
  pitch = sin(time * 0.001f) * 1.3;

  translate_x = sin(time * 0.0005f) * 0.4;

  const float ms = 800.33f;


  scale = sin(time * (1.0f/ms * PI * 2.0f)) * (sin(time * (1.0f/ms * PI)) * 0.5 + 0.5);

  scale_x = scale * 0.1 + 1.0;
  scale_y = scale * 0.3 + 1.0;
  scale_z = scale * 0.1 + 1.0;

  

  for (int y = 0; y < vga.yres; y++)
    memset(vga.backBuffer[y], 0x0, vga.xres * sizeof(uint16_t));


  memcpy(transform.m, perspective.m, 16 * sizeof(float));

  
  transform.multTranslate(translate_x, 0, 2.1);

  //transform.multRotateX(pitch);
  transform.multRotateY(pitch);

  transform.multScale(scale_x, scale_y, scale_z);

  

  
  

  for (int x = 0; x < vertex_count; ++x) {
    vec4* tv = &transformed_vertices[x];
    *tv = transform * vertices[x];

    float inv_w = 1.0f / tv->w;

    tv->x = tv->x * inv_w;
    tv->y = tv->y * inv_w;
    //tv->z = tv->z;
  }

  for (int x = 0; x < triangle_count; ++x) {
    draw_flat_triangle(
      transformed_vertices[faces[x][0]], 
      transformed_vertices[faces[x][1]], 
      transformed_vertices[faces[x][2]], 
      colors[x],
      vga.backBuffer
    );
  }




  delta_time = (float)(delta_time * 200 + frame_time) / 201;
  vga.setFont(Font6x8);
  vga.setCursor(0, 0);
  vga.print((int)(1000.0f / delta_time));

  vga.show();
}
