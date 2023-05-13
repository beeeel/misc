#include <SoftwareSerial.h>


// RX TX

SoftwareSerial usb(2, 3);

int RTSPin = 5;
int CTSPin = 4;
char usbRx;
byte charIn;

long int x, valToWrite, lastLogTime;
int noOfChars, noOfnoOfChars;

char* strBuf[100]; // Assume I'm never gonna exceed 100 chars at once

void SendToUsb(char val) {
  while (digitalRead(RTSPin) == HIGH) { }
  usb.print(val);
  digitalWrite(CTSPin, LOW);
}

void SendToUsbTermCmd() {
  while (digitalRead(RTSPin) == HIGH) { }
  usb.write(13);
  digitalWrite(CTSPin, LOW);

}

void Tee(char* val[], int nChar) {
  // Assume it's fine to write to USB and serial
  for (int i = 0; i < nChar; i++)
  {
    usb.print(val[i]);
    Serial.print(val[i]);
  }
}

void Buff(char* str[], int nChar) {
  for (int i = 0; i < nChar; i++)
  {
    strBuf[i] = str[i];
  }
}

void TeeInt(int val) {
  // For Teeing an int
  usb.print(val);
  Serial.print(val);
}

void OpenFile()
{
  while (digitalRead(RTSPin) == HIGH) { }
  // This next bit is horrible: The address-of turns the const char into char ** with only a warning from compile.
  Buff(*"OPW LOG.CSV", 11);
  strBuf[11] = 13;
  Tee(strBuf, 12);

  //usb.print("OPW LOG.CSV");               // open to write creates a file or appends to existing
  //usb.print(13);                    // return character
  digitalWrite(CTSPin, LOW);

}

void CloseFile()
{
  while (digitalRead(RTSPin) == HIGH) { }
  Buff(*"CLF LOG.CSV", 11);
  strBuf[11] = 13;
  Tee(strBuf, 12);

  //usb.print("CLF LOG.CSV");  // it closes the file
  //usb.print(13);       // return character
  digitalWrite(CTSPin, LOW);

}

void WriteLine()
{
  for (int i = 0; i < BUFSZ; i++) {
    while (digitalRead(RTSPin) == HIGH) { }
    valToWrite = millis(); // time since we started logging
    noOfChars = 1;
    x = valToWrite;                  // need to copy valToWrite as getting no of characters will consume it
    while (x >= 10)
    { // counts the characters in the number
      noOfChars++;             // thanks to D Mellis for this bit
      x /= 10;
    }

    noOfChars += 7;                  // Need to also write temp XX, hum XX, 2 "," and CR

    Tee(*"WRF ", 4);                //write to file (file needs to have been opened to write first)
    TeeInt(noOfChars);              //needs to then be told how many characters will be written
    strBuf[0] = 13;
    Tee(strBuf, 1);                 //return to say command is finished
    TeeInt(valToWrite);
    Tee(*",", 1);
    TeeInt((int)(V1Buff[i]));
    Tee(*",", 1);
    TeeInt((int)(V2Buff[i]));
    strBuf[0] = 13;
    Tee(strBuf, 1);                 //write a return to the contents of the file (so each entry appears on a new line)   

    digitalWrite(CTSPin, LOW);
  }
}