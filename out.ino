#include <VirtualWire.h>

const int led_pin = 13; // Пин светодиода
const int transmit_pin = 12; // Пин подключения передатчика
const int thr = A0; //Пин акк
const int steer = A1; //Пин управления

void setup()
{
  vw_set_tx_pin(transmit_pin);
  vw_setup(2000);       // Скорость передачи (Бит в секунду)
  Serial.begin(9600);
  Serial.println("Engine powering up");
  pinMode(led_pin, OUTPUT);
  pinMode(transmit_pin, OUTPUT);
  pinMode(thr, INPUT);
  pinMode(steer, INPUT);
}

void loop()
{

  int thrt,ster;
  char msg[4];
  thrt = analogRead(thr); // Чтение скорости
  ster = analogRead(steer); // Чтение поворота
  Serial.println(ster);
  //Throttle
  if (thrt > 255) { // Разбить на байты
  msg[1] = byte(thrt);
  msg[0] = byte(thrt >> 8);
  }
  else { //Назначить код если только младший байт
  msg[1] = byte(thrt);
  msg[0] = 4;   
  } //Назначить код если 0 байт
  if (thrt == 0) {
  msg[1] = 1;
  msg[0] = 5;
  }
  //Steering
  if (ster > 255) { // Разбить на байты
  msg[3] = byte(ster);
  msg[2] = byte(ster >> 8);
  }
  else { //Назначить код если только младший байт
  msg[3] = byte(ster);
  msg[2] = 4;   
  } //Назначить код если 0 байт
  if (ster == 0) {
  msg[3] = 1;
  msg[2] = 5;
  }
  digitalWrite(led_pin, HIGH); // Зажигаем светодиод в начале передачи
  vw_send((uint8_t *)msg, strlen(msg)); // Отправка сообщения
  vw_wait_tx(); // Ожидаем окончания отправки сообщения
  digitalWrite(led_pin, LOW); // Гасим светодиод в конце передачи
  delay(500); // Пауза 0.5 секунды
}
