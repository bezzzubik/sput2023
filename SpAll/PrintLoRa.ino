int pflag1;
int pflag2;




inline void setupLoRa() 
{
// put your setup code here, to run once:

pinMode(8, OUTPUT);
pinMode(9, OUTPUT);
Serial1.begin(9600);

}


String command;

void PrintL(){
  Serial1.println("EntCom");
  for(int j=0; j<20; j++, delay(100)){
    char c[64];
    if (Serial1.available()) {
      command = Serial1.readStringUntil('\n');
      strcpy(c, command.c_str());
      Serial1.println(c);
    
      switch (c[0])
      {
        
        case 't': on_the_block(c[1]);
                    break;
        case 'f': off_the_block(c[1]);
                    break;
        case 'p': print_zn(c);
                    break;
        case 'd': off_print(c);
                    break;
        case 'g':print_graf_on(c);
                  break; 
        case 'a':print_graf_off(c);
                  break;
        default :
                  Serial1.print("Com is not ...");
          
      }
    }
  }
}



void print_graf_off(char* c)
{
  if(strcmp(c,"aAll")==0)
  {if(graf)
  {
    graf=false;
    pflag1=0;
    pflag2=0;
  }}else
  {
    int n;
  if(!(n=chusezn(c)))
     Serial1.println("unknown combination"); 
  else{
     if(n<19){
       if(pflag1&(int)ldexp(1,(n-1)))
         pflag1=pflag1-(int)ldexp(1,(n-1));
     }else
      if(pflag2&(int)ldexp(1,(n-19)))
        pflag2=pflag2-(int)ldexp(1,(n-19));
  }
    
}
  
}


void print_graf_on(char* c)
{
  if(!graf){
    graf=true;
    pflag1=0;
    pflag2=0;
  }
  int n;
  if(!(n=chusezn(c)))
     Serial1.println("unknown combination"); 
  else{
     if(n<19)
        pflag1=pflag1|(int)ldexp(1,(n-1));
     else
        pflag2=pflag2|(int)ldexp(1,(n-19));
  }

}



void print_zn(char* c)
{

  int n;
  if(!(n=chusezn(c)))
     Serial1.println("unknown combination"); 
  else{
     if(n<19)
        pflag1=pflag1|(int)ldexp(1,(n-1));
     else
        pflag2=pflag2|(int)ldexp(1,(n-19));
  }
  return;
}


void off_print(char* c)
{

  int n;
  if(!(n=chusezn(c)))
     Serial1.println("unknown combination"); 
  else{
     if(n<19){
       if(pflag1&(int)ldexp(1,(n-1)))
         pflag1=pflag1-(int)ldexp(1,(n-1));
     }else
      if(pflag2&(int)ldexp(1,(n-19)))
        pflag2=pflag2-(int)ldexp(1,(n-19));
  }
  return;
  
}

int chusezn(char* c)
{

  switch(c[1])
  {
  case 'T':if (strlen(c) != 2)
              return chuseT(c[2]);
          break;
  case 'c': return 6;
          break;
  case 'n': return 7;
          break;
  case 'I': return 8;
          break;
  case 'V': if(strlen(c) != 2)
              return chuseV(c[2]);
          break;
  case 'l': if(strlen(c) != 2)
              return chuseL(c[2]);
          break;
  case 't': return 14;
          break;
  case 'A': return chuseAGC(c, 15);
          break;
  case 'G': return chuseAGC(c, 19);
          break;
  case 'C': return chuseAGC(c, 23);
          break;
  case 'P': return 27;
          break;
  case 'h': if(strlen(c) != 2)
             return chuseH(c[2]);
          break;
  case 'R': if(strlen(c) != 2)
              return chuseR(c[2]);
          break;
  case 'N':return 32;
          break;
  case 'g':return 33;
          break;
  case 'H':return 35;
          break;
  }
  return 0;
}



int chuseH(char c)
{

  if(c=='a')
      return 28;
  else if(c=='r')
      return 29;
  return 0;
  
}


int chuseR(char c)
{

  if(c=='d')
     return 30;
  else if(c=='s')
      return 31;
  return 0;
  
}




