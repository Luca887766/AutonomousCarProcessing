/*
 * ESP-32
 * Connessione modulo wifi:
 * CE   CSN   SCK   MOSI  MISO  VCC
 * 4    5     18    23    19    3.3v
*/

#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>

#define CE_PIN 4
#define CSN_PIN 5

const byte addresses[
6] = {"00001"};

RF24 radio(CE_PIN, CSN_PIN);

void setup() {
  Serial.begin(9600);
  radio.begin();
  radio.openReadingPipe(0, addresses);
  radio.setDataRate(RF24_2MBPS);
  radio.setPALevel(RF24_PA_MAX);
  radio.startListening();
}

void loop() {
  long dati[3];
  radio.read(&dati, sizeof(dati));
  Serial.print("L");
  Serial.println(dati[0]);
  Serial.print("R");
  Serial.println(dati[1]);
  Serial.print("C");
  Serial.println(dati[2]);
  delay(50);
}
