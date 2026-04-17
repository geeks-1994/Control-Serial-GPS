#include <sys/_stdint.h>
#include "WString.h"
#include "esp32-hal.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <stdint.h>
#include "SerialPorts.h"
#include "DwinToolsInterface.h"
#include "ParseModule.h"
#include "toolsFunctionsScreen.h"
#include <Arduino.h>
#include "Datalog.h"
#include "SendDataGPS.h"
#include "ConfigManager.h"

/// Variables tipo texto


ConfigManager config;

static const uint16_t  GSNM_station = 0x2100;
static const uint16_t vehicleID = 0x2110;
static const uint16_t unitMeasure = 0x2008;
static const uint16_t banner = 0x2200;
static const uint16_t  vevehicle = 0x1300;
static const uint16_t inputname = 0x1311;



///variablres numericas 
static const uint16_t  TotalGross = 0x100E;
static const uint16_t  heigtLevel = 0x1012;
static const uint16_t  flowRate  = 0x1000; 
static const uint16_t  preset = 0x1006;
static const uint16_t  counter = 0x100A;
static const uint16_t  volume = 0x1002;
static const uint16_t  inputName = 0x1100;
static const uint16_t  bannersplashscreen = 0x1300;
static const uint16_t  iconstatus = 0x1200;
static const uint16_t  txtLabor = 0x2250;
static const uint16_t  txtcharge = 0x2300;
static const uint16_t  txtissue =  0x2400;
static const uint16_t  implement = 0x1024;

//variables nivel texto
static  char *totalCounter = "<REQ|SCREEN|MAIN|CONFIG|TGAL>";
static  char *IDPCB        = "<REQ|SCREEN|MAIN|CONFIG|GSID>";
static  char *name_station = "<REQ|SCREEN|MAIN|CONFIG|GSNM>";
static  char *unit_measure = "<REQ|SCREEN|MAIN|CONFIG|GSUM>";

// estados de pantalla
ScreenFlow screenflow = {
   .inputNameKeypad = "",
   .chargeInput = "",
   .activityInput = "",
   .ImplementInput = "",
   .decimalCursors = "",
   .device = "",
   .stateScreen = "",
   .stateActivity = false
 
}; 

//flujo de pantalla
Proccess proccess = {
   .screenCharge = false,
   .screenActivity = false
};

bool flowlogic = false;


bool charToBool(const char* str) {

    if (str == NULL) return false;

    if (strcmp(str, "true") == 0) {
        return true;
    }

    if (strcmp(str, "false") == 0) {
        return false;
    }

    return false; // valor por defecto
};


int GetLengData(char event[][50]) {
    int i = 0;
    int len = 0;

    // Buscamos la primera cadena no vacía
    while (event[i][0] != '\0') {

        len = 0;

        // Contamos caracteres de esa cadena
        while (event[i][len] != '\0') {
            len++;
        }

        return len;   // regresamos la longitud
    }

    return 0; // no hay cadenas válidas
};


void DebugSend(const char *label, const char *data) {
    DebugSerial.print("[DEBUG] ");
    DebugSerial.print(label);
    DebugSerial.print(" -> ");
    DebugSerial.println(data);
};


float charToFloatCustom(const char *str, uint8_t decimals) {
    float result = 0.0f;
    float sign = 1.0f;
    uint32_t integerPart = 0;
    uint32_t decimalPart = 0;
    uint8_t decimalCount = 0;

    // Validación básica
    if (str == NULL || *str == '\0') {
        return 0.0f;
    }

    // Signo negativo
    if (*str == '-') {
        sign = -1.0f;
        str++;
    }

    // Parte entera
    while (*str >= '0' && *str <= '9') {
        integerPart = integerPart * 10 + (*str - '0');
        str++;
    }

    // Parte decimal
    if (*str == '.') {
        str++;
        while (*str >= '0' && *str <= '9' && decimalCount < decimals) {
            decimalPart = decimalPart * 10 + (*str - '0');
            decimalCount++;
            str++;
        }
    }

    // Convertimos a float
    result = (float)integerPart;

    float divisor = 1.0f;
    for (uint8_t i = 0; i < decimalCount; i++) {
        divisor *= 10.0f;
    }

    result += (float)decimalPart / divisor;

    return result * sign;
};



//funciones de session
void saveScreenFlow() {
    config.begin("screen");

    config.setBytes("flow", &screenflow, sizeof(screenflow));

    config.end();

    DebugSerial.println("ScreenFlow guardado");
};


bool loadScreenFlow() {

    config.begin("screen");

    size_t len = config.getBytes("flow", &screenflow, sizeof(screenflow));

    config.end();

    if (len != sizeof(screenflow)) {
        DebugSerial.println("No hay datos guardados");
        return false;
    }

    return true;
};


