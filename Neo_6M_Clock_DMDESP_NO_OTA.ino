/*  ____       __________              _ __  
   / __ \_____/_  __/ __ \____  ____  (_) /__
  / / / / ___/ / / / /_/ / __ \/ __ \/ / //_/
 / /_/ / /  _ / / / _, _/ /_/ / / / / / ,<   
/_____/_/  (_)_/ /_/ |_|\____/_/ /_/_/_/|_|  

   Neo6M Modul Time & P10 Panel / Demo Sketch
   Dr.TRonik / YouTube  06/2025 İzmir/Türkiye
   This demo sketch demonstrates the use of a TinyGPSPlus (TinyGPSPlus) object as a gps clock.
   It requires the use of SoftwareSerial, and assumes that you have a
   9600-baud serial GPS device hooked up on pins GPIO2(Rx) and A0(Tx).
   https:
   P10 MonoColor Hardware Connections:
            ------IDC16 IN------
  CS/GPIO15/D8  |1|   |2|  D0/GPIO16
            Gnd |3|   |4|  D6/GPIO12/MISO
            Gnd |5|   |6|  X
            Gnd |7|   |8|  D5/GPIO14/CLK
            Gnd |9|   |10| D3/GPIO0
            Gnd |11|  |12| D7/GPIO13/MOSI
            Gnd |13|  |14| X
            Gnd |15|  |16| X
            --------------------
*/

/********************************************************************
  GLOBALS___GLOBALS___GLOBALS___GLOBALS___GLOBALS___GLOBALS___
 ********************************************************************/
//Kütüphane
#include <TinyGPSPlus.h>
TinyGPSPlus gps;

#include <SoftwareSerial.h>
static const int RXPin = 2, TXPin = A0;  //Rx D4; Tx A0
SoftwareSerial ss(RXPin, TXPin);

#include <DMDESP.h>               // https://github.com/busel7/DMDESP
#include <fonts/SystemFont5x7.h>  //Saniye
#include <fonts/EMSans8x16.h>     //Saat

//DMD
#define DISPLAYS_WIDE 2
#define DISPLAYS_HIGH 1
DMDESP Disp(DISPLAYS_WIDE, DISPLAYS_HIGH);

int p10_Brightness, a;

/********************************************************************
  SETUP___SETUP___SETUP___SETUP___SETUP___SETUP___SETUP___SETUP___
 ********************************************************************/
void setup() {
  ss.begin(9600);
  delay(100);

  // DMDESP
  Disp.start();
  Disp.setBrightness(1);
}

/********************************************************************
  LOOP__LOOP__LOOP__LOOP__LOOP__LOOP__LOOP__LOOP__LOOP__LOOP__LOOP__
 ********************************************************************/
void loop() {
  //GPS
  while (ss.available() > 0)
    if (gps.encode(ss.read()))
      if (millis() > 5000 && gps.charsProcessed() < 10) {
        Serial.println(F("No GPS detected: check wiring."));
        while (true)
          ;
      }

  static unsigned long timer = millis();

  // P10 Print
  if (millis() - timer > 1000) {
    timer = millis();

    if (gps.time.isValid()) {
      a = gps.time.hour() + 3;  // (UniversalTimeCode UTC+3)

      if (a == 24)  // UTC 21 , Local 00
        a = 0;
      if (a == 25)  // UTC 22 , Local 01
        a = 1;
      if (a == 26)  // UTC 23 , Local 02
        a = 2;

      set_bright();
      Disp.setBrightness(p10_Brightness);

      Disp.setFont(EMSans8x16);

      char hour[3];
      sprintf(hour, "%02d", a);
      Disp.drawText(5, 0, hour);

      char min[3];
      sprintf(min, "%02d", gps.time.minute());
      Disp.drawText(30, 0, min);

      char sec[3];
      sprintf(sec, "%02d", gps.time.second());
      Disp.setFont(SystemFont5x7);
      Disp.drawText(49, 0, sec);

      // if ((gps.date.isValid())) {
      //   char calendar[12];
      //   sprintf(calendar, "%02d/%02d/%04d", (gps.date.day()), (gps.date.month()), (gps.date.year()));
      // }
    }
  }
  //Saat : Dakika Ayraç
  Disp.setFont(EMSans8x16);
  Disp.drawChar(24, 0, ':');

  Disp.loop();
}

/********************************************************************
  VOIDs__VOIDs__VOIDs__VOIDs__VOIDs__VOIDs__VOIDs__VOIDs__VOIDs__VOIDs
********************************************************************/
int set_bright() {
  //Saate göre parlaklık  ayarlama
  if (a >= 8 && a < 12) {
    p10_Brightness = 20;
  } else if (a >= 12 && a < 19) {
    p10_Brightness = 30;
  } else if (a >= 19 && a < 22) {
    p10_Brightness = 10;
  } else if (a >= 22 && a < 8) {
    p10_Brightness = 1;
  }
  return p10_Brightness;
}

/*
📌
📩 bilgi@ronaer.com
🟩 https://whatsapp.com/channel/0029VaxtFPiLSmbgUryuGs0E
📷 https://www.instagram.com/dr.tronik2023/   
📺 www.youtube.com/c/DrTRonik 
PCBWay: https://www.pcbway.com/project/member/shareproject/?bmbno=A0E12018-0BBC-4C
*/
