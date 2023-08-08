#include <TinyGPS++.h>
#include <Adafruit_MPU6050.h>
#include <Adafruit_SSD1306.h>
#include <Adafruit_Sensor.h>
#include <Wire.h>
#include <MS5611.h>

#define KX 9.5
#define KY 9.81
#define KZ 9.0
#define adr 0x1E
#define ss Serial3 
#define fl Serial2


TinyGPSPlus gps;

Adafruit_MPU6050 mpu;
Adafruit_SSD1306 display = Adafruit_SSD1306(128, 32, &Wire);

MS5611 ms5611;

double referencePressure;



void setupGeo() {

  ss.begin(9600);
  fl.begin(9600);

  
  if (!mpu.begin())
    while (1)
      yield();
  
  
  if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    Serial.println(F("SSD1306 allocation failed"));
      for (;;)
        ;
  }
  
  display.display();
  
//  delay(10);
  
  display.setTextSize(1);
  
  display.setTextColor(WHITE);
  
  display.setRotation(0);


  // Initialize MS5611 sensor
//  Serial.println("Initialize MS5611 Sensor");
  
  while(!ms5611.begin())
  {
  Serial.println("Could not find a valid MS5611 sensor, check wiring!");
  delay(10);
  }
 
  // Get reference pressure for relative altitude
//  referencePressure = ms5611.getPressure();

  StartComp();

  // Check settings
}


void StartComp()
{

    Wire.begin(); // инициализация I2C
  
    // Задаём режим работы датчика HMC5883:
    Wire.beginTransmission(adr);
    Wire.write(0x00); // выбираем регистр управления CRA (00)
    Wire.write(0x70); // записываем в него 0x70 [усреднение по 8 точкам, 15 Гц, нормальные измерения]
    Wire.write(0xA0); // записываем в регистр CRB (01) 0xA0 [чувствительность = 5]
    Wire.write(0x00); // записываем в регистр Mode (02) 0x00 [бесконечный режим измерения]
    Wire.endTransmission();
}




void geo() {

  if(checkBlock(5))
     Axel();
  if(checkBlock(6))
    Compas();
  if(checkBlock(7))
    Pres();
}





void Compas()
{
  numbl=23;  
  Wire.beginTransmission(adr);
  Wire.write(0x03);
  Wire.endTransmission();
  
  Wire.requestFrom(adr, 6);
  while( Wire.available() )  
  { 
    int h = Wire.read();
    int l = Wire.read();
    int x = word(h, l);

    int y = Wire.read();
    y = y << 8;
    y = y | Wire.read();
    
    int z = Wire.read() << 8;
    z |= Wire.read();

    int Abs=(int)sqrt( pow(x,2) + pow(y,2) + pow(z,2) );

    PrintIn(x, 4);
    printIntL("C",x);
    numbl++;
    PrintIn(y, 4);

    printIntL("C",y);
    numbl++;
    
    PrintIn(z, 4);


    printIntL("C",z);
    numbl++;
    
    PrintIn(Abs, 4);

    printIntL("C", Abs);
  }
  EndB(6);
}


void Pres()
{
  numbl=27;
  long realPressure = ms5611.readPressure();
  
  // Calculate altitude
  float absoluteAltitude = ms5611.getAltitude(realPressure);
  float relativeAltitude = ms5611.getAltitude(realPressure, referencePressure);
  hReal = absoluteAltitude;
  EndB(7);
  PrintIn(realPressure, 6);
  printIntL("P", realPressure);
  PrintFl(absoluteAltitude, 8, 2);
  PrintFl(relativeAltitude, 8, 2);
  printFL2L("AH", absoluteAltitude);
  numbl++;
  printFL2L("rH", relativeAltitude);


//
//  if (count % 20 == 0)
//  {
//    Serial.println();
//    Serial.println("DUR\tRES\tTEMP\tPRES");
//  }
//
//  Serial.print(stop - start);
//  Serial.print("\t");
//  Serial.print(result);
//  Serial.print("\t");
//  Serial.print(ms5611.getTemperature(), 2);
//  Serial.print("\t");
//  Serial.print(ms5611.getPressure(), 2);
//  Serial.println();
}



void Axel()
{

  numbl=15;
  sensors_event_t a, g, temp;
  mpu.getEvent(&a, &g, &temp);
  display.clearDisplay();
  display.setCursor(0, 0);
  float x, y, z;
  x=a.acceleration.x*9.81/KX;
  z=a.acceleration.z*9.81/KZ;
  y=a.acceleration.y*9.81/KY;  
  double all=sqrt( pow(x, 2) + pow(y, 2) + pow(z, 2) ); 

  PrintFl(x, 4, 1);  
  printAGL(x);
  numbl++;

  PrintFl(y, 4, 1);
  printAGL(y);
  numbl++;
    
  PrintFl(z, 4, 1);
  
  printAGL(z);
  numbl++;
    
  PrintFl(all, 4, 1);
  
  printAGL(all);

  numbl++;
    
  all=sqrt( pow(g.gyro.x, 2) + pow(g.gyro.y, 2) + pow(g.gyro.z, 2) );

  PrintFl(g.gyro.x, 6, 1);
  printAGL(g.gyro.x);
  numbl++;

  PrintFl(g.gyro.y, 6, 1);
  printAGL(g.gyro.y);
  numbl++;
  
  PrintFl(g.gyro.z, 6, 1);
  printAGL(g.gyro.z);
  numbl++;
  
  PrintFl(all, 6, 1);
  printAGL(all);
  numbl++;
  EndB(5);
}




