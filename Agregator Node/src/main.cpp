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
//Web layer
#include <BlynkSimpleStream.h>
//Auth Token for app connection
char auth[] = "b57790fd86c647639f00485878139be8";

// Attach virtual terminal terminal to Virtual Pin V1
WidgetTerminal terminal(V1);

/** Intructions for code:
 * Plug in the sensor node first and then the agreggator node. Hit the Reset button on the agregator node
 * The first itteration for the agregator node is just seeting up everyhting
 * Remember to set the 'maxSensorNodes' variable to the amount of sensor nodes in the network
 *
 */
//-----------------------Set variables----------------------------
int maxSensorNodes = 2; //IMPORTANT: Set according to how many sensor nodes there are
int transmissionIt = 2; //Set according to the number of transmission itterations (when agrregator calling a sensor node)
//----------------------------------------------------------------

// Create Amplitude Shift Keying Object
RH_ASK rf_driver;

// Function Prototypes
bool processData(String dataR);
void printLoadingMessages(String message);
void testAgreggator();
void saveData();
void printWeb();

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
static char *msg;

//Data Recieveing Variables
uint8_t buf[9];
uint8_t buflen;
bool setWarning;
int amountLoop = 0;

//TDMA variables
char nodeNumber = "0"; //only to intialize
int nodeInc = 0;
int nCount = 0;

//Web layer Variables
String dataBuffer[5];

//Other variables
long int count = 0;
enum State {TRANSMITTINGNODENUM,  RECIEVINGDATA, NEXTNODE, WEBLAYER} state;

void setup()
{
  //terminal.println("Initializing aggregator node...");
  // Initialize ASK Object
  rf_driver.init();
  //Setup Reciver LED
  pinMode(LED_RECIEVING, OUTPUT);
  pinMode(LED_TRANSMITTING, OUTPUT);
  // Setup terminal Monitor
  Serial.begin(9600);
  Blynk.begin(Serial, auth);
  terminal.println(F("Blynk v" BLYNK_VERSION ": Device started"));
terminal.println(F("-------------"));
terminal.flush();
  //Set State
  state = TRANSMITTINGNODENUM;
}

void loop()
{
  Blynk.run();

  switch(state){
     case TRANSMITTINGNODENUM:
      //Case to transmit node number that aggrgator wants to "hear"
      digitalWrite(LED_TRANSMITTING, HIGH);
      //terminal.println("Transmitting Node Number: ");
      msg = "1";
      //terminal.println(msg);
      //Send ID name (node number) message to listening nodes
      rf_driver.send((uint8_t *)msg, strlen(msg));
      rf_driver.waitPacketSent();
      digitalWrite(LED_TRANSMITTING, LOW);
      delay(1000);
      //Transmission itterations in order for sensor nodes to have time to recieve ID number and then start transmitting
      if(count == transmissionIt){
        count = 0;
        state = RECIEVINGDATA;
      }
      count ++;
      break;

    case RECIEVINGDATA:
      //NOTE: Here we implement a time slot for each sensor (TDMA)
      // Set buffer to size of expected message
      // Antenna Test successful,
      buflen = sizeof(buf);
      recievedData = (char*)buf;
      // Check if received packet is correct size
      if (rf_driver.recv(buf, &buflen) )
      {
        processData(recievedData);
        //make sure the message recieved is correct
        if(amountLoop >= 1){
          digitalWrite(LED_RECIEVING, HIGH);
          //Save data form the transmission
          saveData();
          terminal.println("Recieveing Data...");
          //processes the data recieved by the aggregator node and separates it
          //if(processData(recievedData)){
          digitalWrite(LED_RECIEVING,LOW);
          //terminal.println(count);
          amountLoop = 0;
          count = 0;
          state = NEXTNODE;
        //}
        }
        amountLoop++; //has to loop twice because first itteration dosen't get values
        //count = 0;
      }
      // In order to break if a sensor dosen't respond after N amount of time NOTE: have to determine value with N sensors
      if(count > 150000){
        setWarning = true;
        terminal.println("");
        terminal.print("WARNING: Node did not respond, node number:");
        terminal.println((char*)nodeNumber);
        terminal.println("");
        terminal.println("");
        count = 0;
        state = NEXTNODE;
      }
      count ++; //in order to be able to break from this case
      digitalWrite(LED_RECIEVING,LOW);
      break;

    case NEXTNODE:
      //Wait for the sensor node to reset itself
      if(nCount > 20000){
        nCount = 0;
        terminal.println("Waiting for next node...");
        //Loop on max amount of nodes (reset to zero when max is reached)
        if(nodeInc > maxSensorNodes){
          nodeInc = 0;
          state = WEBLAYER;
          break;
        }
        itoa(nodeInc++,nodeNumber,10); //converts int to char. Max is 9.
        terminal.println(nodeNumber);
        state = TRANSMITTINGNODENUM;
      }
      nCount++;
      break;

    case WEBLAYER:
      printWeb();
      state = TRANSMITTINGNODENUM;
      break;
    }

}

