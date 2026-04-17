//incluyo librerias  de parseo o manejo de eventos, libreria para controlar los eventos de la pantalla  y cntrol de puestros seriales
#include "ParseModule.h"
#include "SerialPorts.h"
#include "DwinToolsInterface.h"
#include "toolsFunctionsScreen.h"
#include "SendDataGPS.h"
#include "IONGPS.h"
// variables globales de uso;
String InputSerial = "";
char messageConvert[100];


// ===== Serial RX buffer =====
static char rxBuffer[256];
static uint16_t rxIndex = 0;


//lista de funciones 
typedef void (*FunctionType)(char[][50]);


//Se define la estructura para el uso de las funciones 
typedef struct {
const  char* name;
FunctionType function;

} Command;




void readSerial(Stream &port) {

    while (port.available() > 0) {

        char c = port.read();

        // Inicio de trama
        if (c == '<') {
            rxIndex = 0;
        }

        // Guardar carácter
        if (rxIndex < sizeof(rxBuffer) - 1) {
            rxBuffer[rxIndex++] = c;
        }

        // Fin de trama → procesar
        if (c == '>') {
            rxBuffer[rxIndex] = '\0';
            processMessage(rxBuffer);
            rxIndex = 0;
        }
    }
};




void processMessage(const char *msg) {

    DebugSerial.print("[RX] ");
    DebugSerial.println(msg);

    char messageConvert[256];
    char tokens[20][50];

    strncpy(messageConvert, msg, sizeof(messageConvert) - 1);
    messageConvert[sizeof(messageConvert) - 1] = '\0';

    int N_messages = ParseMessage(messageConvert, tokens);

    const char *Method = WordKey(N_messages, tokens);

    DebugSerial.print("[METHOD] ");
    DebugSerial.println(Method);

    executeCommand(Method, tokens);
};




// lista de comandos activos

Command commands[] = {
 {"REQAPPCONFIG",setConfig},
 {"REQAPPHTC",setDateScreen}
};




// Buscar y ejecutar el comando
const char* executeCommand(const char* commandName, char event[][50]) {
    for (size_t i = 0; i < sizeof(commands) / sizeof(commands[0]); i++) {
        if (strcmp(commands[i].name, commandName) == 0) {
            commands[i].function(event); // Ejecutar la función
            return "Comando ejecutado correctamente";
        }
    }
    return "Comando no agregado";
};


//Enviar un comando




void setup() {
  // put your setup code here, to run once:
  pinMode(LED_BUILTIN, OUTPUT);
  pinMode(LED_CONNTTL, OUTPUT);
  digitalWrite(LED_BUILTIN, LOW);
  digitalWrite(LED_CONNTTL, LOW);

  SerialPorts::begin();

  DebugSerial.println("Debug Listo por USB");
  delay(4000);
  dwinChangePage_VP(1);
  //FactorySettings();
   SetEnableScreen();
   
   SetConfigScreen();

   restoreScreenFlow();
//SetBacklight(50);
delay(1000);
LoadBacklightValue();
LoadBacklightValue();
}

void loop() {
  // put your main code here, to run repeatedly:
   // USB (Debug)
    if (DebugSerial.available() > 0) {
        readSerial(DebugSerial);
    }

    // UART Host (Raspberry / CPU)
    if (HostSerial.available() > 0) {
        readSerial(HostSerial);
    }


    if(DwinSerial.available() > 0){
        
        DWIN_ReadFrames(DwinSerial);
    
    }

    // Leer siempre para poder detectar fin de bloque por timeout
    readHardwareKeypad();
  

// esto es para el bleuetooth aun no decido  como usarlo 

// if(PortBt.available() > 0 ){

// readSerial(PortBt);

// } 


}
