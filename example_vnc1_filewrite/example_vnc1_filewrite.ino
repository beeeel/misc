#include <SoftwareSerial.h>


// RX TX

SoftwareSerial usb(2, 3);

int RTSPin = 5;
int CTSPin = 4;
char usbRx;
byte charIn;

float tNow = 10.25;
float hNow = 50.75;
long int x, valToWrite;
int noOfChars, noOfnoOfChars;

char* strBuf[100]; // Assume I'm never gonna exceed 100 chars at once

void setup() {
pinMode(RTSPin, INPUT);
pinMode(CTSPin, OUTPUT);

usb.begin(9600);
//usb.begin(57600); // You can modify the default speed of the VDIP1 with firmware updates to be up to 57600 through newSoftSerial.
digitalWrite(CTSPin, LOW); // Set the CTS to low and keep it there.
Serial.begin(9600); // opens serial port, sets data rate to 9600 bps

Serial.print("Starting");

usb.print("IPA"); // Set to ascii mode.
usb.write(13);
}

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

void Tee(char* val[], int nChar){
// Assume it's fine to write to USB and serial
for (int i = 0; i < nChar; i++)
{
  usb.print(val[i]);
  Serial.print(val[i]);
} 
}

void Buff(char* str[], int nChar){
  for (int i = 0; i < nChar; i++)
  {
    strBuf[i] = str[i];
  }
}

void TeeInt(int val){
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
  Buff(*"CLF LOG.CSV",11);
  strBuf[11] = 13;
  Tee(strBuf, 12);
  
	//usb.print("CLF LOG.CSV");  // it closes the file
	//usb.print(13);       // return character
  digitalWrite(CTSPin, LOW);

}

void WriteLine()
{
  while (digitalRead(RTSPin) == HIGH) { }
	valToWrite=millis(); // time since we started logging
	noOfChars=1;
	x=valToWrite;                    // need to copy valToWrite as getting no of characters will consume it
	while (x>= 10)
	{                     		 // counts the characters in the number
		noOfChars++;             // thanks to D Mellis for this bit
		x/=10;     
	}

	noOfChars += 7;                  // Need to also write temp XX, hum XX, 2 "," and CR

  Tee(*"WRF ", 4);
  TeeInt(noOfChars);
  strBuf[0] = 13;
  Tee(strBuf, 1);
  TeeInt(valToWrite);
  Tee(*",", 1);
  TeeInt((int)(tNow+0.5));
  Tee(*",",1);
  TeeInt((int)(hNow+0.5));
  strBuf[0] = 13;
  Tee(strBuf,1);
  
	//usb.print("WRF ");               //write to file (file needs to have been opened to write first)
	//usb.print(noOfChars);            //needs to then be told how many characters will be written
	//usb.print(13);             	 //return to say command is finished

	//usb.print(valToWrite);
	//usb.print(",");
	//usb.print((int)(tNow+0.5));
	//usb.print(",");
	//usb.print((int)(hNow+0.5));
	//usb.print(13);             //write a return to the contents of the file (so each entry appears on a new line)

	tNow++;		// Increment t and h because this is a test
	hNow++;
 digitalWrite(CTSPin, LOW);

}

void loop() {

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

OpenFile();
WriteLine();
CloseFile();

delay(20000);
}
