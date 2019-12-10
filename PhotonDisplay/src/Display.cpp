#include "Display.h"


Adafruit_SSD1306 oled(OLED_RESET);

void SDD1306_Init()
{
    oled.begin(SSD1306_SWITCHCAPVCC, 0x3C);  // initialize with the I2C addr 0x3D (for the 128x64)
    oled.clearDisplay();
    oled.display();
}
void SDD1306_ClearDisplay(){
    oled.clearDisplay();
    oled.display();
}

void SDD1306_Temp(String T)
{   
   
    oled.clearDisplay();
    oled.setTextColor(WHITE);
    oled.setTextSize(1);
    oled.setCursor(50,16);
    oled.print("Temp:");
    oled.setTextSize(2);
    oled.setCursor(35,34);
    oled.print(String(T,2));
    oled.setTextSize(1);
    oled.setCursor(42,54);
    oled.print("C");
    oled.display();

}

void SDD1306_Humi(String H)
{
    oled.clearDisplay();
    oled.setTextSize(1);
    oled.setCursor(40,16);
    oled.print("Humidity:");
    oled.setTextSize(2);
    oled.setCursor(35,34);
    oled.print(String(H,2));
    oled.setTextColor(WHITE);
    oled.setTextSize(2);
    oled.setCursor(60,50);
    oled.print("%");
    oled.display();
}
void SDD1306_Press(String P)
{
    oled.clearDisplay();
    oled.setTextSize(1);
    oled.setCursor(40,16);
    oled.print("Pressure:");
    oled.setTextSize(2);
    oled.setCursor(35,34);
    oled.print(String(P,4));
    oled.setTextColor(WHITE);
    oled.setTextSize(2);
    oled.setCursor(48,50);
    oled.print("hPa");
    oled.display();
}
void SDD1306_Rain(String R)
{
    oled.clearDisplay();
    oled.setTextSize(1);
    oled.setCursor(50,16);
    oled.print("Rain:");
    oled.setTextSize(2);
    oled.setCursor(35,34);
    oled.print(String(R,2));
    oled.setTextColor(WHITE);
    oled.setTextSize(1);
    oled.setCursor(42,54);
    oled.print("mm a day");
    oled.display();
}