int chuseAGC(char* c, int p)
{

  if(strlen(c) == 3)
  {
     int k=chuseXYZ(c[2]);
        if (k)
            return p+k-1;
        else
             return k; 
  }else{
     return p+3;
  }

}



int chuseXYZ(char c)
{

  switch(c)
  {
  case 'x': return 1;
          break;
  case 'y': return 2;
          break;
  case 'z': return 3;
          break; 
   }
   return 0;
}


int chuseL(char c)
{

  switch(c)
  {
  case 'a': return 12;
          break;
  case 'o': return 13;
          break;
  }
  return 0;
}



int chuseV(char c)
{

  switch(c)
  {
  case '1': return 9;
        break;
  case '2': return 10;
        break;
  case '3': return 11;
        break; 
  }
  return 0;
}




int chuseT(char c)
{

  switch(c)
  {
  case '1': return 1;
          break;
  case '2': return 2;
          break;
  case '3': return 3;
          break;
  case '4': return 4;
          break;
  case '5': return 5;    
          break;
  }
  return 0;
}



void off_the_block(char d)
{
  
  int a=(int)d-48;
  eeprom_update_byte(a, 2);
  Serial1.print("Block ");
  Serial1.print(a);
  Serial1.println("is off");
  return;

}




void on_the_block(char d)
{
  
  int a=(int)d-48;
  eeprom_update_byte(a, 0);
  Serial1.print("Block ");
  Serial1.print(a);
  Serial1.println("is on");
  return;

}




bool printLoRa()
{
  Serial1.print(' ');
  if(numbl<19)
  {  if((int)ldexp(1,(numbl-1))&pflag1)
      return true;
    else
      return false;
  }else{
    if((int)ldexp(1,(numbl-19))&pflag2)
      return true;
    else
      return false;
  }
}


void printTL(int t, int j)
{

  Serial1.print('T');
  Serial1.print(j);
  Serial1.print('=');
  Serial1.print(t);
  Serial1.print(',');
  delay(200);

}

void printCountL(int j)
{
  Serial1.print("CoT=");
  Serial1.print(j);
  Serial1.print(',');
}


void printHeL(int o)
{
  delay(50);
  Serial1.print("h=");
  delay(50);
  if(o)
      Serial1.print('1');
  else
      Serial1.print('0');
  Serial1.print(',');
  delay(100);
  return;
}



void printFL2L(char* n, float v)
{
  Serial1.print(n);
  delay(100);
  if(n[0] == 'V')
    Serial1.print(numbl+3);
  Serial1.print('=');
  delay(100);
  Serial1.print(v,2);
  Serial1.print(',');
  delay(100);
}



void printAGL(float A)
{

  int i=numbl;
  if(i<19)
     Serial1.print('g');
  else{
     Serial1.print("gi");
     i=i-4;
  }
  XYZprL(i-15);
  
  Serial1.print('=');
  Serial1.print(A, 1);
  Serial1.print(',');
  delay(100);
}



void printIntL(char* n, long A)
{
  Serial1.print(n);  
  if( numbl>=23 && numbl<=26)
      XYZprL(numbl-23);
      
  Serial1.print('=');
  Serial1.print(A);
  Serial1.print(',');
  delay(100);
}

void XYZprL(int i)
{

  switch(i)
  {
    case 0: Serial1.print('x');
              break;
    case 1: Serial1.print('y');
              break;  
    case 2: Serial1.print('z');
              break;
  }
}



/*
   T1 - 1
   T2 - 2
   T3 - 3
   T4 - 4
   T5 - 5
   Количество температурных датчиков - 6
   Состояние нагревателя - 7
   I - 8a
   V12 - 9
   V13 - 10
   V14 - 11
   Lat - 12
   Long - 13
   Time - 14
   accelerometer
   X - 15
   Y - 16
   Z - 17
   ALL - 18
   
   gyro
   X - 19
   Y - 20
   Z - 21
   ALL - 22
   compas
   X - 23
   Y - 24
   Z - 25
   ALL - 26
   Давление - 27
   AbsoluteAltitude - 28
   relativeAltitude - 29
   Rad dyn - 30
   Rad stat - 31
   Num of puls - 32
   gaz - 33
   temp - 34
   Hum - 35
 */


 
