#ifndef IOGPS_FUNCTIONS_H
#define IOGPS_FUNCTIONS_H


typedef struct {
    uint32_t lastValue;
    uint32_t lastChangeTime;
    uint32_t windowStart;
    uint32_t eventCount;
    float frequency;
} FrequencyTracker;



extern FrequencyTracker frequency;



void readHardwareKeypad();


#endif