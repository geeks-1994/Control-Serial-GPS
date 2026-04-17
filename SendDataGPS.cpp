#include <Arduino.h>
#include "toolsFunctionsScreen.h"
#include "SerialPorts.h"
#include "SendDataGPS.h"
#include <stdio.h>
#include <string.h>
#include <Arduino.h>
#include <climits>
#include <ctype.h>
#include <stdlib.h>


String ToHexGPS(uint32_t value){

String hexValue = String(value,HEX);
hexValue.toUpperCase();

return hexValue;

}


// Convierte una cadena hexadecimal tipo "023BA897700D0A03" a bytes binarios
int hexStringToBytesGPS(const String& hexStr, byte* buffer, size_t bufferSize) {
  int len = hexStr.length() / 2;
  if (len > (int)bufferSize) len = bufferSize;

  for (int i = 0; i < len; i++) {
    String byteStr = hexStr.substring(i * 2, i * 2 + 2);
    buffer[i] = (byte) strtol(byteStr.c_str(), NULL, 16);
  }
  return len;
};





String toHexStringGPS(const char* input) {
  String hexString = "";
  while (*input) {
    char buf[3];
    sprintf(buf, "%02X", *input); // convierte cada carácter a dos dígitos hex
    hexString += buf;
    input++;
  }
  return hexString;
};


String toAsciiHexString(const char* text) {
  String out = "";

  while (*text) {
    char buf[3];
    sprintf(buf, "%02X", (uint8_t)(*text));
    out += buf;
    text++;
  }

  return out;
}


void SendCodeGPS(const char* id) {
  char message[100];

  String dataSend = toAsciiHexString(id);   // convierte "4600" -> "34363030"
  String codecAVL = "02" + dataSend + "0D0A03";

  DebugSerial.print("Trama HEX: ");
  DebugSerial.println(codecAVL);

  byte buffer[32];
  int length = hexStringToBytesGPS(codecAVL, buffer, sizeof(buffer));

  if (length <= 0) {
    DebugSerial.println("Error convirtiendo la trama a bytes");
    return;
  }

  HostSerial.write(buffer, length);

  DebugSerial.print("Trama enviada: ");
  for (int i = 0; i < length; i++) {
    DebugSerial.printf("%02X ", buffer[i]);
  }
  DebugSerial.println();
}



//funcion para  enviar data rfid teltonika
bool SendRFID_MF7_Teltonika(const char* input) {
  if (input == NULL) {
    DebugSerial.println("MF7 ERROR: input null");
    return false;
  }

  // 1) Saltar espacios iniciales
  while (*input == ' ' || *input == '\t' || *input == '\r' || *input == '\n') {
    input++;
  }

  // 2) Validar vacio
  if (*input == '\0') {
    DebugSerial.println("MF7 ERROR: input vacio");
    return false;
  }

  // 3) Copiar solo digitos decimales
  char cleanInput[16];
  int len = 0;
  const char* p = input;

  while (*p != '\0' &&
         *p != ' ' &&
         *p != '\t' &&
         *p != '\r' &&
         *p != '\n') {

    if (!isdigit((unsigned char)*p)) {
      DebugSerial.println("MF7 ERROR: solo numeros decimales permitidos");
      return false;
    }

    if (len >= (int)(sizeof(cleanInput) - 1)) {
      DebugSerial.println("MF7 ERROR: input demasiado largo");
      return false;
    }

    cleanInput[len++] = *p;
    p++;
  }

  cleanInput[len] = '\0';

  if (len == 0) {
    DebugSerial.println("MF7 ERROR: input vacio");
    return false;
  }

  // 4) Convertir decimal a entero
  errno = 0;
  char* endPtr = NULL;
  unsigned long long value = strtoull(cleanInput, &endPtr, 10);

  if (errno != 0) {
    DebugSerial.println("MF7 ERROR: overflow en conversion");
    return false;
  }

  if (endPtr == cleanInput || *endPtr != '\0') {
    DebugSerial.println("MF7 ERROR: conversion invalida");
    return false;
  }

  // 5) El documento MF7 usa 8 HEX para tarjetas tipo 4 bytes
  //    8 HEX = 32 bits maximos
  if (value > 0xFFFFFFFFULL) {
    DebugSerial.println("MF7 ERROR: el valor excede 8 HEX");
    return false;
  }

  // 6) Convertir a 8 HEX ASCII, completando con ceros a la izquierda
  char hexData[9];
  snprintf(hexData, sizeof(hexData), "%08llX", value);

  // 7) Debug
  DebugSerial.print("Entrada decimal: ");
  DebugSerial.println(cleanInput);

  DebugSerial.print("HEX normalizado: ");
  DebugSerial.println(hexData);

  DebugSerial.print("MF7 TX: 02 ");
  DebugSerial.print(hexData);
  DebugSerial.println(" 0D 0A 03");

  // 8) Enviar trama MF7 real
  HostSerial.write(0x02);      // STX
  HostSerial.print(hexData);   // DATA ASCII HEX
  HostSerial.write(0x0D);      // CR
  HostSerial.write(0x0A);      // LF
  HostSerial.write(0x03);      // ETX

  return true;
}


// Parpadeo del LED
void blinkLED(int attempts, int duration) {
  for (int i = 0; i < attempts; i++) {
    digitalWrite(LED_BUILTIN, HIGH);
    delay(duration);
    digitalWrite(LED_BUILTIN, LOW);
    delay(duration);
  }
};


void blinkCONNTTL(int attempts, int duration){
for (int i = 0; i < attempts; i++) {
    digitalWrite(LED_CONNTTL, HIGH);
    delay(duration);
    digitalWrite(LED_CONNTTL, LOW);
    delay(duration);
  }
};



const char* convertDateTimeToHex(const char* input) {

    static char output[30];

    int yy, mm, dd, hh, mi, ss;

    if (sscanf(input, "%d-%d-%d %d:%d:%d",
               &yy, &mm, &dd,
               &hh, &mi, &ss) != 6) {
        return "00-00-00 00:00:00";
    }

    snprintf(output, sizeof(output),
             "%02X%02X%02X%02X%02X%02X",
             yy, mm, dd, hh, mi, ss);

    return output;
};

