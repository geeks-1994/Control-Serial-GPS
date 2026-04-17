#ifndef GPS_CODE_H
#define GPS_CODE_H

#include <Arduino.h>
#include <ctype.h>
#include <stdlib.h>


#define LED_BUILTIN 2
#define LED_CONNTTL 14

void SendCodeGPS(const char* id);
bool SendRFID_MF7_Teltonika(const char* input);
void blinkCONNTTL(int attempts, int duration);
void blinkLED(int attempts, int duration);
const char* convertDateTimeToHex(const char* input);
String toHexStringGPS(const char* input); 
int hexStringToBytesGPS(const String& hexStr, byte* buffer, size_t bufferSize);

#endif