#include <Arduino.h>
#include <RadSensBoard.h>

RadSensBoard radSensBoard;

uint32_t delayMS;

// ClimateGuard_RadSens1v2 radSens(RS_DEFAULT_I2C_ADDRESS); /*Constructor of the class ClimateGuard_RadSens1v2,
                                                          // sets the address parameter of I2C sensor.
                                                          // Default address: 0x66.*/

void setupEnv()
{
  setupDos();
// if(checkBlock(9))
//   setupDHT_Gas();
}


void env()
{

  Dos();
 //if(checkBlock(9))
  // DHT_Gas();
 delay(50);

}




void setupDos() 
{
 
    radSensBoard.init();

    radSensBoard.getFirmwareVersion();

    radSensBoard.getBoardAddress();

    radSensBoard.getCalibrationValue();

    radSensBoard.getHighVoltageGeneratorState();

    radSensBoard.getLedIndicationState();

 
}



void Dos(){

 numbl=30;
 radSensBoard.readData();
 PrintFl(radSensBoard.getRadiationLevelDynamic()*1000, 6, 2);
 printFL2L("RD", radSensBoard.getRadiationLevelDynamic()*1000);
 


   printFL2L("RS", radSensBoard.getRadiationLevelStatic()*1000);
 PrintFl(radSensBoard.getRadiationLevelStatic()*1000, 6, 2);

 
 PrintIn(radSensBoard.getPulseCount(), 4);
 

 printFL2L("NumP", radSensBoard.getPulseCount());
 EndB(8);
}




void setupDHT_Gas() {
  //  delayMS = sensor.min_delay / 1000;
}


void DHT_Gas() {

 numbl=34;
 delay(delayMS);
 int gaz;
 float sensorValueGaz = analogRead(A7);
 gaz=(int)(100*(sensorValueGaz/1024*5.00));
 PrintIn(gaz, 3);
 if(printLoRa())
    printIntL("gaz", gaz);
 numbl++;
 EndB(9);
}
