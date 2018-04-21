/*
 * This prevents this header to be included more then once in the complier proccess
 * If not defined in the comlier already then defines
 */
#ifndef UltraSonicSensor_h
#define UltraSonicSensor_h

#include "Arduino.h"

class UltraSonicSensor{
public:
  UltraSonicSensor(int triggerPin, int echoPinnn, int setDistance);
  int distanceFromWall();
  String distanceFromPerson();
  void initialize();

private:
  // defines variables
  long duration;
  int distance;
  int trigPin;
  int echoPin;
  int dist;

};

#endif
