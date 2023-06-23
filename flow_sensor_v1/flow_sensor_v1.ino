volatile int tLast, tNow;
volatile bool flowState;
int motorPin = 10;

void setup() {
  // put your setup code here, to run once:

  // Say hi!
  Serial.begin(9600);
  Serial.println(F("hi!"));
  
  // Does what it says
  attachInterrupt(digitalPinToInterrupt(2),inputChange,CHANGE); 

  // Prepare the output pin
  pinMode(motorPin, OUTPUT);
}

void loop() {
  // put your main code here, to run repeatedly:
  Serial.println("Got here!");

  while ( !flowState )
  {
    // Do nothing until a flow is detected
  }

  // Flow has been detected so turn on the output pin
  analogWrite(motorPin, HIGH);
  Serial.println("Turned on motor");
  
  while ( flowState )
  {
    Serial.println("Haven't turned it off yet");
    // Now check in case the flow has stopped
    delay(500);
    tNow = millis();
    // If the last event is more than 500ms ago turn off
    if ( tNow > ( tLast + 500 ) )
    {
      flowState = false;
    }
  }

  // Turn off the motor
  analogWrite(motorPin, LOW);
  Serial.println("Turned off motor");
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
