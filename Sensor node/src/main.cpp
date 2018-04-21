#include <Arduino.h>
//#include <UltraSonicSensor.h>
//#include <Pins.h>
//------------------------Trasmitter Code-----------------------
/*
  433 MHz RF Module Transmitter Demonstration 1
  RF-Xmit-Demo-1.ino
  Demonstrates 433 MHz RF Transmitter Module
  Use with Receiver Demonstration 1
  DroneBot Workshop 2018
  https://dronebotworkshop.com
*/

// Include RadioHead Amplitude Shift Keying Library
#include <RH_ASK.h>
// Include dependant SPI Library
#include <SPI.h>
//All pins for easy accesses
#include "globalPins.h"
//Others
#include "UltraSonicSensor.h"

// Create Amplitude Shift Keying Object
RH_ASK rf_driver;

//Variables
String str_out;

//Prototypes
String concatSensorReadings(String nanoName, String oneS, String twoS, String threeS);

//Object Variables
UltraSonicSensor sensor1(9,8,30); //MAX: 300 [cm] = 3 [m]

void setup()
{
    // Initialize ASK Object
    rf_driver.init();
    sensor1.initialize();
    pinMode(LED_TRANSMITTING, OUTPUT);
    Serial.begin(9600);
}

void loop()
{
    digitalWrite(LED_TRANSMITTING, HIGH);
    Serial.println("Transmitting...");
    str_out = concatSensorReadings("N1", "0","0", sensor1.distanceFromPerson());
    const char *msg = str_out.c_str();
    //Serial.println(msg);
    rf_driver.send((uint8_t *)msg, strlen(msg));
    rf_driver.waitPacketSent();
    digitalWrite(LED_TRANSMITTING, LOW);
    delay(1000);
}

//*******************************************************************
//*******************************************************************
//*******************************************************************
//****************************Fucnctions*****************************
String concatSensorReadings(String nanoName, String oneS, String twoS, String threeS){
  return nanoName + "," + oneS + "," + twoS + "," + threeS + ",";
}