void restoreScreenFlow() {

    if (!loadScreenFlow()) {
        return;
    }

    if (screenflow.stateActivity == true) {

        DebugSerial.println("Restaurando estado...");

        // ir a pantalla 4
        dwinChangePage_VP(4);

        // restaurar textos
        if (strlen(screenflow.activityInput) > 0) {
            writeTextClean(banner, screenflow.activityInput, 150);
            writeTextClean(txtcharge,screenflow.chargeInput,150);
        }

        if (strlen(screenflow.ImplementInput) > 0) {
            float val = charToFloatCustom(screenflow.ImplementInput, 0);
            writeU32(implement, (uint32_t)val * 100);
        }
    }
};
//funciones de session

void showInvalidWorkMessage() {
    writeTextClean(txtLabor, "Invalido, vuelva a ingresar el numero", 150);
    delay(4000);
    writeTextClean(txtLabor, "...", 150);
}

void showdatawork(const char* id) {

    if (id == NULL) {
        showInvalidWorkMessage();
        return;
    }

    if (strcmp(screenflow.inputNameKeypad, "InputNameOne") == 0) {

        if (existChargeTP(id)) {
            const char* laborName = getChargeNameById(id);
            DebugSend("[cargo]", laborName);
            writeTextClean(txtcharge, laborName, 150);
          //data encode GPS
            char idFixed[50];
            snprintf(screenflow.chargeInput, sizeof(screenflow.chargeInput), "%s", laborName);
           snprintf(idFixed,sizeof(idFixed),"%s%s",id,"45");
            SendCodeGPS(idFixed);
            screenPrompt();

        } else {
            showInvalidWorkMessage();
        }

    } 
    else if(strcmp(screenflow.inputNameKeypad, "InputNametwo") == 0){
 
         if (existsLabor(id)) {
            const char* laborName = getLaborNameById(id);
            DebugSend("[Labor]", laborName);
            writeTextClean(banner, laborName, 150);
            
            //data encoded GPS
            char idFixed[50];

            snprintf(screenflow.activityInput, sizeof(screenflow.activityInput), "%s", laborName);
             snprintf(idFixed,sizeof(idFixed),"%s%s",id,"45");
            SendCodeGPS(idFixed);
           screenPrompt();
        } else {
            showInvalidWorkMessage();
        }

    }
    else if (strcmp(screenflow.inputNameKeypad, "Inputthree") == 0) {

        if (existsTP(id)) {
            const char* tpName = getLossTimeID(id);
            DebugSend("[T.P.]", tpName);
            writeTextClean(txtissue, tpName, 150);
            SendCodeGPS(id);
            screenflow.stateActivity = true;
            snprintf(screenflow.activityInput,sizeof(screenflow.activityInput),"%s",tpName);
            dwinChangePage_VP(4);
            screenflow.stateActivity = true;
            saveScreenFlow();
        } else {
            showInvalidWorkMessage();
        }

    } else if (strcmp(screenflow.inputNameKeypad, "InputNameFour") == 0) {

        float thirdfield = charToFloatCustom(id, 2);
        DebugSend("[TX]:", id);
        DebugSerial.println(thirdfield);
        snprintf(screenflow.ImplementInput, sizeof(screenflow.ImplementInput),"%s", id);
      uint32_t value = atoi(id);
        writeU32(implement,value * 100);
        SendCodeGPS(id);
        dwinChangePage_VP(4);
        saveScreenFlow();
    }
};


void FactorySettings(){

    config.begin("settings");

   config.setString("veVehicleId", "GKS-001");
    
    // Flujo principal de variables
   config.setString("InputNameOne","Cargo");
   config.setString("InputNametwo","Labor");
   

      config.setString("Inputthree", "Tiempo Perdido");
      config.setString("InputNameFour", "Implemento");
   
   // flujo de pantallas
   config.setBool("screenOne",true);
   config.setBool("screenTwo",true);

    config.end();
};

void SetConfigScreen(){

config.begin("settings");
String vehiclename = config.getString("veVehicleId", "default");
String InputName = config.getString("InputNameOne", "default");

writeTextClean(vevehicle,vehiclename.c_str(),50);
writeTextClean(inputname,InputName.c_str(),50);

config.end();

};


