#include "DHT.h"

// DHT 11s are on pins 2 and 3
#define DHTTYPE DHT11
#define DHTPIN 2
DHT dht(DHTPIN, DHTTYPE);
// Number of readings to buffer
#define BUFSZ 6
// Working variables
float tempBuff[BUFSZ];
float humBuff[BUFSZ];
float tNow;
float hNow;
int buffCont = 0;

// Relays are on pins 5 to 8
const int ledPin = 5;
int ledState = 0;

// Thermostat
float highTemp = 16.0;

void setup() {
  // Open serial port at baud rate:
  Serial.begin(9600);
}
  
  Serial.println(F("DHTxx buffered test!"));
  
  pinMode(ledPin, OUTPUT);

  dht.begin();
}

void loop() {
  // put your main code here, to run repeatedly:

  // Get readings into buffer
  do {
    delay(2000);
    readDHTtoBuff(&tempBuff[buffCont], &humBuff[buffCont]);
    doThermostat(tempBuff[buffCont]);
    buffCont += 1;
  } while ( buffCont < BUFSZ );
  buffCont = 0;

  writeBufftoSerial(tempBuff, humBuff);

}

void readDHTtoBuff(float* tBuff, float* hBuff) {
  // Read the DHT and buffer the result
  tNow = 0.0;
  tNow = dht.readTemperature();
  tBuff[0] = tNow;
  hNow = 0.0;
  hNow = dht.readHumidity();
  hBuff[0] = hNow;
}

void writeBufftoSerial(float* tBuff, float* hBuff) {
    for (int i = 0; i < BUFSZ; i++) {
      Serial.print(F("Temperature "));
      Serial.print(i);
      Serial.print(F(" = "));
      Serial.print(tBuff[i]);
      Serial.print(F("°C   Humidity = "));
      Serial.print(hBuff[i]);
      Serial.println(F("%"));
  }
}

void doThermostat(float tNow) {
  if (tNow > highTemp) {
    ledState = 128;
  }
  else {
    ledState = 0;
  }
  analogWrite(ledPin, ledState);
}
