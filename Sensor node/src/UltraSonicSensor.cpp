#include "UltraSonicSensor.h"
#include <Arduino.h>

//Constructor
UltraSonicSensor::UltraSonicSensor(int triggerPin, int echoPinnn,int setDistance){
  trigPin = triggerPin;
  echoPin = echoPinnn;
  dist = setDistance;
}

void UltraSonicSensor::initialize(){
  pinMode(trigPin, OUTPUT); // Sets the trigPin as an Output
  pinMode(echoPin, INPUT); // Sets the echoPin as an Input
  }

//Method that gives return disatnce from the wall as [in]
int UltraSonicSensor::distanceFromWall(){
  // Clears the trigPin
  digitalWrite(trigPin, LOW);
  delayMicroseconds(10);

  // Sets the trigPin on HIGH state for 10 micro seconds
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);

  // Reads the echoPin, returns the sound wave travel time in microseconds
  duration = pulseIn(echoPin, HIGH);

  // Calculating the distance [cm]
  distance= duration*0.034/2;

  // Prints the distance on the Serial Monitor
  // Serial.print(distance);
  // Serial.print("|");
  // delay(400);

  return distance;
}


//Method that return true if the distnce is right
String UltraSonicSensor::distanceFromPerson(){
  int sucReadings = 0;
  int i = 0;
  int isGood = 0;

  for(i = 0; i <= 10;i++){
  // Clears the trigPin
  digitalWrite(trigPin, LOW);
  delayMicroseconds(5);

  // Sets the trigPin on HIGH state for 10 micro seconds
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);

  // Reads the echoPin, returns the sound wave travel time in microseconds
  duration = pulseIn(echoPin, HIGH);

  // Calculating the distance [cm]
  distance= duration*0.034/2;

  //returns 1 if there is a person within the distance set
  if(distance <= dist){
    sucReadings++;
  }
}

//Calculate Reading Success
isGood = (sucReadings/i)*100;

//Return
if(isGood > 60){
  return "1"; //Someone sitting
}else{
  return "0"; //No one sitting
}
  // Prints the distance on the Serial Monitor
  // Serial.print(distance);
  // Serial.print("|");
  // delay(400);
}