void setConfig(char event[][50]){

    config.begin("settings");

  char answer[50];
  int n = snprintf(answer, sizeof(answer), "%s%s", event[3], event[4]);

    DebugSend("configuracion:", answer);
  if (n < 0 || n >= (int)sizeof(answer)) {
    DebugSend("Error: comando truncado", answer);
    return;
  }

   
   if(strcmp(answer,"CONFIGVEHICLENAME") == 0){
   DebugSend("[TX]Vehicle:", event[5]);
   config.setString("veVehicleId", String(event[5]));

   }else if(strcmp(answer,"CONFIGINPUTNAME") == 0){
   
   DebugSend("[TX]Input Naame:", event[5]);
   config.setString("InputNameOne", String(event[5]));
   
   } else if(strcmp(answer,"CONFIGINPUTNAMETWO") == 0){
   
   DebugSend("[TX]Input Name:", event[5]);
   config.setString("InputNametwo", String(event[5]));
   
   } else if (strcmp(answer,"CONFIGINPUTNAMETHREE") == 0){

   DebugSend("[TX]Input Name:", event[5]);
   config.setString("Inputthree", String(event[5]));
   
   }else if (strcmp(answer,"CONFIGFACTORY") == 0){
   DebugSend("[TX] factory:", "RESTAURACION COMPLETA");
FactorySettings();
   } else if(strcmp(answer,"CONFIGSCREENONE") == 0 ){

      DebugSend("[TX] SCREEN ONE ",event[5]);
      config.setBool("screenOne",charToBool(event[5]));

   }else if(strcmp(answer,"CONFIGSCREENTWO") == 0 ){

      DebugSend("[TX] SCREEN TWO ",event[5]);
      config.setBool("screenTwo",charToBool(event[5]));

   }




     config.end();
};


void SetEnableScreen() {
config.begin("settings");
proccess.screenActivity = config.getBool("screenTwo",false);
proccess.screenCharge = config.getBool("screenOne",false);
config.end();
};


void SaveBacklightValue(uint8_t brightness) {
  if (brightness > 100) brightness = 100;

  config.begin("settings");
  config.setInt("brightness", brightness);
  config.end();
}


void LoadBacklightValue() {
  config.begin("settings");

  int brightness = config.getInt("brightness", 100);  // default = 100%

  config.end();

  // Validación
  if (brightness < 0) brightness = 0;
  if (brightness > 100) brightness = 100;

  DebugSerial.print("💡 Brightness cargado: ");
  DebugSerial.println(brightness);

  // Aplicar a la pantalla
  SetBacklight((uint8_t)brightness);
}


void setDateScreen(char event[][50]){

  char answer[50];
  int n = snprintf(answer, sizeof(answer), "%s%s", event[3], event[4]); 

  String Datasend = convertDateTimeToHex(event[5]);
  String codeAVL = "5AA50B82009C5AA5" + Datasend;
  DebugSerial.print("Trama HEX:");
  DebugSerial.println(codeAVL);
  byte buffer[32];

  int len = hexStringToBytesGPS(codeAVL, buffer, sizeof(buffer));

   DwinSerial.write(buffer,len);


DebugSend("[TX] SETDATE", "HORAS SET OK");

};





void getShowInputData(const char* nameInput){

config.begin("settings");

String InputData = config.getString(nameInput, "default");
strncpy(screenflow.stateScreen, nameInput, sizeof(screenflow.stateScreen) - 1);
DebugSend("[TX] SCREEN:", nameInput);

strncpy(screenflow.inputNameKeypad,nameInput, sizeof(screenflow.inputNameKeypad) - 1);

writeTextClean(inputname, InputData.c_str(),50);

dwinChangePage_VP(2);

config.end();

};

void homeScreen(){

if(screenflow.stateActivity == false){
dwinChangePage_VP(1);

}else if(screenflow.stateActivity == true){
dwinChangePage_VP(4);
}

};
 






void setActivityState() {

    screenflow.stateActivity = true;

    strcpy(screenflow.stateScreen, "SCREEN4");

    saveScreenFlow();
}


void screenPrompt() {

    if (proccess.screenCharge && screenflow.chargeInput[0] == '\0') {
        getShowInputData("InputNameOne");
        return;
    }

    if (proccess.screenActivity && screenflow.activityInput[0] == '\0') {
        getShowInputData("InputNametwo");
        return;
    }

    screenflow.stateActivity = true;
    strncpy(screenflow.stateScreen, "SCREEN4", sizeof(screenflow.stateScreen) - 1);
    screenflow.stateScreen[sizeof(screenflow.stateScreen) - 1] = '\0';

    saveScreenFlow();
    dwinChangePage_VP(4);
};


void cleanflow() {

    screenflow.chargeInput[0] = '\0';
    screenflow.activityInput[0] = '\0';
    screenflow.ImplementInput[0] = '\0';

writeTextClean(txtcharge,"",50);
writeTextClean(txtLabor,"",50);
writeTextClean(txtissue,"",50);
writeTextClean(banner,"",50); 
    DebugSend("[TX]", "Input clean ok");
    saveScreenFlow();
}


