#include "Adafruit_SSD1306.h"
#include <string.h>

#define OLED_RESET D4

void SDD1306_Init();
void SDD1306_ClearDisplay();

void SDD1306_Temp(String T);
void SDD1306_Humi(String H);
void SDD1306_Press(String P);
void SDD1306_Rain(String R);