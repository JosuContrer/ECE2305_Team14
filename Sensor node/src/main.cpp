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

/** Intructions for code:
 * Plug in the sensor node first and then the agreggator node. Hit the Reset button on the agregator node
 * Remember to set the 'nodeID' variable with the ID number of the sensor
 */
//-----------------------Set variables----------------------------
char *nodeID = "1"; //Set this value to match the Sensor ID number

//----------------------------------------------------------------

// Create Amplitude Shift Keying Object
RH_ASK rf_driver;

//Variables
String str_out;
int count;
static char *msg;
// uint8_t buf[1];
uint8_t buflen;
String recievedData;
enum State {LISTENING,TRANSMITTINGDATA} state;

//Prototypes
String concatSensorReadings(String nanoName, String oneS, String twoS, String threeS);
void testSensorNode();

//Object Variables
//UltraSonicSensor(int triggerPin, int echoPinnn, int setDistance);
UltraSonicSensor sensor1(s1Trig,s1Echo,30); //MAX: 300 [cm] = 3 [m]
UltraSonicSensor sensor2(s2Trig,s2Echo,30); //MAX: 300 [cm] = 3 [m]
UltraSonicSensor sensor3(s3Trig,s3Echo,30); //MAX: 300 [cm] = 3 [m]

void setup()
{
  Serial.println("Initializing sensor node...");
  // Initialize ASK Object
  rf_driver.init();
  sensor1.initialize();
  sensor2.initialize();
  sensor3.initialize();
  pinMode(LED_RECIEVING, OUTPUT);
  pinMode(LED_TRANSMITTING, OUTPUT);
  Serial.begin(9600);
  state = LISTENING;
  //state = TRANSMITTINGDATA;
}

void loop()
{
  switch(state){
    case LISTENING:
    digitalWrite(LED_RECIEVING, HIGH);
      if(count == 18000){
        count = 0;
        Serial.print("Sensor Node ");
        Serial.print((char*)nodeID);
        Serial.println(" Listening...;)");
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
        recievedData = (char*)buf;
        count = 0;
        //digitalWrite(LED_RECIEVING, HIGH);
        Serial.println("Authenticating Number...");
        if(recievedData.equals(nodeID)){ //set nodeID in 'set variables'
          Serial.println(recievedData);
          state = TRANSMITTINGDATA;
        }else{
          Serial.print("Not me... it is: ");
          Serial.println(recievedData);
        }
      }
      digitalWrite(LED_RECIEVING,LOW);
      break;

    case TRANSMITTINGDATA:
      digitalWrite(LED_TRANSMITTING, HIGH);
      Serial.println("Transmitting...");
      str_out = concatSensorReadings("N1", sensor1.distanceFromPerson(),sensor2.distanceFromPerson(), sensor3.distanceFromPerson());
      msg = str_out.c_str();
      Serial.println(msg);
      rf_driver.send((uint8_t *)msg, strlen(msg));
      rf_driver.waitPacketSent();
      digitalWrite(LED_TRANSMITTING, LOW);
      delay(1000);
      //TDMA- have to take into account the above delay
      if(count > 3){
        count = 0;
        state = LISTENING;
      }
      count++;
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
  str_out = concatSensorReadings("N1", sensor1.distanceFromPerson(),sensor2.distanceFromPerson(), sensor3.distanceFromPerson());
  msg = str_out.c_str();
  //Serial.println(msg);
  rf_driver.send((uint8_t *)msg, strlen(msg));
  rf_driver.waitPacketSent();
  digitalWrite(LED_TRANSMITTING, LOW);
  delay(1000);
}
