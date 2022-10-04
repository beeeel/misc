#define TLOG 1

// Number of readings to buffer
#define BUFSZ 6
// Working variables
float V1Buff[BUFSZ];
float V2Buff[BUFSZ];
float vNow;
int buffCont = 0;

// Relays are on pins 5 to 8
const int gatePin = 5;
int gateState = 0;

// Voltage input pins - set to A0 and A1
const int V1Pin = 14;
const int V2Pin = 15;

// Voltostat - you will need to figure out how to calculate the output voltage in the function below
float highTemp = 16.0;

void doVoltostat(float V1Now, float V2Now) {
  // Here you define the logic that relates the voltages read with the output PWM (0-255) 
  if (tNow > highTemp) {
    gateState = 128;
  }
  else {
    gateState = 0;
  }
  analogWrite(gatePin, gateState);
}

void readVoltstoBuff(float* V1Buff, float* V2Buff) {
  // Read the volts and buffer the result
  vNow = 0.0;
  vNow = analogRead(V1Pin);
  V1Buff[0] = vNow;
  vNow = 0.0;
  vNow = analogRead(V2Pin);
  V2Buff[0] = vNow;
}
