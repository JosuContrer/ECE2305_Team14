/*
Sensor Node code for Final Project
Team 14
ECE2305
By Josue Contreras
Contibutors Fivos, Anastasia, John
*/

#include <Arduino.h>
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
int count;
const char *msg;
uint8_t buflen;
String recievedData;
enum State {LISTENING,TRANSMITTINGDATA} state;

//Prototypes
String concatSensorReadings(String nanoName, String oneS, String twoS, String threeS);
void testSensorNode();

//Object Variables
UltraSonicSensor sensor1(9,8,30); //MAX: 300 [cm] = 3 [m]

void setup()
{
  // Initialize ASK Object
  rf_driver.init();
  sensor1.initialize();
  pinMode(LED_RECIEVING, OUTPUT);
  pinMode(LED_TRANSMITTING, OUTPUT);
  Serial.begin(9600);
  state = TRANSMITTINGDATA;
}

void loop()
{
  switch(state){
    case LISTENING:
      if(count == 18000){
        count = 0;
        Serial.println("Sensor Node 1 Listening...;)");
        Serial.println("");
      }
      count++;
      // Set buffer to size of expected message
      // Antenna Test successful,
      uint8_t buf[1];
      buflen = sizeof(buf);
      recievedData = (char*)buf;
      // Check if received packet is correct size
      if (rf_driver.recv(buf, &buflen))
      {
        digitalWrite(LED_RECIEVING, HIGH);
        Serial.println("Authenticating Number...");
        if(recievedData.equals("1")){
          state = TRANSMITTINGDATA;
        }else{
          Serial.println("Not me...");
        }
      }
      digitalWrite(LED_RECIEVING,LOW);
      break;

    case TRANSMITTINGDATA:
      digitalWrite(LED_TRANSMITTING, HIGH);
      Serial.println("Transmitting...");
      str_out = concatSensorReadings("N1", "0","0", sensor1.distanceFromPerson());
      msg = str_out.c_str();
      //Serial.println(msg);
      rf_driver.send((uint8_t *)msg, strlen(msg));
      rf_driver.waitPacketSent();
      digitalWrite(LED_TRANSMITTING, LOW);
      delay(1000);
      break;
  }
}

//*******************************************************************
//*******************************************************************
//*******************************************************************
//*****************************Functions*****************************

//Packages up the data recieved by concatenating the data and adding overhead
String concatSensorReadings(String nanoName, String oneS, String twoS, String threeS){
  return nanoName + "," + oneS + "," + twoS + "," + threeS + ",";
}

//Function just in case something breaks and we need to check if it is hardware
void testSensorNode(){

  digitalWrite(LED_TRANSMITTING, HIGH);
  Serial.println(" Testing Transmission...");
  str_out = concatSensorReadings("N1", "0","0", sensor1.distanceFromPerson());
  msg = str_out.c_str();
  //Serial.println(msg);
  rf_driver.send((uint8_t *)msg, strlen(msg));
  rf_driver.waitPacketSent();
  digitalWrite(LED_TRANSMITTING, LOW);
  delay(1000);
}
