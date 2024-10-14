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



f_matrix transform;


uint16_t c = 0;

void setup() {
  vga.setFrameBufferCount(2);
  vga.init(vga.MODE200x150, vga.VGABlackEdition);

  vga.setTextColor(vga.RGB(0, 255, 0));

  Serial.begin(115200);
  while (!Serial) {;}

  
  c = (vga.RGB(255, 45, 0) & vga.RGBMask) | vga.SBits;

  
}

float pitch = 0;
float yaw = 0;

byte keys = 0;



void loop() {
  if (Serial.available() >= 9) {
    byte buffer[9];
    Serial.readBytes(buffer, 9);

    int receivedInts[2];

    memcpy(&receivedInts[0], &buffer[0], 4);
    memcpy(&receivedInts[1], &buffer[4], 4);

    keys = buffer[8];


    yaw -= receivedInts[0] * 0.3f;
    pitch += receivedInts[1] * 0.3f;
  }


  uint time = millis();
  frame_time = time - current_time;
  current_time = time;

  for (int y = 0; y < vga.yres; y++)
    memset(vga.backBuffer[y], 0x0000, vga.xres * sizeof(uint16_t));


  transform.setZero();
  
  transform.setPerspective(120.0, 5.0, 10.0, 190);

  

  
  transform.multTranslate(0, 0, 3.1);

  transform.multRotateX(pitch);
  transform.multRotateY(yaw);


  for (int x = 0; x < vertex_count; ++x) {
    auto tv = &transformed_vertices[x];
    *tv = transform * vertices[x];

    int64_t inv_w = (int64_t(1) << 32) / tv->w;

    tv->x = (tv->x * inv_w >> 32) + 100;
    tv->y = (tv->y * inv_w >> 32) + 75;
    //tv->z = tv->z >> 16;
  }

  if (keys & 0b00000001) {
    c = (vga.RGB(rand()%255, rand()%255, rand()%255) & vga.RGBMask) | vga.SBits;
  }

  for (int x = 0; x < triangle_count; ++x) {
    draw_flat_triangle(
      transformed_vertices[*faces[x][0]], 
      transformed_vertices[*faces[x][1]], 
      transformed_vertices[*faces[x][2]], 
      c, 
      vga.backBuffer
    );
  }




  delta_time = (float)(delta_time * 100 + frame_time) / 101;
  vga.setFont(Font6x8);
  vga.setCursor(0, 0);
  vga.print((int)(1000.0f / delta_time));

  vga.show();
}
