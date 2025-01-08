/*  ____       __________              _ __  
   / __ \_____/_  __/ __ \____  ____  (_) /__
  / / / / ___/ / / / /_/ / __ \/ __ \/ / //_/
 / /_/ / /  _ / / / _, _/ /_/ / / / / / ,<   
/_____/_/  (_)_/ /_/ |_|\____/_/ /_/_/_/|_|  

   Neo6M Modul Test / Demo Sketch
   Dr.TRonik / YouTube  12/2024 İzmir/Türkiye
   This demo sketch demonstrates the normal use of a TinyGPSPlus (TinyGPSPlus) object.
   It requires the use of SoftwareSerial, and assumes that you have a
   9600-baud serial GPS device hooked up on pins 4(Rx) and 5(Tx).
   https://youtu.be/4wGDE9PS97A
*/

/********************************************************************
  GLOBALS___GLOBALS___GLOBALS___GLOBALS___GLOBALS___GLOBALS___
 ********************************************************************/
//Kütüphane
#include <TinyGPSPlus.h>
#include <SoftwareSerial.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>  // https://github.com/ronaer/I2C_Lib_LiquidCrystal
#include <ArduinoOTA.h>


// Nesne oluşturma:
TinyGPSPlus gps;
LiquidCrystal_I2C lcd(0x3F, 20, 4);  //Kullandığımız LCD I²C adresi ve karakter satır sayısı

//Tanımlar
static const int RXPin = 4, TXPin = 5;  //esp8266 d1 WeMos RxD2 gpio4 & TxD1 gpio5
SoftwareSerial ss(RXPin, TXPin);        // The serial connection to the GPS device
static const uint32_t GPSBaud = 9600;

#define STA_SSID "YourSSID"
#define STA_PASSWORD "YourPass"


bool connected = false;
unsigned long last_second;

/********************************************************************
  SETUP___SETUP___SETUP___SETUP___SETUP___SETUP___SETUP___SETUP___
 ********************************************************************/
void setup() {
  Serial.begin(9600);
  ss.begin(GPSBaud);
  Wire.begin(2, 0);  // Wire.begin(SDA, SCL);
  lcd.begin();       // LCD başlasın
  lcd.clear();       // ve ekran temizlensin
  lcd.setCursor(4, 0);
  lcd.print("Neo 6 M MODUL");
  lcd.setCursor(0, 1);
  lcd.print("TEST TEST  TEST TEST");
  lcd.setCursor(1, 2);
  lcd.print("  Dr.TRonik");
  lcd.setCursor(10, 3);
  lcd.print("YouTube");
  delay(2000);
  lcd.clear();

  WiFi.mode(WIFI_STA);
  Serial.print("Trying WiFi connection to ");
  Serial.println(STA_SSID);

  WiFi.setAutoReconnect(true);
  WiFi.begin(STA_SSID, STA_PASSWORD);

  ArduinoOTA.begin();
}

/********************************************************************
  LOOP__LOOP__LOOP__LOOP__LOOP__LOOP__LOOP__LOOP__LOOP__LOOP__LOOP__
 ********************************************************************/
void loop() {
  static unsigned long timer = millis();
  //Handle OTA
  ArduinoOTA.handle();
  if (WiFi.status() == WL_CONNECTED) {
    if (!connected) {
      connected = true;
      Serial.println("");
      Serial.print("Connected to ");
      Serial.println(STA_SSID);
      Serial.println("WiFi connected");
      Serial.print("IP address: ");
      Serial.println(WiFi.localIP());
    }
  } else {
    if (connected) {
      connected = false;
      Serial.print("Disonnected from ");
      Serial.println(STA_SSID);
    }
  }

  // This sketch displays information every time a new sentence is correctly encoded.
  while (ss.available() > 0)
    if (gps.encode(ss.read()))
      //displayInfo();

      if (millis() > 5000 && gps.charsProcessed() < 10) {
        Serial.println(F("No GPS detected: check wiring."));
        while (true)
          ;
      }
  if (millis() - last_second > 2000) {
    last_second = millis();

    char sat[3];
    sprintf(sat, "%2d", gps.satellites.value());
    lcd.setCursor(0, 0);
    lcd.print(sat) + lcd.print("U");

    if ((gps.date.isValid())) {
      char calendar[12];
      sprintf(calendar, "%02d/%02d/%04d", (gps.date.day()), (gps.date.month()), (gps.date.year()));
      lcd.setCursor(4, 0);
      lcd.print(calendar);
    }

    if ((gps.time.isValid())) {
      char time[6];
      sprintf(time, "%02d:%02d", ((gps.time.hour()) + 3), (gps.time.minute()));
      lcd.setCursor(15, 0);
      lcd.print(time);
    }

    if (gps.location.isValid()) {
      lcd.setCursor(0, 1);
      lcd.print("Enlem :") + lcd.print(gps.location.lat(), 6);
      lcd.setCursor(0, 2);
      lcd.print("Boylam:") + lcd.print(gps.location.lng(), 6);
    } else {
      lcd.setCursor(0, 1);
      lcd.print("Enlem :") + lcd.print("?");
      lcd.setCursor(0, 2);
      lcd.print("Boylam:") + lcd.print("?");
    }

    if ((gps.speed.isValid())) {
      if ((gps.speed.isUpdated())) {
        char speed[4];
        sprintf(speed, "%3d", int(gps.speed.kmph()));
        lcd.setCursor(0, 3);
        lcd.print("HIZ:") + lcd.print(speed) + lcd.print("km/h");
      }
    } else {
      lcd.setCursor(0, 3);
      lcd.print("HIZ:?") + lcd.print("  km/h");
    }

    if ((gps.altitude.isValid())) {
      if ((gps.altitude.isUpdated())) {
        char alti[4];
        sprintf(alti, "%3d", int(gps.altitude.meters()));
        lcd.setCursor(12, 3);
        lcd.print("YUK:") + lcd.print(alti) + lcd.print("m");
      }
    } else {
      lcd.setCursor(12, 3);
      lcd.print(" YUK:?") + lcd.print(" m");
    }
  }
}

/********************************************************************
  VOIDs__VOIDs__VOIDs__VOIDs__VOIDs__VOIDs__VOIDs__VOIDs__VOIDs__VOIDs
********************************************************************/

/*___:
bilgi@ronaer.com
WhatsApp: https://whatsapp.com/channel/0029VaxtFPiLSmbgUryuGs0E
Instagram: https://www.instagram.com/dr.tronik2023/   
YouTube: Dr.TRonik: https://www.youtube.com/c/DrTRonik
PCBWay: https://www.pcbway.com/project/member/shareproject/?bmbno=A0E12018-0BBC-4C
*/