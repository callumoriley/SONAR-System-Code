#include <RH_ASK.h>
#include <SPI.h>

#define TRIG 9
#define ECHO 8
#define LED 13
#define TIMEBETWEENPULSES 10
#define MAX_STR_LEN 8

long duration;
float distance;
char sendBuffer[MAX_STR_LEN];

RH_ASK driver;

void setup() {
  pinMode(ECHO, INPUT);
  pinMode(TRIG, OUTPUT);
  pinMode(LED, OUTPUT);
  driver.init();
}

void loop() {
  digitalWrite(LED, HIGH); // getting 5V for the LED from pin 13 because I already used the 5V pins on the Arduino to power the distance sensor and the transmitter
  
  digitalWrite(TRIG, LOW); // this sends out a good trigger signal for the distance sensor
  delayMicroseconds(2);
  digitalWrite(TRIG, HIGH);
  delayMicroseconds(20);
  digitalWrite(TRIG, LOW);

  duration = pulseIn(ECHO, HIGH);
  distance = duration * 0.34 / 2; // calculates the distance in mm from the pulse length returned from the sensor

  dtostrf(distance, MAX_STR_LEN, 2, sendBuffer); // formats the distance into a string and sends it over RF link
  driver.send((uint8_t *)sendBuffer, MAX_STR_LEN);

  delay(TIMEBETWEENPULSES);
}
