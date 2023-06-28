// Testvoid 

volatile int tLast, tNow;
volatile bool flowState;
int motorPin = 10;

void setupFlowSensor()
{
  // Say hi!
  Serial.begin(9600);
  Serial.println(F("Hi from flow sensor!"));
  
  // Does what it says
  attachInterrupt(digitalPinToInterrupt(2),inputChange,CHANGE); 

  // Prepare the output pin
  pinMode(motorPin, OUTPUT);
}

void inputChange()
{
  tNow = millis();
  // if the last event is more than 1000ms ago do nothing
  if ( tNow > ( tLast + 1000 ) )
  {
    tLast = tNow;
  }
  else
  {
    flowState = true;
  }
}
