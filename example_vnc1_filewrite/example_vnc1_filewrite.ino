#include <SoftwareSerial.h>


// RX TX

SoftwareSerial usb(2, 3);

int RTSPin = 5;
int CTSPin = 4;

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

char usbRx;
byte charIn;

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

void loop() {
// Disable the sending on the USB device if you have filled more than 50% of your buffer (64 bytes by default)
if (usb.available() > 32) {
digitalWrite(CTSPin, HIGH);
} else {
digitalWrite(CTSPin, LOW);
}
if (usb.available() > 0) { // read the incoming byte
usbRx = usb.read();
if (usbRx == 0x0D) {
Serial.println();
} else {
Serial.print(usbRx);
}
}
if (Serial.available() > 0) { // read the incoming byte
if (Serial.read() == (byte)'~') {
charIn = 0;
while (charIn != (byte)'~') { // wait for header byte again
if (Serial.available() > 0) {
charIn = Serial.read();
Serial.print(charIn, HEX);
if (charIn == (byte)'~') {
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
