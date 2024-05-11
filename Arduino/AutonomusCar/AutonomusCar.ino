/*
 * ARDUINO
 * Connessione modulo wifi:
 * CE   CSN   SCK   MOSI  MISO  VCC
 * 7    8     13    11    12    3.3v
*/
 
#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>

//PWM channel
#define PWMLEFT 6
#define PWMRIGHT 5

//motor
#define IN1 A0
#define IN2 A1
#define IN3 A2
#define IN4 A3

//sensor TRIG
#define TRIG1 3
#define TRIG2 10
#define TRIG3 9

//sensor ECHO
#define ECHO1 2
#define ECHO2 4
#define ECHO3 0

//Costanti debug per distanza
#define MAXDIST 80
#define MINDIST 25
#define MINDISTL 20

//PIN modulo radio
#define CE_PIN 7
#define CSN_PIN 8

//creo l'antenna
RF24 radio(CE_PIN, CSN_PIN);

//indirizzo antenna
const byte addresses[6] = {"12345"};

void setup() {
  //MOTOR
  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);
  pinMode(IN3, OUTPUT);
  pinMode(IN4, OUTPUT);
  //SENSOR TRIG
  pinMode(TRIG1, OUTPUT);
  pinMode(TRIG2, OUTPUT);
  pinMode(TRIG3, OUTPUT);
  //SENSOR ECHO
  pinMode(ECHO1, INPUT);
  pinMode(ECHO2, INPUT);
  pinMode(ECHO3, INPUT);
  
  Serial.begin(9600);
  radio.begin();
  radio.openWritingPipe(addresses);
}

void loop() {
  long dL, dR, dC;
  long tdL = MAXDIST, tdR = MAXDIST, tdC = MAXDIST;
  long dati[3];
  
  //LEFT
  dL = sLeft();
  if (dL != -1) tdL = dL;
  else dL = MAXDIST;
  dati[0] = dL;
  
  //RIGHT
  dR = sRight();
  if (dR != -1) tdR = dR;
  else dR = MAXDIST;
  dati[1] = dR;

  //CENTER
  dC = sCenter();
  if (dC != -1) tdC = dC;
  else dC = MAXDIST;
  dati[2] = dC;
  
  mAvanti();
  if (dC <= MINDIST) mGiro();
  else if(dL <= MINDISTL) mRight();
  else if (dR <= MINDISTL) mLeft();
  else mAvanti();
  radio.write(&dati, sizeof(dati));
}

//calcola distanza sensore Centrale
long sCenter() {
  digitalWrite(TRIG2, LOW);
  digitalWrite(TRIG2, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG2, LOW);
  long durata = pulseIn(ECHO2, HIGH);
  if (durata > 10000) {
    return MAXDIST;
  }
  long distanza = durata / 58.31;
  if (distanza > MAXDIST) {
    return MAXDIST;
  }
  return distanza;
}

//calcola distanza sensore a Destra
long sRight() {
  digitalWrite(TRIG1, LOW);
  digitalWrite(TRIG1, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG1, LOW);
  long durata = pulseIn(ECHO1, HIGH);
  if (durata > 10000) {
    return MAXDIST;
  }
  long distanza = durata / 58.31;
  if (distanza > MAXDIST) {
    return MAXDIST;
  }
  return distanza;
}

//calcola distanza sensore a Sinistra
long sLeft() {
  digitalWrite(TRIG3, LOW);
  digitalWrite(TRIG3, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG3, LOW);
  long durata = pulseIn(ECHO3, HIGH);
  if (durata > 10000) {
    return MAXDIST;
  }
  long distanza = durata / 58.31;
  if (distanza > MAXDIST) {
    return MAXDIST;
  }
  return distanza;
}

//Muove la macchina indietro
void mIndietro() {
  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, HIGH);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, LOW);
}

//Muove la macchina avanti
void mAvanti() {
  analogWrite(PWMRIGHT, 255);
  analogWrite(PWMLEFT, 255);
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, HIGH);
  digitalWrite(IN4, HIGH);
}

//Ferma la macchina
void mStop() {
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, LOW);
}

//Ruota la macchina verso destra
void mLeft() {
  analogWrite(PWMRIGHT, 255);
  analogWrite(PWMLEFT, 200);
  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, HIGH);
}

//Ruota la macchina verso sinistra
void mRight() {
  analogWrite(PWMRIGHT, 200);
  analogWrite(PWMLEFT, 255);
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, HIGH);
  digitalWrite(IN3, HIGH);
  digitalWrite(IN4, LOW);
}

//Ruota la macchina su se stessa
void mGiro() {
  analogWrite(PWMRIGHT, 255);
  analogWrite(PWMLEFT, 255);
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, HIGH);
  digitalWrite(IN3, HIGH);
  digitalWrite(IN4, LOW);
}
