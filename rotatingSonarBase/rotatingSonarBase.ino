#include <RH_ASK.h>
#include <SPI.h>

#define LIGHTSENSORTHRESHOLD 400
#define MAX_STR_LEN 8
#define MOTOR_PIN 3
#define TARGET_RPS 1.2
#define PWM_INCREMENT 1
#define PWM_DECREMENT 1

// experimentally determined the value for the light sensor threshold to be about 400

boolean sentStop = false; // keeps track if the light sensor has passed over the LED
unsigned long lastMillis = 0;
unsigned long currMillis;
int pwmVal = 200;

RH_ASK driver;

void setup() {
  Serial.begin(9600);
  if (!driver.init())
  {
    Serial.println("Initialization failed!");
  }
  pinMode(MOTOR_PIN, OUTPUT);
}

void loop() {
  uint8_t inBuffer[MAX_STR_LEN];
  uint8_t bufLen = sizeof(inBuffer);
  if (driver.recv(inBuffer, &bufLen)) // receive and print number from RF link
  {
    Serial.println((char*)inBuffer);
  }
  int sensorVal = analogRead(A0);
  if (sensorVal > LIGHTSENSORTHRESHOLD && sentStop == false)
  {
    currMillis = millis();
    Serial.println("s");
    sentStop = true;
    
    if ((float)1000.0/(currMillis - lastMillis) < TARGET_RPS)
    {
      pwmVal += PWM_INCREMENT;
    }
    else if ((float)1000.0/(currMillis - lastMillis) > TARGET_RPS)
    {
      pwmVal -= PWM_DECREMENT;
    }
    lastMillis = currMillis;
  }
  else if (sensorVal < LIGHTSENSORTHRESHOLD && sentStop == true)
  {
    sentStop = false; // light sensor value has to go below threshold to reset
  }
  
  if (pwmVal > 255) pwmVal = 255;
  else if (pwmVal < 0) pwmVal = 0;
  analogWrite(MOTOR_PIN, pwmVal);
}
