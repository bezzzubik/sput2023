#include <OneWire.h>

#define KOF 10 //коэффициент пропорциональности регулятора
#define TEMPNORM 15 //необходимая температура
#define acs712_pin A11


int zero = 506; // уровень нуля, относительно которого измеряется ток, обычно VCC/2
float VolAk;
int Nap=255;

OneWire ds(10);

void setupBort()
{

  zero = getSmoothedValue(); // определим нуль шкалы (до включения нагрузки)
  analogReference( EXTERNAL );
//  pinMode(8, OUTPUT);
//  pinMode(9, OUTPUT);
  pinMode(37, OUTPUT);
  pinMode(11, OUTPUT);
  digitalWrite(11, LOW);
  
}

void bort()
{
  heater( checkBlock(1)?Temp():Nap);
   
  if(checkBlock(2))
    Amper();
  if (checkBlock(3))
    VolAk=Voltage();
}



byte addrAk[8]={40, 215, 84, 0, 161, 34, 7, 91};


int Temp()
{

  byte i;
  byte j=1;
  byte present = 0;
  byte type_s;
  byte data[12];
  byte addr[8];
  int celsius;
  int celA=-70;
  bool akk_li;

  while (ds.search(addr)) {
  akk_li=true;
  if (OneWire::crc8(addr, 7) != addr[7]) {
      return celA;
  }
  for(int i=0; i<8; i++){
    if(addr[i]!=addrAk[i])
      akk_li=false;
  } 
  // the first ROM byte indicates which chip
  switch (addr[0]) {
    case 0x10:
      type_s = 1;
      break;
    case 0x28:
      type_s = 0;
      break;
    case 0x22:
      type_s = 0;
      break;
    default:
      return celA;
  } 

  ds.reset();
  ds.select(addr);
  ds.write(0x44, 1);        // start conversion, with parasite power on at the end
     // maybe 750ms is enough, maybe not
  // we might do a ds.depower() here, but the reset will take care of it.
  
  present = ds.reset();
  ds.select(addr);    
  ds.write(0xBE);         // Read Scratchpad

  for ( i = 0; i < 9; i++)           // we need 9 bytes
    data[i] = ds.read();
  OneWire::crc8(data, 8);

  // Convert the data to actual temperature
  // because the result is a 16 bit signed integer, it should
  // be stored to an "int16_t" type, which is always 16 bits
  // even when compiled on a 32 bit processor.
  int16_t raw = (data[1] << 8) | data[0];
  if (type_s) {
    raw = raw << 3; // 9 bit resolution default
    if (data[7] == 0x10) {
      // "count remain" gives full 12 bit resolution
      raw = (raw & 0xFFF0) + 12 - data[6];
    }
  } else {
    byte cfg = (data[4] & 0x60);
    // at lower res, the low bits are undefined, so let's zero them
    if (cfg == 0x00) raw = raw & ~7;  // 9 bit resolution, 93.75 ms
    else if (cfg == 0x20) raw = raw & ~3; // 10 bit res, 187.5 ms
    else if (cfg == 0x40) raw = raw & ~1; // 11 bit res, 375 ms
    //// default is 12 bit resolution, 750 ms conversion time
  }

    celsius = (int)raw / 16;

    if (akk_li)
    {
      celA=celsius;      
      j=2;
    }
    printTL(celsius, j);
    PrintIn(celsius, 4);
    
    j++;
    numbl++;
  }
  
  ds.reset_search();
  EndB(1);
  return celA;

}



int dError=0; //переменная обработки ошибки



void heater(int temp)
{
  
  numbl=7;
  if (temp < -60) //если функция выявления ошибок возвращает истину
    dError++; //к переменной добавляется 1, иначе она обнуляется
  else
  dError=0;
  
  if (dError > 5) //проверка ошибок:если переменная ошибок выявила
  error(); //более 5 ошибочных значений, то начинает работать
  else //функция обработки ошибки, иначе идет нормальный
  normal(temp); //режим работы программы
}





void voltage(int out) //функция передачи сигнала на затвор транзистора
{
  if (out == 0){
    digitalWrite(37, LOW);
    digitalWrite(11, LOW);
  }else if(out > 1000){
    digitalWrite(11, HIGH);
    digitalWrite(37, HIGH);
  }else{
    digitalWrite(11, out);
    digitalWrite(37, out);
  }
  
  printHeL(out);
    
  PrintHeat(out);
  return ;
}





void normal(int temp)
{

  int output=KOF*(TEMPNORM - temp);
  
  if( temp < TEMPNORM )
  
    voltage(output);
    
  else
  
    voltage(0);
  
  return ;
}






void error(){ //функция обработки ошибок

    Serial.print("Attention, possibly disabled signal");
    
    Serial.println("The heater runs at 30%\n"); //сообщение о том, что передается 30% от максимального значения сигнала
    
    voltage(225); //передача 30% от максимального сигнала
    
    return ;
}



// получает сглаженное значение с АЦП Arduino

int getSmoothedValue(){
  
  int value;
  int repeats = 10;
  
  for (int i=0; i<repeats; i++)
  { // измеряем значение несколько раз
    value += analogRead(acs712_pin); // суммируем измеренные значения

  }
  
  value /= repeats; // и берём среднее арифметическое
  return value;
}




void Amper() {

  int sensorValue = analogRead(acs712_pin); // читаем значение с АЦП и выводим в монитор
  int c = getCurrent(sensorValue); // преобразуем в значение тока и выводим в монитор
  numbl=8;
  PrintIn(c, 4);
  printIntL("I", c);
  EndB(1);
}

// рассчитывает ток в мА по значению с АЦП
int getCurrent(int adc) {
  int delta = -zero + adc; // отклонение от нуля шкалы
  float scale = 37.888888; // сколько единиц АЦП приходится на 1 ампер, по формуле (1)
  int current = (int)delta*1000*2/scale; // считаем ток в мА и округляем до целых, по формуле (2)
  return current;
}



float Voltage()
{
  numbl=9;
 int analogInput, value;
  float vout, vin, R1, R2;

  for(analogInput=12; analogInput !=15; analogInput++ )
  {
    switch (analogInput)
    {
    
     case 12: R1 = 6800.0;
              R2 = 4700.0;
            break;
     case 13: R1 = 4700.0;
              R2 = 6800.0;
            break;
     case 14: R1 = 6800.0;
              R2 = 1100.0;
    }
    
    value = analogRead(analogInput);
    vout = (value * 5.0) / 1024.0;
    vin = vout / (2*R2/(R1+R2));

    if (vin<0.09) {
      vin=0.0;// обнуляем нежелательное значение
    }
    
    PrintFl(vin, 5, 2);
    printFL2L("V", vin);
    numbl++;
  }
  EndB(3);
  return vin;
}