//*******************************************************************
//*******************************************************************
//*******************************************************************
//*****************************FUNCTIONS*****************************
//*******************************************************************
//*******************************************************************
//*******************************************************************

/**Fucntion that processes the data recieved form a single sensor module
* @param dataR String with all the data to be separated (has to be lenght of 9 only)
* @return boolen True if the message has been decoded completely
*/
bool processData(String dataR){
  terminal.println("Processing Data Recieved...");
  ind1 = dataR.indexOf(',');                   //finds location of first ,
  sensorNodeName = dataR.substring(0, ind1);   //captures first data String
  ind2 = dataR.indexOf(',', ind1+1 );          //finds location of second ,
  sensor1 = dataR.substring(ind1+1, ind2);   //captures second data String
  ind3 = dataR.indexOf(',', ind2+1 );
  sensor2 = dataR.substring(ind2+1, ind3);
  ind4 = dataR.indexOf(',', ind3+1 );
  sensor3 = dataR.substring(ind3+1, ind4);            //captures remain part of data after last ,

  terminal.println("");
  // Message received with valid checksum
  terminal.println("*Message Received: ");
  //terminal.println((char*)buf);
  terminal.print("Sensor Name: ");
  terminal.println(sensorNodeName);
  terminal.print("Sensor1: ");
  terminal.println(sensor1);
  terminal.print("Sensor2: ");
  terminal.println(sensor2);
  terminal.print("Sensor3: ");
  terminal.println(sensor3);
  terminal.println("");
  terminal.println("Done processing...");
  terminal.println("");
  terminal.println("");
  return true;
}

void saveData(){
  if(sensorNodeName.equals("N0")){
    // for(int i = 0; i < 4; i++){
    //   dataBuffer[i] =
    // }
    //dataBuffer[0] = sensorNodeName;
    dataBuffer[0] = sensor1;
    dataBuffer[1] = sensor2;
    dataBuffer[2] = sensor3;
  }
  if(sensorNodeName.equals("N1")){
    //dataBuffer[4] = sensorNodeName;
    dataBuffer[3] = sensor1;
    dataBuffer[4] = sensor2;
    dataBuffer[5] = sensor3;
  }
}

void printWeb(){
  // if(sensorNodeName == "N1"){
  //   lcd.print(1, 0, "1"); // use: (position X: 0-15, position Y: 0-1, "Message you want to print")
  // }
  String n = "0";
  for(int i; i < 6; i++){
    terminal.print("seat ");
    terminal.print(i);
    terminal.print(": ");
    if(n.equals(dataBuffer[i])){
      terminal.println("free");
    }else{
      terminal.println("occupied");
    }
    terminal.flush();
  }
}

/**Function just in case something breaks and we need to check if it is hardware
 * @method testAgreggator
 */
void testAgreggator(){
  if(count == 18000){
    count = 0;
    terminal.println("Testing connection...");
    terminal.println("");
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
    terminal.println("Recieveing Data...");
    processData(recievedData);

  }
  digitalWrite(LED_RECIEVING,LOW);
}

//fucntion ideas-----------
//Fucntion that takes parmaters of all the nodes and stroes them to read them later
//store_data(String rSensorName, String rSensor1, String rSensor2, String rSensor3);
