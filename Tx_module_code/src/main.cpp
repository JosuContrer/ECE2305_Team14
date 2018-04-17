#include <Arduino.h>
#include <UltraSonicSensor.h>
#include <Pins.h>
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

// Create Amplitude Shift Keying Object
RH_ASK rf_driver;

void setup()
{
    // Initialize ASK Object
    rf_driver.init();
}

void loop()
{
    const char *msg = "N1,0,1,0,";
    rf_driver.send((uint8_t *)msg, strlen(msg));
    rf_driver.waitPacketSent();
    delay(1000);
}

//----------------------------------------------------------------
//---------------------------------------------------------
//-------------Tesiting code Ultrasonic--------------------
/*
UltraSonicSensor sensor1;

void setup()
{
    // Setup Serial Monitor
    Serial.begin(9600);
}

void loop()
{
  Serial.print(sensor1.distanceFromWall(S1TRIGGERPIN,S1ECHOPIN))
}
*/
