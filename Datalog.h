#ifndef Datalog_H
#define Datalog_H

#include <Arduino.h>


const char* getLaborNameById(const char* id);
const char* getLossTimeID(const char* id);
const char* getChargeNameById(const char* id);
bool existsLabor(const char* id);
bool existsTP(const char* id);
bool existChargeTP(const char* id);


#endif