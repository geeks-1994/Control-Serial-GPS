
#ifndef SCREEN_FUNCTIONS_H
#define SCREEN_FUNCTIONS_H

#include <stdint.h>
#include "ConfigManager.h"


typedef struct {
    char inputNameKeypad[50];
    char chargeInput[50];
    char activityInput[50];
    char ImplementInput[50];
    char decimalCursors[50];
    char device[50];
    char stateScreen[50];
    bool stateActivity;
} ScreenFlow;


typedef struct {
 
bool screenCharge;
bool screenActivity;

} Proccess;




extern  ScreenFlow screenflow;
extern ConfigManager config;
extern Proccess  proccess;

extern bool flowlogic;

void GoHomePage(char event[][50]);
void getConfigData(char event[][50]);
void Showbanner(char event[][50]);
void ResponsePing(char event[][50]);
void DinaRefuel(char event[][50]);
void printRefuel(char event[][50]);
void Getfindespacho(char event[][50]);
void ShowInputScreen(char event[][50]);
void Printedvehicle(char event[][50]);
void QRscreen(char event[][50]);
void ExecuteBuzzer(char event[][50]);
void DebugSend(const char *label, const char *data);
void splashScreen(int value);
void dwinStartupXP();
void dwinErrorTone_Loud();

void showdatawork(const char* id);
void setDateScreen(char event[][50]);
void FactorySettings();
void SetConfigScreen();
void setConfig(char event[][50]);
void getShowInputData(const char* nameInput);
void homeScreen();
void SetEnableScreen();
void screenPrompt();
void cleanflow();
void restoreScreenFlow();
void saveScreenFlow();
void SaveBacklightValue(uint8_t brightness);
void LoadBacklightValue();



#endif