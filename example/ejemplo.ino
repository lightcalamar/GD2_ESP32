/************************************************
 *  Testing 5" display with FT810 graphics controller
 *  and ESP32 processor
 *  
 *  FT81x graphics driver is initially copied from 
 *  jamesbowman / gd2-lib
 *  (https://github.com/jamesbowman/gd2-lib.git)
 *  Reduced and modified to compile with ESP32
 *  
 *  Connections:
 *  leaming README.md
 *
 * 
 ************************************************/

#include <SPI.h>
#include <GD2ESP32.h>
#include <Wire.h>
#include "walk_assets.h"

void setup()
{
  //wdt_disable();
  Serial.begin(9600);
  Serial.println("Initializing WeatherNG graphics controller FT81x...");
  GD.begin(0);
  LOAD_ASSETS();
  Serial.println("Done.");
}

void drawMainText() {

  GD.ColorRGB(255,255,255);
  GD.ColorA(230);

  // put FT81x font 33 in slot 1
  GD.cmd_romfont(1, 33);
  
  // Text centered on screen
  GD.cmd_text(GD.w / 2, GD.h / 2 - 40,   1, OPT_CENTER, "ESP32 with Gameduino2");
  GD.cmd_text(GD.w / 2, GD.h / 2 + 20 , 30, OPT_CENTER, "Processor: ESP32 WROOM-2");
  GD.cmd_text(GD.w / 2, GD.h / 2 + 50 , 30, OPT_CENTER, "Graphics library: Modified Gameduino2/3");
  GD.cmd_text(GD.w / 2, GD.h / 2 + 80 , 29, OPT_CENTER, "Display controller board: HOTMCU with FT810");
  GD.cmd_text(GD.w / 2, GD.h / 2 + 110, 29, OPT_CENTER  "Display: 5'' 800x480");

}


void drawCircle(word x, word y, word pixels) {
  GD.PointSize(16 * pixels);
  GD.Begin(POINTS);
  //GD.Vertex2ii(x, y); // vertex2ii only handles up to 512 pixels !!! Use 2f instead
  GD.Vertex2f(x * 16, y * 16);
}

void drawSprite(int16_t x, int16_t y, byte handle, byte cell) {
  GD.SaveContext();
  GD.Begin(BITMAPS);
  GD.ColorRGB(255,255,255);
  GD.ColorA(255);
  GD.Vertex2ii(x, y, handle, cell);
  GD.RestoreContext();
}

void drawRandomCircles(int nr) {
  GD.SaveContext();
  GD.Begin(POINTS);
  for (int i = 0; i < nr; i++) {
    GD.PointSize(GD.random(16*50));
    GD.ColorRGB(GD.random(256), GD.random(256), GD.random(256));
    GD.ColorA(GD.random(256));
    GD.Vertex2ii(GD.random(800), GD.random(480));
  }
  GD.RestoreContext();
}

/*
// Trying to solve problem with constant watchdog resets
// ref. http://internetofhomethings.com/homethings/?p=396
void delayWithYield(int ms) {
  int i;
  for(i=1;i!=ms;i++) {
    delay(1);
    if(i%50 == 0) {
      ESP.wdtFeed(); 
      yield();
    }
  }
}
*/
int outsize_screen = 20; // how many pixels outside screen we will use
int walker_figure = 0;
int walker_position = -outsize_screen;
int live_counter;

void loop()
{

  Serial.println("looping...");
  GD.ClearColorRGB(0x000000);
  GD.Clear();

  drawRandomCircles(2);
  yield();
  drawMainText();

  walker_position = walker_position + 4;
  if (walker_position > 800 + outsize_screen) {
    walker_position = - outsize_screen;
  }

  walker_figure = walker_figure + 1;
  if (walker_figure > 7) {
    walker_figure = 0;
  }
  drawSprite(walker_position, 100, WALK_HANDLE, walker_figure); // animate

  GD.ColorRGB(255,255,255);
  GD.ColorA(255);
  GD.cmd_number(15, 60 , 29, OPT_CENTERY, live_counter);

  GD.swap();    
//  delayWithYield(100);
  live_counter ++;
}
