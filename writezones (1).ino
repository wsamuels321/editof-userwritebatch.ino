//
// FILE: UserDataWriteBatch.ino
// AUTHOR: Rob Tillaart
// VERSION: 0.1.0
// PURPOSE: use of alarm field as user identification demo
// DATE: 2019-12-23
// URL:
//
// Released to the public domain
//

//edited by William Samuels 6/21 @wsamuels321@gmail.com
#include <iostream>
#include <OneWire.h>
#include <DallasTemperature.h>

const int seedPin =2;
const int totZones = 2;

//will make function with array initializations
OneWire oneWireZn[totZones]{OneWire(2), OneWire(3)}; 
DallasTemperature DTS[totZones]{DallasTemperature(&oneWireZn[0]), DallasTemperature(&oneWireZn[1])};
//

uint8_t field = 0x01;
 uint16_t wd;
uint8_t deviceCount = 0x00;



void setup(void)
{

  Serial.begin(9600);

  Serial.println("Write user ID to DS18B20\n");
   Serial.println(seedPin);
  //intialize sensors
   DTS[0].begin();
   DTS[1].begin();
   //--
   //uncomment these if you want to write to sensors
//labler();
//reader();


//-------basic use
DeviceAddress ad;
int tmpzn = 0;
DTS[tmpzn].getAddress(ad, 1);  //puts the address of sensor(1) of wire0 to variable ad
uint8_t *g;
 g = DTS[tmpzn ].getFieldandAddr(ad);  //gets int[2] of ad and stores it in g
 
 int16_t usrdat = DTS[tmpzn ].getUserDataByIndex(1);  //the exact value of sensor(1) of wire0's data field
 usrdat = usrdat % 255;   //this gets the zone number
 Serial.println(usrdat );


 Serial.print("DTS[1].g[0]:  ");
 Serial.println(g[0]);
 Serial.println(usrdat,HEX);
    uint8_t yy[2] = {1,2};  //zone1 sensor3
uint8_t zz[2] = {0,1}; //zone0 sensor2 
   float temp = getTempFrmZnAd(yy);  //get temp from the given int[2] arrays
 float tempo = getTempFrmZnAd(zz);   //  ''            ''              ''
  Serial.println(temp);  //print it
 Serial.println(tempo);

 
 

}

void loop(void) {

  /*
  uint8_t yy[2] = {0,1};  //zone1 sensor1
uint8_t zz[2] = {1,2};
   float temp = getTempFrmZnAd(yy);
 float tempo = getTempFrmZnAd(zz);
  Serial.println(temp);
 Serial.println(tempo);
 delay(50);*/

  }

void reader(){
  Serial.println();
  Serial.println("current ID's");

    
  //loop through zones
  for (int index = 0; index < totZones; index++)
  {
    Serial.print("DTS[index]: ");
    Serial.println(index);
    //loop through all devices per zone
    int zoneSensorCount = DTS[index].getDeviceCount();
    Serial.println("sensorCount of above:");
    Serial.println(zoneSensorCount);
    for (int  jdex = 0; jdex < zoneSensorCount; jdex++)
    {
      Serial.println("print addresses");
        DeviceAddress t;
    DTS[index].getAddress(t, jdex);
    printAddress(t);
    Serial.print("\t\tID: ");
    uint16_t id = DTS[jdex].getUserData(t);
     printID(id);
   
    }
 
  }
  }

  //---this function uses DallasTemperature.setFieldandAddr(Device Address, int[2]) to write to sensor
void labler(){
   DeviceAddress t;  //temp address variable
  Serial.println("Start labeling ...");
  uint8_t tmpdat[2];
  //loop through zonesusing DTS array
  for(uint8_t zns=0;zns<totZones; zns++){
   uint8_t sensorsInZn = DTS[zns].getDeviceCount();  //sensors in DTS[] element

    Serial.print("sensorsInZn: ");
    Serial.println(sensorsInZn,DEC);
   tmpdat[0] = zns;
     //loop through sensors in each zone
  for (uint8_t index=0 ; index < sensorsInZn; index++)
  {
    tmpdat[1] = index;
    Serial.print("sensor#= ");
    Serial.println(index,DEC);
    Serial.print("zone: ");
    Serial.println(zns,DEC);
    if(!DTS[zns].getAddress(t, index)){
      Serial.print("Not Valid Address!!");  //get address at index
    }
  //  uint8_t *tm;
    DTS[zns].setFieldandAddr(t, tmpdat);
    //tm = DTS[zns].getFieldandAddr(t);
    //Serial.print("tmpdat[0]:  ");
    //Serial.println(tm[0]);
  }
  
  }
}


//print the burned id array[2] where [0]=field/zone and [1]=sensor number in field/zone
void printID(uint8_t _usrdat[]){
   Serial.println(_usrdat[0]);
   Serial.println( _usrdat[1]);
}


//get the temperature of a zensor using the int[2] array
float getTempFrmZnAd(uint8_t _zn[]){
DeviceAddress adr;
uint8_t zone = _zn[0];
 addrByZN(_zn, adr);
 Serial.print("zone:  ");
 Serial.println(zone);
 Serial.print("address of temp sensor to be read: ");
 printAddress(adr);
 Serial.println();
 return DTS[zone].getTempF(adr);

  
}
//get DeviceAddress by userData!  uses the int[2] to compare and get address of sensor
void addrByZN(uint8_t _zn[], uint8_t* _addr){
  //get array then put them together for compare
  uint8_t zone= _zn[0];
  uint8_t addr =_zn[1];
  //--below makes a uint16_t for comparing with the EEPROM of the DallasTemp Sensor
  uint16_t dataSand;
  dataSand = _zn[0]<<8;
  dataSand+=_zn[1];  
  //--

  int totSens = DTS[zone].getDeviceCount();
  uint16_t compare;
  int count = 0;
    for (int index = 0; index < totSens; index++){
     compare = DTS[zone].getUserDataByIndex(index); 


     if(compare == dataSand){
   
      DTS[zone].getAddress(_addr , count);  //get the DeviceAddress and store it in pointer _addr
    
     Serial.println("got a positive compare");
      printAddress(_addr);
 
      return;  //then return
     }
    }
}


void printAddress(DeviceAddress deviceAddress)
{
  for (uint8_t i = 0; i < 8; i++)
  {
    // zero pad the address if necessary
    if (deviceAddress[i] < 16) Serial.print("0");

    Serial.print(deviceAddress[i], HEX);
  }
}

// END OF FILE
