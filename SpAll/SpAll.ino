#include <avr/wdt.h>
#include <avr/eeprom.h>

int numbl = 0;

bool graf = false;

float hReal = 0;

void setup() {

  //  pinMode(11, OUTPUT);
  wdt_disable();
  Serial.begin(9600);
//  checkStart();
  delay(100);
  //  PrintForm();
  setupBort();
  setupLoRa();
  setupGeo();
  setupPr();
  setupServo();
  setupEnv();
  wdt_enable (WDTO_8S);

  delay(100);
}

long timer=millis();


byte a = 0;
bool PsWork = false;
int h5km = 0;
bool servClose=true;



void loop() {

  /*   if(a){
        PrintL();
        a=0;
     }else
        a=1;
  */
  Zer();
  GPS();
  wdt_reset();
  bort();
  wdt_reset();
  env();
  wdt_reset();
//  geo();
  wdt_reset();
  Serial1.print(';');
  Serial1.println();
  delay(4000);
  wdt_reset();


  /*!!!!!!!!!!!!!!!!УДАЛИТЬ ПОСЛЕ ТЕСТА!!!!!!!!!!!!!!!!!!!!!!!!*/

  PsWork=true;
  servClose=false;
  /*!!!!!!!!!!!!!!!!УДАЛИТЬ ПОСЛЕ ТЕСТА!!!!!!!!!!!!!!!!!!!!!!!!*/
  if(PsWork){
      if(servClose)
      {
        servoGo();
        servClose=false;
      } 
      wdt_reset();
//       if(checkBlock(10))
         loopPr();
  }
  wdt_reset();
  Serial.println();
  for(int i=0; i<1; i++)
  {
    delay(3000);
    wdt_reset();
  }
}
