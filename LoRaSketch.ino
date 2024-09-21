#include <SparkFun_u-blox_GNSS_v3.h>

#include <SD.h>
#include <sd_defines.h>
#include <sd_diskio.h>

#include <Wire.h>

#include <LoRa.h>

#include <MicroNMEA.h> //https://github.com/stevemarple/MicroNMEA

#include <BLEDevice.h>
#include <BLEUtils.h>
#include <BLEScan.h>
#include <BLEAdvertisedDevice.h>

#include <BLEDevice.h>
#include <BLEUtils.h>
#include <BLEScan.h>
#include <BLEAdvertisedDevice.h>


/*
  Read NMEA sentences over sERIAL using Ublox module SAM-M8Q, NEO-M8P, etc
  Base on SparkFun_Ublox_Arduino_Library //https://github.com/sparkfun/SparkFun_Ublox_Arduino_Library
*/

#include "LoRaBoards.h" // LilyGo lib to determine which features the board can support


#define DISPLAY_ADDR            0x3C

DISPLAY_MODEL *u8g;                                     
SFE_UBLOX_GNSS myGNSS;

//char nmeaBuffer[100];
//MicroNMEA nmea(nmeaBuffer, sizeof(nmeaBuffer));

void setup()
{
    setupBoards();
    // When the power is turned on, a delay is required.
    delay(1000);

  while (myGNSS.begin() == false) //Connect to the u-blox module using Wire port
  {
      Wire.beginTransmission(DISPLAY_ADDR);
      if (Wire.endTransmission() == 0) {
        u8g2 = new DISPLAY_MODEL(U8G2_R0, U8X8_PIN_NONE);
        u8g2->begin();
        u8g2->clearBuffer();
        u8g2->setFont(u8g2_font_inb19_mr);
        u8g2->drawStr(0, 30, "u-blox GNSS not detected");
        u8g2->setFont(u8g2_font_inb19_mf);
        u8g2->drawStr(0, 60, "at default I2C address. Retrying...");
        u8g2->sendBuffer();
        delay(3000);
    }
    Serial.println(F("u-blox GNSS not detected at default I2C address. Retrying..."));
    delay (1000);
  }
  myGNSS.setI2COutput(COM_TYPE_UBX); //Set the I2C port to output UBX only (turn off NMEA noise)

}

void loop()
{
  Wire.beginTransmission(DISPLAY_ADDR);
  // Request (poll) the position, velocity and time (PVT) information.
  // The module only responds when a new position is available. Default is once per second.
  // getPVT() returns true when new data is received.
  if (myGNSS.getPVT() == true)
  {
    if (Wire.endTransmission() == 0) {
        Serial.printf("Find Display model at 0x%X address\n", DISPLAY_ADDR);
        u8g2 = new DISPLAY_MODEL(U8G2_R0, U8X8_PIN_NONE);
        u8g2->begin();
        u8g2->clearBuffer();
        u8g2->setFont(u8g2_font_inb16_mf);
        u8g2->drawStr(0, 30, "Test2");
        u8g2->setFont(u8g2_font_fur11_tf);
        u8g2->drawStr(58, 60, "Test3");
        u8g2->sendBuffer();
            
        int32_t latitude = myGNSS.getLatitude();
        Serial.print(F("Lat: "));
        Serial.print(latitude);

        int32_t longitude = myGNSS.getLongitude();
        Serial.print(F(" Long: "));
        Serial.print(longitude);
        Serial.print(F(" (degrees * 10^-7)"));

        int32_t altitude = myGNSS.getAltitudeMSL(); // Altitude above Mean Sea Level
        Serial.print(F(" Alt: "));
        Serial.print(altitude);
        Serial.print(F(" (mm)"));

        Serial.println();
        delay(3000);
      }
    } else {
      if (Wire.endTransmission() == 0) {
        Serial.printf("Find Display model at 0x%X address\n", DISPLAY_ADDR);
        u8g2 = new DISPLAY_MODEL(U8G2_R0, U8X8_PIN_NONE);
        u8g2->begin();
        u8g2->clearBuffer();
        u8g2->setFont(u8g2_font_fur11_tf);
        u8g2->drawStr(0, 30, "Test1");
        u8g2->setFont(u8g2_font_fur11_tf);
        u8g2->drawStr(58, 60, "Test4");
        u8g2->sendBuffer();
        delay(3000);
    }
  }
}

// boolean displayGNSSInfo()
// {
//   // Check if fresh position, velocity and time (PVT) information is available.
//   // getPVT returns true if there is a fresh navigation solution available.
//   if (myGNSS.getPVT() == true)
//   {
//      Wire.beginTransmission(DISPLAY_ADDR);
//     if (Wire.endTransmission() == 0) {
//         Serial.printf("Find Display model at 0x%X address\n", DISPLAY_ADDR);
//         u8g = new DISPLAY_MODEL(U8G2_R0, U8X8_PIN_NONE);
//         u8g->begin();
//         u8g->clearBuffer();
//         u8g->setFont(u8g2_font_freedoomr10_tu);
//         u8g->drawStr(0, 30, "TestingWHat");

//         u8g->setFont(u8g2_font_inb19_mf);
//         u8g->drawStr(58, 60, "Testingwhathappenswithmorecharacters");
//         u8g->sendBuffer();
//         u8g->setFont(u8g2_font_freedoomr10_tu);
//         delay(3000);
//         return true;
//     }
//   }
//   else
//   {
//     Serial.print(".");
//     delay(100);
//   }
//   return false;
// }
