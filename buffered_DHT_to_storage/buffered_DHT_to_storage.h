// Number of seconds between logged readings
#define TLOG 1 
// Number of milliseconds between readings
#define TPOLL 100
// Number of readings to average every time
#define NAVG 100

// Number of readings to buffer
#define BUFSZ 1
// Working variables
float V1Buff[BUFSZ];
float V2Buff[BUFSZ];
int gateVolt[BUFSZ];
float vNow, tmp1, tmp2;
int buffCont = 0;


// Relays are on pins 5 to 8
const int gatePin = 3;
int gateState = 250;

// Voltage input pins - set to A0 and A1
const int V1Pin = 14;
const int V2Pin = 15;

// Voltostat - you will need to figure out how to calculate the output voltage in the function below
float highVolt = 29.72;// 31.45;


void readVoltstoBuff(float* V1Buff, float* V2Buff) {
  // Read the volts and buffer the result - perform averaging!
  vNow = 0.0;
  for (int idx = 0; idx < NAVG; idx++)
  {
    vNow += ((float) analogRead(V1Pin)) * 5 * (1.5/0.21) / 1023;
  }
  V1Buff[0] = vNow/NAVG;

  vNow = 0.0;
  for (int idx = 0; idx < NAVG; idx++)
  {
    vNow += ((float) analogRead(V2Pin)) * 5 * (1.5/0.21) / 1023;
  }
  V2Buff[0] = vNow/NAVG; 
}

void doVoltostat(float V1Now, float V2Now) {
//gateState = 255;
// Here you define the logic that relates the voltages read with the output PWM (0-255) 
  readVoltstoBuff(&V1Now, &V2Now);
  vNow = V1Now;
  if (vNow < highVolt) {
          Serial.print("voltage low... ");

    gateState -= 1;
    analogWrite(gatePin, gateState);
    delay(100);
    readVoltstoBuff(&V1Now, &V2Now);

    if (V1Now < vNow)
    {
      gateState -= 2;
      Serial.println("increased");
    }
  }
//  else {
//          Serial.print("voltage high ... ");
//
//    gateState -= 1;
//
//    analogWrite(gatePin, gateState);
//    delay(100);
//    readVoltstoBuff(&V1Now, &V2Now);
//
//    if (V1Now > vNow)
//    {
//      gateState += 2;
//            Serial.println("decreased");
//
//    }
//  }
  analogWrite(gatePin, gateState);
  gateVolt[buffCont] = gateState;

  Serial.print("GateState = ");
  Serial.println(gateState);
}