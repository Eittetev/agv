#include <VirtualWire.h>
  byte message[VW_MAX_MESSAGE_LEN]; // Буфер для хранения принимаемых данных
  byte messageLength = VW_MAX_MESSAGE_LEN; // Размер сообщения
  const int receiver_pin = 12; // Пин подключения приемника
void setup()
{
  Serial.begin(9600); // Скорость передачиданных
  Serial.println("MX-RM-5V is ready");
  vw_set_rx_pin(receiver_pin); // Пин подключения приемника
  vw_setup(2000); // Скорость передачи данных (бит в секунду)
  vw_rx_start(); // Активация применика
  pinMode(5,OUTPUT);
  pinMode(6,OUTPUT);
  pinMode(7,OUTPUT);
  pinMode(11, OUTPUT);
  digitalWrite(7,LOW);
}
void loop()
{
  int thr,ster,ster2,i; // Переменные управления и скорости
  ster2 = 512;
  if (vw_get_message(message, &messageLength)) // Если есть данные..
  {
  //Throttle
  thr = ((message[0]) << 8) + ((message[1])); // Перевод в десятичное 2 байт
  if (byte(message[0])==5) // Если 0 байт
  {thr = 0;}
  if (byte(message[0])==4) // Если 1 байт
  {thr = ((message[1]));}
  //Steering
  ster = ((message[2]) << 8) + ((message[3])); // Перевод в десятичное 2 байт
  if (byte(message[2])==5)  // Если 0 байт
  {ster = 0;}
  if (byte(message[2])==4)  // Если 1 байт
  {ster = ((message[3]));}
  analogWrite(11,thr/4);
  // Вывод на экран
  Serial.println();
  Serial.print("Throttle = ");
  Serial.print(thr);
  Serial.println();
  Serial.print("Steering = ");
    if (ster < 512)
  { Serial.print("Right ");
    Serial.print(512-ster);
  }
     if (ster > 512)
  { Serial.print("Left ");
    Serial.print(ster-512);
  }
  Serial.println();

  }
  ster2 = ster2 - ster;
  if (ster > 512)
  {
    ster2 = ster-512;
    digitalWrite(6,LOW);
    for (i = 512;i<ster2;i++)
    {
      digitalWrite(5,HIGH);
      delay(1);
      digitalWrite(5,LOW);
    }
  }
  if (ster < 512)
  {
    ster2 = 512-ster;
    digitalWrite(6,HIGH);
    for (i = 0;i<ster2;i++)
    {
      digitalWrite(5,HIGH);
      delay(1);
      digitalWrite(5,LOW);
    }
    digitalWrite(6,LOW);
  }

}
