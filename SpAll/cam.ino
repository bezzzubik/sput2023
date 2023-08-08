#define CAM 44
unsigned long maxTime = (long) 1000*60*60*3;

unsigned long Time;

bool VT;

void setupCam()
{
 
  pinMode(CAM, OUTPUT);
  pinMode(33, OUTPUT);
  digitalWrite(CAM, HIGH);
  digitalWrite(33, HIGH);
  Time=millis();
  VT=true;
}



void timeP()
{
  int sec = (millis()/1000)%60;
  int mi = ((millis()/1000)%(24*60*60))/60%60;
  int ho = ((millis()/1000)%86400)/60/60;
  Serial.print(ho);
  Serial.print(':');
  Serial.print(mi);
  Serial.print(':');
  Serial.print(sec);
  Serial.print(' ');
}

void cam()
{

   if (VT)
     if( ( millis()- Time >= maxTime) || (VolAk < 10.0) )
        off_cam();
  
}

void off_cam()
{

   digitalWrite(CAM, LOW);
   digitalWrite(33, LOW);
   VT=false;

}
