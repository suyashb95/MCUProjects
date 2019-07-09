#include <stdint.h>
#include <LCD.h>
#include <SPI.h>

int zOff = 150;
int xOff = 0;
int yOff = 0;
int cSize = 50;
int view_plane = 32;
float angle = PI/100;

float cube3d[8][3] = {
  {xOff - cSize,yOff + cSize,zOff - cSize},
  {xOff + cSize,yOff + cSize,zOff - cSize},
  {xOff - cSize,yOff - cSize,zOff - cSize},
  {xOff + cSize,yOff - cSize,zOff - cSize},
  {xOff - cSize,yOff + cSize,zOff + cSize},
  {xOff + cSize,yOff + cSize,zOff + cSize},
  {xOff - cSize,yOff - cSize,zOff + cSize},
  {xOff + cSize,yOff - cSize,zOff + cSize}
};
unsigned char cube2d[8][2];

void setup() {
  // put your setup code here, to run once:

    SPI.setDataMode(SPI_MODE0);
    SPI.setBitOrder(MSBFIRST);
    SPI.begin();
    
    Tft.lcd_init();
    Tft.lcd_clear_screen(BLACK);  
}

void loop() {
  yrotate(2*PI - angle);
  printcube();
  draw_cube();
  clear_cube();
  //Tft.lcd_fill_rect_fast(77, 118, 60, 65, BLACK);
}

void yrotate(float q) {
  float tx,tz,temp;
  for(byte i = 0; i < 8; i++) {
    tx = cube3d[i][0] - xOff;
    tz = cube3d[i][2] - zOff;
    temp = tz * cos(q) - tx * sin(q);
    tx = tz * sin(q) + tx * cos(q);
    tz = temp;
    cube3d[i][0] = tx + xOff;
    cube3d[i][2] = tz + zOff;
  }
}

void printcube() {
  //calculate 2d points
  for(byte i = 0; i < 8; i++) {
    cube2d[i][0] = (unsigned char)((cube3d[i][0] * view_plane / cube3d[i][2]) + (240/2));
    cube2d[i][1] = (unsigned char)((cube3d[i][1] * view_plane / cube3d[i][2]) + (320/2));
  }  
}

void draw_cube() {
  Tft.lcd_draw_line_fast(cube2d[0][0],cube2d[0][1],cube2d[1][0],cube2d[1][1],RED);
  Tft.lcd_draw_line_fast(cube2d[0][0],cube2d[0][1],cube2d[2][0],cube2d[2][1],RED);
  Tft.lcd_draw_line_fast(cube2d[0][0],cube2d[0][1],cube2d[4][0],cube2d[4][1],RED);
  Tft.lcd_draw_line_fast(cube2d[1][0],cube2d[1][1],cube2d[5][0],cube2d[5][1],RED);
  Tft.lcd_draw_line_fast(cube2d[1][0],cube2d[1][1],cube2d[3][0],cube2d[3][1],RED);
  Tft.lcd_draw_line_fast(cube2d[2][0],cube2d[2][1],cube2d[6][0],cube2d[6][1],RED);
  Tft.lcd_draw_line_fast(cube2d[2][0],cube2d[2][1],cube2d[3][0],cube2d[3][1],RED);
  Tft.lcd_draw_line_fast(cube2d[4][0],cube2d[4][1],cube2d[6][0],cube2d[6][1],RED);
  Tft.lcd_draw_line_fast(cube2d[4][0],cube2d[4][1],cube2d[5][0],cube2d[5][1],RED);
  Tft.lcd_draw_line_fast(cube2d[7][0],cube2d[7][1],cube2d[6][0],cube2d[6][1],RED);
  Tft.lcd_draw_line_fast(cube2d[7][0],cube2d[7][1],cube2d[3][0],cube2d[3][1],RED);
  Tft.lcd_draw_line_fast(cube2d[7][0],cube2d[7][1],cube2d[5][0],cube2d[5][1],RED);
}

void clear_cube() {
  Tft.lcd_draw_line_fast(cube2d[0][0],cube2d[0][1],cube2d[1][0],cube2d[1][1],BLACK);
  Tft.lcd_draw_line_fast(cube2d[0][0],cube2d[0][1],cube2d[2][0],cube2d[2][1],BLACK);
  Tft.lcd_draw_line_fast(cube2d[0][0],cube2d[0][1],cube2d[4][0],cube2d[4][1],BLACK);
  Tft.lcd_draw_line_fast(cube2d[1][0],cube2d[1][1],cube2d[5][0],cube2d[5][1],BLACK);
  Tft.lcd_draw_line_fast(cube2d[1][0],cube2d[1][1],cube2d[3][0],cube2d[3][1],BLACK);
  Tft.lcd_draw_line_fast(cube2d[2][0],cube2d[2][1],cube2d[6][0],cube2d[6][1],BLACK);
  Tft.lcd_draw_line_fast(cube2d[2][0],cube2d[2][1],cube2d[3][0],cube2d[3][1],BLACK);
  Tft.lcd_draw_line_fast(cube2d[4][0],cube2d[4][1],cube2d[6][0],cube2d[6][1],BLACK);
  Tft.lcd_draw_line_fast(cube2d[4][0],cube2d[4][1],cube2d[5][0],cube2d[5][1],BLACK);
  Tft.lcd_draw_line_fast(cube2d[7][0],cube2d[7][1],cube2d[6][0],cube2d[6][1],BLACK);
  Tft.lcd_draw_line_fast(cube2d[7][0],cube2d[7][1],cube2d[3][0],cube2d[3][1],BLACK);
  Tft.lcd_draw_line_fast(cube2d[7][0],cube2d[7][1],cube2d[5][0],cube2d[5][1],BLACK);
}
