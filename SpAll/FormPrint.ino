
void PrintIn(long a, int k)
{ /*
  int len, j;
  char c[32];
  
  long a1=a;
  for(j=0;a1>0;j++)
    a1=a1/10;
  len=j;
  
  j=0;
  if(a<0)
  {
    j++;
    c[0]='-';
  }
  a=abs(a);
  len--;
  c[k]='\0';
  k--;
  for(; len < k; k--)
    c[k]=' ';
  for(; len>=j; len--)
  {
     c[len]=(a%10)+48;
    a=a/10;
  }
  Serial.print(c);*/
  Serial.print(a);
  Serial.print(',');
}


void PrintFl(double f, int k, int l)
{ /*
  int len, j;
  char c[32];
  len=(int)f;
  for(j=0; len>0; j++)
    len=len/10;
  len=j;
  j=0;
  if(f<0)
    c[0]='-';
  c[k]='\0';
  k--;
  for(; len < k; k--)
    c[k]=' ';
  for(int n=l; n != 0; n--)
    f=f*10;
  j=(int)f;
  j=abs(j);
  for(; l!=0; l--, len--)
  {
    c[len]=j%10+48;
    j=j/10;
  }
  c[len]='.';
  len--;
  for(; len>=(f<0?1:0); len--)
  {
     c[len]=(j%10)+48;
    j=j/10;
  }
  Serial.print(c);*/
  Serial.print(f, l);
  Serial.print(',');
}


void PrintHeat(int i)
{

  if( i == 0)
    Serial.print("off,");
  else
    Serial.print("on,");
  
}



void PrintForm()
{
  Serial.begin(9600);
  delay(100);
  Serial.println("Setup...");
  delay(100);
  Serial.print(F("TimeOn T1 T2 T3 T4 T5 HEA I V1 V2 V3 "));
  delay(100);
  Serial.print(F("Sats  Latitude Longitude Fix Date Time Date Alt Course Speed Card Distance Course Card Chars Sentences Checksum"));
  delay(100);
  Serial.println(F(" Ax Ay Az Aa Gx Gy Gz Ga Fx Fy Fz Fa P abA reA RadD RadS NumP gaz T Hum"));
}
