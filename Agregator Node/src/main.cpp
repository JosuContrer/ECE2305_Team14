#include <Arduino.h>
//#include <UltraSonicSensor.h>
//#include <Pins.h>

/*
Agreggator code for Final Project
ECE2305
By Josue Contreras
*/

// Include RadioHead Amplitude Shift Keying Library
#include <RH_ASK.h>
// Include dependant SPI Library
#include <SPI.h>
//All pins easy accesses
#include "globalPins.h"

// Create Amplitude Shift Keying Object
RH_ASK rf_driver;

// Fucntion Prototypes
bool processData(String dataR);

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

//Other varibales
int count;

void setup()
{
  // Initialize ASK Object
  rf_driver.init();
  //Setup Reciver LED
  pinMode(LED_RECIEVING, OUTPUT);
  // Setup Serial Monitor
  Serial.begin(9600);
}

void loop()
{
  if(count == 100000){
    count = 0;
    Serial.println("Waiting to Recieve Data...");
    Serial.println("");
  }
  count++;
  // Set buffer to size of expected message
  // Antenna Test successful,
  uint8_t buf[9];
  uint8_t buflen = sizeof(buf);
  recievedData = (char*)buf;
  // Check if received packet is correct size
  if (rf_driver.recv(buf, &buflen))
  {
    digitalWrite(LED_RECIEVING, HIGH);
    Serial.println("Recieveing Data...");
    processData(recievedData);

  }
  digitalWrite(LED_RECIEVING,LOW);
}

//*******************************************************************
//*******************************************************************
//*******************************************************************
//****************************Fucnctions*****************************
bool processData(String dataR){
  Serial.println("Processing Data Recieved...");
  ind1 = dataR.indexOf(',');                   //finds location of first ,
  sensorNodeName = dataR.substring(0, ind1);   //captures first data String
  ind2 = dataR.indexOf(',', ind1+1 );          //finds location of second ,
  sensor1 = dataR.substring(ind1+1, ind2);   //captures second data String
  ind3 = dataR.indexOf(',', ind2+1 );
  sensor2 = dataR.substring(ind2+1, ind3);
  ind4 = dataR.indexOf(',', ind3+1 );
  sensor3 = dataR.substring(ind3+1, ind4);            //captures remain part of data after last ,

  Serial.println("");
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
  Serial.println("Done processing...");
  Serial.println("");
  Serial.println("");
  return true;
}
//fucntion ideas-----------
//Fucntion that takes parmaters of all the nodes and stroes them to read them later
//store_data(String rSensorName, String rSensor1, String rSensor2, String rSensor3);
