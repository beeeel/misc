#include "buffered_DHT_to_storage.h"
#include "filewrite.h"

void setup() {
  // Open serial port at baud rate:
  Serial.begin(9600);

  Serial.println(F("DHTxx buffered write test!"));

  pinMode(gatePin, OUTPUT);

  pinMode(V1Pin, INPUT);
  pinMode(V2Pin, INPUT);
  pinMode(RTSPin, INPUT);
  pinMode(CTSPin, OUTPUT);

  usb.begin(9600);
  //usb.begin(57600); // You can modify the default speed of the VDIP1 with firmware updates to be up to 57600 through newSoftSerial.
  digitalWrite(CTSPin, LOW); // Set the CTS to low and keep it there.

  usb.print("IPA"); // Set to ascii mode.
  usb.write(13);

}

void loop() {
  // put your main code here, to run repeatedly:

  // Check if there's anything waiting
  CheckUsb();
  CheckSerial();


  // Get readings into buffer
  buffCont = 0;
  do {
    x = millis()/1000; // Time now in s
    
    readVoltstoBuff(&V1Buff[buffCont], &V2Buff[buffCont]);
    doVoltostat(V1Buff[buffCont], V2Buff[buffCont]);
    //Serial.println("Did a reading");
    if (x > lastLogTime + TLOG) // If we're on a logging interval (to nearest second)
    {
      buffCont += 1;
      lastLogTime = x;
      //Serial.println("Buffered a reading");
    }
    else
    {
      //delayMicroseconds(TPOLL*1e6); // Time between readings in us
      delay(TPOLL); // Time between readings in ms
      //Serial.println("Waited");
    }
  } while ( buffCont < BUFSZ );

  // Check if there's anything waiting
  CheckUsb();
  CheckSerial();


  writeBufftoSerial(V1Buff, V2Buff);

  //OpenFile();
  //WriteLine();
  //CloseFile();


}

void CheckUsb() {
  // Disable the sending on the USB device if you have filled more than 50% of your buffer (64 bytes by default)
  if (usb.available() > 32)
  {
    digitalWrite(CTSPin, HIGH);
  }
  else
  {
    digitalWrite(CTSPin, LOW);
  }

  // If USB has something for me
  if (usb.available() > 0)
  { // read the incoming byte
    usbRx = usb.read();
    if (usbRx == 0x0D)
    {
      Serial.println();
    }
    else
    {
      Serial.print(usbRx);
    }
  }
}

void CheckSerial() {
  // If Serial (PC) has something for me
  if (Serial.available() > 0)
  { // read the incoming byte
    if (Serial.read() == (byte)'~')
    {
      charIn = 0;
      while (charIn != (byte)'~')
      { // wait for header byte again
        if (Serial.available() > 0)
        {
          charIn = Serial.read();
          Serial.print(charIn, HEX);
          if (charIn == (byte)'~')
          {
            Serial.println("breaking");
            break;
          }
          SendToUsb(charIn);
        }
      }
      SendToUsbTermCmd();
    }
  }
}

void writeBufftoSerial(float* tBuff, float* hBuff) {
  for (int i = 0; i < BUFSZ; i++) {
    Serial.print(F("V1 "));
    Serial.print(i);
    Serial.print(F(" = "));
    Serial.print(tBuff[i]);
    Serial.print(F("V   V2 = "));
    Serial.print(hBuff[i]);
    Serial.println(F("V"));
  }
}