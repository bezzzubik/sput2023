#include <VirtualWire.h>

byte message[VW_MAX_MESSAGE_LEN]; // Буфер для хранения принимаемых данных
byte messageLength = VW_MAX_MESSAGE_LEN; // Размер сообщения


const int receiver_pin = 5; // Пин подключения приемника

void setupPr()
{

  vw_set_rx_pin(receiver_pin); // Пин подключения приемника
  vw_setup(2000); // Скорость передачи данных (бит в секунду)
  vw_rx_start(); // Активация применика
}

void loopPr()
{
   if(vw_get_message(message, &messageLength));
   { for (int i = 0; i < messageLength; i++)
    {
      Serial.write(message[i]); // выводим их в одной строке
    }
    printPiko();
    Serial.println();
   }
   EndB(10);
}


void printPiko()
{
  for(int i=0; i<messageLength; i++)
  {
    Serial1.print((char)message[i]);
    delay(20);
  }
}