void GPS()
{
  numbl=12;
  static const double LONDON_LAT = 51.508131, LONDON_LON = -0.128002;

  printInt(gps.satellites.value(), gps.satellites.isValid(), 5);
  printFloat(gps.hdop.hdop(), gps.hdop.isValid(), 6, 1);
  printFloat(gps.location.lat(), gps.location.isValid(), 11, 6);
  printFloat(gps.location.lng(), gps.location.isValid(), 12, 6);
 
    Serial1.print("lat=");
    delay(50);
    printFloat1(gps.location.lat(), gps.location.isValid(), 11, 6);
    delay(50);
    Serial1.print(','); 
  numbl++;
  
    delay(50);
    Serial1.print("lng=");
    delay(50);
    printFloat1(gps.location.lng(), gps.location.isValid(), 12, 6);
    delay(50);
    Serial1.print(','); 

  numbl++;
  
  printInt(gps.location.age(), gps.location.isValid(), 5);
  
  TinyGPSTime t=gps.time;
  TinyGPSDate d=gps.date;
//{
  if (!d.isValid())
  {
    Serial.print(F("********** "));
  }
  else
    {
    char sz[32];
    sprintf(sz, "%02d/%02d/%02d ", d.month(), d.day(), d.year());
  
    Serial.print(sz);
  }
  
  if (!t.isValid())
  {
    Serial.print(F("******** "));
  }
  else
  {
    char sz[32];
    sprintf(sz, "%02d:%02d:%02d ", t.hour(), t.minute(), t.second());
    Serial.print(sz);
  }
 
  printInt(d.age(), d.isValid(), 5);
  smartDelay(0);
  //}
  printFloat(gps.altitude.meters(), gps.altitude.isValid(), 7, 2);
  printFloat(gps.course.deg(), gps.course.isValid(), 7, 2);
  printFloat(gps.speed.kmph(), gps.speed.isValid(), 6, 2);
  printStr(gps.course.isValid() ? TinyGPSPlus::cardinal(gps.course.deg()) : "*** ", 6);
 
  unsigned long distanceKmToLondon =
    (unsigned long)TinyGPSPlus::distanceBetween(
      gps.location.lat(),
      gps.location.lng(),
      LONDON_LAT, 
      LONDON_LON) / 1000;
  printInt(distanceKmToLondon, gps.location.isValid(), 9);
 
  double courseToLondon =
    TinyGPSPlus::courseTo(
      gps.location.lat(),
      gps.location.lng(),
      LONDON_LAT, 
      LONDON_LON);
 
  printFloat(courseToLondon, gps.location.isValid(), 7, 2);
 
  const char *cardinalToLondon = TinyGPSPlus::cardinal(courseToLondon);
 
  printStr(gps.location.isValid() ? cardinalToLondon : "*** ", 6);
 
  printInt(gps.charsProcessed(), true, 6);
  printInt(gps.sentencesWithFix(), true, 10);
  printInt(gps.failedChecksum(), true, 9);
  
  smartDelay(1000);
  EndB(4);
  if (millis() > 5000 && gps.charsProcessed() < 10)
    Serial.println(F("No GPS data received: check wiring"));
}
 



static void smartDelay(unsigned long ms)
{
  unsigned long start = millis();
  do 
  {
    while (ss.available())
      gps.encode(ss.read());
  } while (millis() - start < ms);
}
 


static void printFloat(float val, bool valid, int len, int prec)
{
  if (!valid)
  {
    while (len-- > 1)
      Serial.print('*');
    Serial.print(' ');
  }
  else
  {
    Serial.print(val, prec);
    int vi = abs((int)val);
    int flen = prec + (val < 0.0 ? 2 : 1); // . and -
    flen += vi >= 1000 ? 4 : vi >= 100 ? 3 : vi >= 10 ? 2 : 1;
    for (int i=flen; i<len; ++i)
      Serial.print(' ');
  }
  smartDelay(0);
}
 

static void printFloat1(float val, bool valid, int len, int prec)
{
  if (!valid)
  {
    while (len-- > 1)
    Serial1.print('0');
  }
  else
  {
    Serial1.print(val, prec);

  }
  smartDelay(0);
}



static void printInt(unsigned long val, bool valid, int len)
{
  char sz[32] = "*****************";
  if (valid)
    sprintf(sz, "%ld", val);
  sz[len] = 0;
  for (int i=strlen(sz); i<len; ++i)
    sz[i] = ' ';
  if (len > 0) 
    sz[len-1] = ' ';
  Serial.print(sz);
  smartDelay(0);
}
 
 


static void printStr(const char *str, int len)
{
  int slen = strlen(str);
  for (int i=0; i<len; ++i)
    Serial.print(i<slen ? str[i] : ' ');
  smartDelay(0);
}
