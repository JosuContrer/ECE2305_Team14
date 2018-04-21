/*
  Agreggator code for Final Project
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
//All pins easy accesses
#include "globalPins.h"
//Other classes


// Create Amplitude Shift Keying Object
RH_ASK rf_driver;

// Fucntion Prototypes
bool processData(String dataR);
void printLoadingMessages(String message);
void testAgreggator();

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
const char *msg;

//Other varibales
int count = 0;
enum State {TRANSMITTINGNODENUM,  RECIEVINGDATA} state;

void setup()
{
  // Initialize ASK Object
  rf_driver.init();
  //Setup Reciver LED
  pinMode(LED_RECIEVING, OUTPUT);
  pinMode(LED_TRANSMITTING, OUTPUT);
  // Setup Serial Monitor
  Serial.begin(9600);
  //Set State
  state = TRANSMITTINGNODENUM;
}

void loop()
{
  switch(state){
     case TRANSMITTINGNODENUM:
      digitalWrite(LED_TRANSMITTING, HIGH);
      Serial.println("Transmitting Node Number:");
      msg = "1";
      Serial.println(msg);
      rf_driver.send((uint8_t *)msg, strlen(msg));
      rf_driver.waitPacketSent();
      digitalWrite(LED_TRANSMITTING, LOW);
      delay(1000);
      if(count == 3){
        count = 0;
        state = RECIEVINGDATA;
      }
      count ++;
      break;

    case RECIEVINGDATA:
      //Extra code not needed
      // if(count == 29000){
      //   count = 0;
      //   Serial.println("Waiting to Recieve Data...");
      //   Serial.println("");
      // }
      // count++;

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
        digitalWrite(LED_RECIEVING,LOW);
        Serial.println(count);
        count = 0;
        //state = TRANSMITTINGNODENUM;
      }
      count ++;
      digitalWrite(LED_RECIEVING,LOW);
      //NOTE: create a counter non blocking code to know when to chnage state
      //Takes arounf 9600-9700 to get a succesfull message  (sometimes 10000 - 20000)
      break;
    }

}

//*******************************************************************
//*******************************************************************
//*******************************************************************
//*****************************Functions*****************************

//Fucntion that processes the data recieved form a single sensor module
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


//Function just in case something breaks and we need to check if it is hardware
void testAgreggator(){
  if(count == 18000){
    count = 0;
    Serial.println("Testing connection...");
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
//fucntion ideas-----------
//Fucntion that takes parmaters of all the nodes and stroes them to read them later
//store_data(String rSensorName, String rSensor1, String rSensor2, String rSensor3);
