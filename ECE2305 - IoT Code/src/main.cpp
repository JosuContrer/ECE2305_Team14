#include <Arduino.h>
#include <UltraSonicSensor.h>
#include <Pins.h>

//-------------------------Reciever Code--------------------------
/*
433 MHz RF Module Receiver Demonstration 1
RF-Rcv-Demo-1.ino
Demonstrates 433 MHz RF Receiver Module
Use with Transmitter Demonstration 1

DroneBot Workshop 2018
https://dronebotworkshop.com
*/

// Include RadioHead Amplitude Shift Keying Library
#include <RH_ASK.h>
// Include dependant SPI Library
#include <SPI.h>

// Create Amplitude Shift Keying Object
RH_ASK rf_driver;
//String Variables
String recievedData;
String sensorNodeName;
String sensor1;
String sensor2;
String sensor3;
//Location Variables
int ind1;
int ind2;
int ind3;
int ind4;

void setup()
{
  // Initialize ASK Object
  rf_driver.init();
  // Setup Serial Monitor
  Serial.begin(9600);
}

void loop()
{
  // Set buffer to size of expected message
  // Antenna Test successful,
  uint8_t buf[9];
  uint8_t buflen = sizeof(buf);
  recievedData = (char*)buf;
  // Check if received packet is correct size
  if (rf_driver.recv(buf, &buflen))
  {
    ind1 = recievedData.indexOf(',');                   //finds location of first ,
    sensorNodeName = recievedData.substring(0, ind1);   //captures first data String
    ind2 = recievedData.indexOf(',', ind1+1 );          //finds location of second ,
    sensor1 = recievedData.substring(ind1+1, ind2);   //captures second data String
    ind3 = recievedData.indexOf(',', ind2+1 );
    sensor2 = recievedData.substring(ind2+1, ind3);
    ind4 = recievedData.indexOf(',', ind3+1 );
    sensor3 = recievedData.substring(ind3+1, ind4);            //captures remain part of data after last ,

    // Message received with valid checksum
    Serial.println("Message Received: ");
    //Serial.println((char*)buf);
    Serial.print("Sensor Name: ");
    Serial.println(sensorNodeName);
    Serial.print("Sensor1: ");
    Serial.println(sensor1);
    Serial.print("Sensor2: ");
    Serial.println(sensor2);
    Serial.print("Sensor3: ");
    Serial.println(sensor3);
    Serial.println("");

  }
}

//fucntion ideas-----------
//Fucntion that takes parmaters of all the nodes and stroes them to read them later
//store_data(String rSensorName, String rSensor1, String rSensor2, String rSensor3);
