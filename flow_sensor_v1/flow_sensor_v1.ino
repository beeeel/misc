

void setup() {
  // put your setup code here, to run once:

  setupFlowSensor();
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
