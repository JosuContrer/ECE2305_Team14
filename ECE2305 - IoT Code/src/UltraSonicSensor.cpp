#include "UltraSonicSensor.h"
#include <Arduino.h>
//Hardware:
//Yellow:SDA (Arduino nano = A4)
//Green:SCL (Arduino nano = A5)

//Constructor
UltraSonicSensor::UltraSonicSensor(){}

//Method that gives return disatnce from the wall as an in
int UltraSonicSensor::distanceFromWall(int trigPin, int echoPin){
  // Clears the trigPin
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);

  // Sets the trigPin on HIGH state for 10 micro seconds
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);

  // Reads the echoPin, returns the sound wave travel time in microseconds
  duration = pulseIn(echoPin, HIGH);

  // Calculating the distance
  distance= duration*0.034/2;

  // Prints the distance on the Serial Monitor
  // Serial.print(distance);
  // Serial.print("|");
  // delay(400);

  return distance;
}
