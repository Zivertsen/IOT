#include "RainGuage.h"
#include "application.h"

int Q0 = A0; //Q
int Q1 = A1;
int Q2 = A2;
int Q3 = A3;
int Q4 = A4;
int Q5 = A5;
int Q6 = A7;
int Q_reset = D3;

int rain = 0;



void init_rainGuage()
{
  pinMode(Q0, INPUT);
  pinMode(Q1, INPUT);
  pinMode(Q2, INPUT);
  pinMode(Q3, INPUT);
  pinMode(Q4, INPUT);
  pinMode(Q5, INPUT);
  pinMode(Q6, INPUT);
  pinMode(Q_reset, OUTPUT);
}

int get_rainGuageCount()
{
  rain = 0;
  rain += digitalRead(Q0)*1;
  rain += digitalRead(Q1)*2;
  rain += digitalRead(Q2)*4;
  rain += digitalRead(Q3)*8;
  rain += digitalRead(Q4)*16;
  rain += digitalRead(Q5)*32;
  rain += digitalRead(Q6)*64;
  digitalWrite(Q_reset, HIGH);
  delay(5);              // waits for 5mS
  digitalWrite(Q_reset, LOW);
  return rain;
}
