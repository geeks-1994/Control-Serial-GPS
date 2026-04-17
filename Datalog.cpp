#include "Datalog.h"
#include <string.h>

typedef struct {
    const char* id;
    const char* name;
} LaborItem;



typedef struct {
const char* id;
const char* name;

}  LossTime;




static const LaborItem laborList[] = {

    {"1100", "ADMINISTRACION DE FINCA"},
    {"1900", "PRIMERA FULEADA MECANIZACION AGRICOLA"},
    {"1910", "PRIMERA FULEADA RIEGOS"}, 
    {"2020", "ACOMODO MANUAL DE RASTROJO VERDE"},
    {"2040", "SACANDO PIEDRA"},
    {"2050", "ADECUACION DE TIERRAS"},
    {"2060", "SUBSOLADO"},
    {"2070", "ARADURA O VOLTEO"},
    {"2080", "RASTREADO O PULIDO"},
    {"2090", "NIVELADO CON RUFA"},
    {"2100", "SURQUEADO MECANICO"},
    {"2120", "TARJETEADO Y ESTAQUILLADO"},
    {"2130", "CORTE DE SEMILLA"},
    {"2140", "CARGA(TRANSPORTE Y DESCARGA DE SEMILLA)"},
    {"2150", "APLICACION DE FERTILIZANTE E INSECTICIDA"},
    {"2160", "SIEMBRA MANUAL"},
    {"2170", "SIEMBRA MECANICA"},
    {"2180", "TAPADO MECANICO"},
    {"2190", "RESIEMBRA MANUAL"},
    {"2200", "PRIMERA ELIMINACION DE MEZCLA VARIETAL"},
    {"2220", "RIEGO ASPERSION POSCORTE"},
    {"2230", "RIEGO ASPERSION PRECORTE"},
    {"2310", "RIEGO AVANCE FRONTAL PRECORTE"},
    {"2320", "1ER. C.Q. MALEZAS EQUIPO P.M."},
    {"2330", "2DO. C.Q. MALEZAS EQUIPO P.M."},
    {"2340", "1ER. C.Q. MALEZAS EQUIPO P.C."},
    {"2350", "2DO. C.Q. MALEZAS EQUIPO P.C."},
    {"2360", "1ER. C.Q. MALEZAS EQUIPO MECANICO"},
    {"2370", "2DO. C.Q. MALEZAS EQUIPO MECANICO"},
    {"2380", "FOQUEO QUIMICO MALEZAS"},
    {"2390", "C.Q. MALEZAS LAB. MIN. P.M."},
    {"2400", "C.Q. MALEZAS LAB. MIN. P.C."},
    {"2430", "PODA MANUAL DE SEMILLERO"},
    {"2440", "PODA MECANICA DE SEMILLERO"},
    {"2470", "RONDEO MECANICO"},
    {"2480", "RONDEO QUIMICO"},
    {"2490", "CULTIVO"},
    {"2500", "FERTILIZACION MECANICA GRANULADA"},
    {"2501", "TRACTORISTA A EN FERTILIZACION MECANICA GRANULADA"},
    {"2510", "FERTILIZACION MANUAL GRANULADA"},
    {"2530", "CONTROL DE PLAGAS"},
    {"2560", "CONSERVACION DE SUELOS"},
    {"2570", "DESPEJE DE RONDAS"},
    {"2580", "GUARDIAN EN CANAL SEMILLERO"},
    {"2590", "RIEGO MINI ASPERSION POSCORTE"},
    {"2620", "RETAPADO DE SEMILLA EN SIEMBRA"},
    {"2650", "RIEGO MIDI ASPERSION PRECORTE "},
    {"2670", "APLICACION DE MADURANTE"},
    {"2680", "FERTILIZACION SUBPRODUCTOS ORGANICOS"},
    {"2682", "TRACTORISTA A EN FERTILIZACION SUBPRODUCTOS ORGANICOS"},
    {"2700", "APLICACION DE PREMADURANTE"},
    {"2710", "SACADO DE PIEDRA MECANICO"},
    {"2720", "APLICACION DE BIOESTIMULANTES"},
    {"2730", "C.Q. MALEZAS LAB. MI. E. MECANICO"},
    {"2740", "C.Q. MALEZAS LAB. MIN. DRON"},
    {"2760", "RIEGO ASPERSION CANON VIAJERO POSCORTE"},
    {"2770", "PERSONAL EN ESPERA POR REPARACION"},
    {"3010", "REQUEMA"},
    {"3020", "SACANDO PIEDRA"},
    {"3022", "TRACTORISTA B SACANDO PIEDRA"},
    {"3030", "ESTUDIOS TOPOGRAFICOS"},
    {"3035", "TRACTORISTA B EN ESTUDIOS TOPOGRAFICOS"},
    {"3040", "ADECUACION DE TIERRAS"},
    {"3050", "SUBSOLADO"},
    {"3060", "ARADURA O VOLTEO"},
    {"3070", "RASTREADO O PULIDO"},
    {"3080", "NIVELADO CON RUFA"},
    {"3090", "SURQUEADO MECANICO"},
    {"3110", "TARJETEADO Y ESTAQUILLADO"},
    {"3120", "CORTE DE SEMILLA"},
    {"3124", "JORNALERO TRATAMIENTO QUIMICO DE SEMILLA"},
    {"3130", "CARGA(TRANSPORTE Y DESCARGA DE SEMILLA)"},
    {"3140", "APLICACION DE FERTILIZANTE E INSECTICIDA"},
    {"3142", "TRACTORISTA B EN APLICACION DE FERTILIZANTE E INSECTICIDA"},
    {"3150", "SIEMBRA MANUAL"},
    {"3160", "SIEMBRA MECANICA"},
    {"3162", "TRACTORISTA B EN SIEMBRA MECANICA"},
    {"3170", "RESIEMBRA MANUAL"},
    {"3180", "RIEGO ASPERSION POSCORTE"},
    {"3190", "RIEGO BOMBEO GRAVEDAD POSCORTE"},
    {"3191", "TRASLADO O INSTALACION DE EQUIPOSCORTE BOMBEO - GRAVEDAD POSCORTE"},
    {"3200", "1ER. C.Q. MALEZAS EQUIPO P.M."},
    {"3201", "TRACTORISTA A EN 1ER. C.Q. MALEZAS EQUIPO P.M."},
    {"3210", "1ER. C.Q. MALEZAS EQUIPO P.C."},
    {"3230", "ARRANQUE DE MALEZAS"},
    {"3240", "RONDEO MANUAL"},
    {"3250", "RONDEO MECANICO"},
    {"3260", "RONDEO QUIMICO"},
    {"3270", "CULTIVO"},
    {"3280", "FERTILIZACION MECANICA GRANULADA"},
    {"3290", "FERTILIZACION MANUAL GRANULADA"},
    {"3310", "FERTILIZACION AEREA"},
    {"3320", "CONTROL DE PLAGAS"},
    {"3330", "APLICACION DE MADURANTE"},
    {"3340", "APLICACION DE INHIBIDORES"},
    {"3360", "CONSERVACION DE SUELOS"},
    {"3370", "RIEGO MINI ASPERSION POSCORTE"},
    {"3380", "DESPEJE DE RONDAS"},
    {"3390", "RIEGO MINI ASPERSION PRECORTE"},
    {"3400", "MUESTREO DE PLAGAS"},
    {"3410", "PERSONAL EN ESPERA POR REPARACION"},
    {"3420", "RIEGO GRAVEDAD - GRAVEDAD POSCORTE"},
    {"3430", "2DO. C.Q. MALEZAS EQUIPO P.M."},
    {"3440", "C.Q. MALEZAS LAB. MIN. P.M."},
    {"3450", "2DO. C.Q. MALEZAS EQUIPO P.C."},
    {"3460", "C.Q. MALEZAS LAB. MIN. P.C."},
    {"3470", "1ER. C.Q. MALEZAS EQUIPO MECANICO"},
    {"3480", "2DO. C.Q. MALEZAS EQUIPO MECANICO"},
    {"3490", "C.Q. MALEZAS LAB. MIN. E. MECANICO"},
    {"3520", "GUARDIAN EN CANAL PLANTIA"},
    {"3530", "TAPADO MECANICO"},
    {"3540", "FOQUEO QUIMICO MALEZAS"},
    {"3550", "RIEGO AVANCE FRONTAL POSCORTE"},
    {"3560", "RIEGO PIVOTE POSCORTE"},
    {"3570", "RIEGO ASPERSION PRECORTE"},
    {"3590", "RIEGO GRAVEDAD - GRAVEDAD PRECORTE "},
    {"3610", "RIEGO PIVOTE PRECORTE"},
    {"3640", "RIEGO PIVOTE RENTADO PRECORTE"},
    {"3660", "RETAPADO DE SEMILLA EN SIEMBRA"},
    {"3690", "RIEGO MIDI ASPERSION PRECORTE "},
    {"3700", "RIEGO MIDI ASPERSION POSCORTE"},
    {"3710", "FERTILIZACION SUBPRODUCTOS ORGANICOS"},
    {"3720", "TRITURADO MECANICO DE RAC"},
    {"3730", "APLICACION DE PREMADURANTE"},
    {"3740", "SACADO DE PIEDRA MECANICO"},
    {"3750", "APLICACION DE BIOESTIMULANTES"},
    {"3770", "RIEGO AVANCE FRONTAL RENTADO PRECORTE"},
    {"3780", "RIEGO AVANCE FRONTAL RENTADO POSCORTE"},
    {"3790", "RESIEMBRA MECANICA"},
    {"3800", "RIEGO ASPERSION CANON VIAJERO PRECORTE"},
    {"3810", "RIEGO ASPERSION CANON VIAJERO POSCORTE"},
    {"3820", "CORTE DE SEMILLA PARA SIEMBRA MECANICA"},
    {"4020", "SACANDO PIEDRA"},
    {"4030", "RESIEMBRA MANUAL"},
    {"4040", "DESCARNE"},
    {"4041", "TRACTORISTA A EN DESCARNE"},
    {"4050", "SUBSOLADO"},
    {"4060", "CULTIVO"},
    {"4061", "Tractorista A en cultivo"},
    {"4062", "TRACTORISTA B EN CULTIVO"},
    {"4070", "RIEGO ASPERSION POSCORTE"},
    {"4080", "RIEGO BOMBEO GRAVEDAD POSCORTE"},
    {"4090", "1ER. C.Q. MALEZAS EQUIPO P.M."},
    {"4100", "1ER. C.Q. MALEZAS EQUIPO P.C."},
    {"4120", "ARRANQUE DE MALEZAS"},
    {"4130", "RONDEO MANUAL"},
    {"4140", "RONDEO MECANICO"},
    {"4142", "TRACTORISTA B EN RONDEO MECANICO"},
    {"4150", "RONDEO QUIMICO"},
    {"4151", "TRACTORISTA A EN RONDEO QUIMICO"},
    {"4160", "FERTILIZACION MECANICA GRANULADA"},
    {"4170", "FERTILIZACION MANUAL GRANULADA"},
    {"4180", "FERTILIZACION MECANICA GAS AMONIO"},
    {"4200", "CONTROL DE PLAGAS"},
    {"4210", "APLICACION DE MADURANTE"},
    {"4220", "APLICACION DE INHIBIDORES"},
    {"4230", "REMEDIR(MUESTREAR Y ESTIMAR COSECHA)"},
    {"4240", "CONSERVACION DE SUELOS"},
    {"4250", "RIEGO MINI ASPERSION POSCORTE"},
    {"4260", "DESPEJE DE RONDAS"},
    {"4270", "RIEGO MINI ASPERSION PRECORTE"},
    {"4274", "TRACTORISTA B EN RIEGO MINI ASPERSION PRECORTE"},
    {"4290", "PERSONAL EN ESPERA POR REPARACION"},
    {"4291", "PERSONAL EN ESPERA POR REPARACION"},
    {"4300", "MUESTREO DE PLAGAS"},
    {"4320", "ACOMODO MECANICO DE RASTROJO VERDE"},
    {"4330", "ACOMODO MANUAL DE RASTROJO QUEMADO"},
    {"4340", "ACOMODO MECANICO DE RASTROJO QUEMADO"},
    {"4360", "2DO. C.Q. MALEZAS EQUIPO P.M."},
    {"4370", "2DO. C.Q. MALEZAS EQUIPO P.C."},
    {"4380", "1ER. C.Q. MALEZAS EQUIPO MECANICO"},
    {"4390", "2DO. C.Q. MALEZAS EQUIPO MECANICO"},
    {"4400", "C.Q. MALEZAS AEREO"},
    {"4420", "GUARDIAN EN CANAL SOCA"},
    {"4422", "Guardian de canales"},
    {"4430", "RIEGO ASPERSION PRECORTE"},
    {"4440", "FOQUEO QUIMICO MALEZAS"},
    {"4442", "TRACTORISTA B EN FOQUEO QUIMICO DE MALEZAS"},
    {"4460", "RIEGO PIVOTE POSCORTE"},
    {"4470", "RIEGO ASPERSION PRECORTE"},
    {"4471", "Traslado o instalacion de equipos riego aspersion precorte"},
    {"4474", "TRACTORISTA B EN RIEGO ASPERSION PRECORTE"},
    {"4479", "TRACTORISTA EN BRECHADO EN RIEGO ASPERSION PRECORTE"},
    {"4490", "RIEGO GRAVEDAD - GRAVEDAD PRECORTE "},
    {"4510", "RIEGO PIVOTE PRECORTE"},
    {"4520", "FERTIRIEGO EQUIPO DE AVANCE FRONTAL"},
    {"4540", "RIEGO PIVOTE RENTADO PRECORTE"},
    {"4550", "RIEGO PIVOTE RENTADO POSCORTE"},
    {"4580", "RIEGO MIDI ASPERSION PRECORTE "},
    {"4590", "RIEGO MIDI ASPERSION POSCORTE"},
    {"4600", "FERTILIZACION SUBPRODUCTOS ORGANICOS"},
    {"4610", "APLICACION DE PREMADURANTE"},
    {"4630", "RESIEMBRA MECANICA"},
    {"4632", "TRACTORISTA B RESIEMBRA MECANICA"},
    {"4640", "APLICACION DE BIOESTIMULANTES"},
    {"4650", "RIEGO AVANCE FRONTAL RENTADO PRECORTE"},
    {"4670", "RIEGO ASPERSION CANON VIAJERO PRECORTE"},
    {"4680", "RIEGO ASPERSION CANON VIAJERO POSCORTE"},
    {"4686", "JORNALERO RIEGO ASPERSION CANON VIAJERO POSCORTE"},
    {"5020", "MANTENIMIENTO OBRAS DE RIEGO"},
    {"5030", "MANTENIMIENTO Y HECHURA DE CERCAS"},
    {"5040", "MANTENIMIENTO OBRAS DE DRENAJE"},
    {"5045", "JORNALERO MANTENIMIENTO OBRAS DE DRENAJE"},
    {"5050", "MANTENIMIENTO CAMINOS Y PUENTES"},
    {"5053", "Tractorista B en mantenimiento caminos y puentes"},
    {"5060", "MANTENIMIENTO CASCO FINCA"},
    {"5090", "PERSONAL EN ESPERA POR REPARACION"},
    {"5091", "PERSONAL EN ESPERA POR REPARACION"},
    {"5100", "AGUA PARA REBOMBEO"},
    {"5140", "ESTABLECIMIENTO Y MANTO. DE AREAS FORESTALES"},
    {"5150", "GUARDIAN EN INFRAESTRUCTURA FISICA"},
    {"7010", "TRABAJOS VARIOS FCA LA BOLSA (SIN COBRO)"},
    {"7300", "CORTE"},
    {"7305", "JORNALERO"},
    {"7413", "JORNALEROS"},
    {"7901", "CONSTRUCCION PASO FCA CONCEPCION LA NORIA"},
    {"7910", "1 POZO MECANICO Y EQUIPO P. REBOMBEAR FCA COYOLATE"},
    {"7911", "PROYECTO CIUDAD CAMANTULUL"},
    {"7916", "PASO EQUIPO RENTADO FCA CONCEPCION LA NORIA A ING EL PILAR"},
    {"7920", "JORNALERO RECONST 300MTS BORDA FCA TESALIA"},
    {"7927", "FASE II DE COMPTA. INGRESO DE AGUA FCA C.L.N."},
    {"7930", "TOLVAS CANERAS P.M. 2"},
    {"7936", "APLICACION DE HERBICIDA REFORESTACION"},
    {"7939", "JORNALERO EN CORTE DE SEMILLA PARA RESIEMBRA MANUAL"},
    {"7940", "SIEMBRA REFORESTACION"},
    {"7945", "JORNALERO EN CORTE DE SEMILLA"},
    {"7950", "JORNALERO CARGA Y DESCARGA RESIEMBRA MECANICA"},
    {"7959", "ESPIGON FCA. SANTA MARTA 25-26"},
    {"7960", "FERTILIZACION REFORESTACION"},
    {"7972", "11 Canones Viajeros Fca Los Amigos"},
    {"7973", "8 CANORNES VIAJEROS FCA EL TESORO"},
    {"7976", "5 CANONES VIAJEROS FCA COYOLATE"},
    {"7984", "JORNALERO EN SIEMBRA MANUAL"},
    {"7985", "TRABAJOS FINCA SALAMANCA"},
    {"7990", "1 POZO ARTESANAL 18MTS PRODUNDIDAD FCA LAS FLORES MT"},
    {"7994", "REFORZAR Y AMPLIAR PUENTES RUTA INTERNA CDS"},
    {"9150", "TRANSPORTES"},
    {"9200", "TALLER MECANICO"},
    {"9270", "INGENIERIA AGRICOLA"},
    {"9280", "MECANICO"},
    {"9287", "JORNALERO"}

};


static const LossTime LosstimeList[] = {
    {"9001","Problema Mecanico"},
    {"9002","Problema Implemento"},
    {"9003","Problema Llantas"},
    {"9004","Problema Hidraulico"},
    {"9005","Problema Electrico"},
    {"9006","Servicio Preventivo"},
    {"9007","Reparacion en Taller"},
    {"9008","Problema Clima/Terreno"},
    {"9010","Falta de Combustible"},
    {"9011","Falta de Operador"},
    {"9012","Falta Area/Labor"},
    {"9013","Traslado Rodado o Lowboy"},
    {"9014","Sistema FIQ, Fertilizacion"},
    {"9015","Sistema Autoguiado"},
    {"9016","Sistema a GPS"},
    {"9017","Laborando"},
    {"9018","Accidente"},
    {"9019","Calibracion"},
    {"9020","Disponible"},
    {"9021","Pruebas Maquinaria"}
};


static const LaborItem  ChargeItemList[] = {
  {
     "21",
     "DEPTO. CONTROL MALEZA Y MADURANTE"
  },
  {
     "22",
     "DEPTO. MECANIZACION Y FERTILIZACION"
  },
  {
     "117",
     "MANT. Y REP. DE TOMAS AGUA FABRICA"
  },
  {
     "472",
     "PRUEBA DE TRACTORES EN TALLER (MECANIZACION)"
  },
  {
     "32",
     "FINCA CAMANTULUL"
  },
  {
     "57",
     "FINCA GARRUCHA"
  },
  {
     "80",
     "FINCA NUEVO MUNDO"
  },
  {
     "91",
     "FINCA SANTA ISABEL"
  },
  {
     "173",
     "FINCA LOS AMIGOS"
  },
  {
     "187",
     "FINCA MONTE CRISTO"
  },
  {
     "190",
     "FINCA TESALIA"
  },
  {
     "193",
     "FINCA CRISTOBAL"
  },
  {
     "263",
     "FINCA PATRICIA"
  },
  {
     "265",
     "FINCA EL TESORO"
  },
  {
     "280",
     "FINCA LAS FLORES MT"
  },
  {
     "306",
     "FINCA MADRE TIERRA 2"
  },
  {
     "33",
     "FINCA MADRE TIERRA"
  },
  {
     "34",
     "FINCA XATA"
  },
  {
     "36",
     "FINCA VICTORY"
  },
  {
     "39",
     "FINCA CAÑAVERALES DEL SUR"
  },
  {
     "40",
     "FINCA LIMONES"
  },
  {
     "41",
     "FINCA PALMIRA"
  },
  {
     "42",
     "FINCA CONCEPCION BUENA VISTA"
  },
  {
     "385",
     "FINCA SAN SALVADOR"
  },
  {
     "38",
     "FINCA EL RETIRO"
  },
  {
     "43",
     "FINCA MIXQUEÑO"
  },
  {
     "44",
     "FINCA VARIEDADES PROPIA"
  },
  {
     "81",
     "FINCA LA ESPERANZA MAPAN"
  },
  {
     "102",
     "FINCA SANTA MARTA"
  },
  {
     "112",
     "FINCA HORIZONTE"
  },
  {
     "119",
     "FINCA SAN LORENZO"
  },
  {
     "121",
     "FINCA LAS PARCELAS N.C."
  },
  {
     "161",
     "FINCA LA BENDICION"
  },
  {
     "194",
     "FINCA COYOLATE"
  },
  {
     "195",
     "FINCA SAN ROQUE"
  },
  {
     "214",
     "FINCA BARRANQUILLA"
  },
  {
     "336",
     "EL MANANTIAL"
  },
  {
     "384",
     "SAN ANTONIO MAPÁN"
  },
  {
     "441",
     "PARCELA ESPINOZA"
  },
  {
     "452",
     "FINCA EL RECREO"
  },
  {
     "46",
     "FINCA PETEN"
  },
  {
     "47",
     "FINCA CANOAS"
  },
  {
     "50",
     "FINCA PAJUIL"
  },
  {
     "96",
     "FINCA SANTA EMILIA"
  },
  {
     "109",
     "FINCA MINAR"
  },
  {
     "110",
     "FINCA NACIMIENTO"
  },
  {
     "111",
     "FINCA SAN ANTONIO"
  },
  {
     "136",
     "FINCA LAS MAJADAS"
  },
  {
     "151",
     "FINCA EL DORAL"
  },
  {
     "186",
     "FINCA SAN JOSE EL PARAISO"
  },
  {
     "241",
     "FINCA CAMELIAS"
  },
  {
     "242",
     "FINCA EL CARMEN"
  },
  {
     "284",
     "FINCA LA BOLSA TERCEROS"
  },
  {
     "303",
     "FINCA MALAGA"
  },
  {
     "529",
     "FINCA CONCEPCION LA NORIA"
  },
  {
     "557",
     "FINCA SAN MIGUEL MALAGA"
  },
  {
     "120",
     "FINCA LA ENCANTADORA"
  },
  {
     "185",
     "FINCA SAN FRANCISCO"
  },
  {
     "197",
     "FINCA LAS MARIAS"
  },
  {
     "198",
     "FINCA LAS FLORES"
  },
  {
     "199",
     "FINCA LA PRIMAVERA"
  },
  {
     "230",
     "FINCA COVADONGA CAÑA"
  },
  {
     "353",
     "FINCA  AGUACAPA"
  },
  {
     "364",
     "FINCA LAS POZAS"
  },
  {
     "390",
     "FINCA LA LUZ"
  }
};

static const int laborCount = sizeof(laborList) / sizeof(laborList[0]);
static const int lossTimecount = sizeof(LosstimeList) / sizeof(LosstimeList[0]);
static const int ChargeTimecount = sizeof(ChargeItemList) / sizeof(ChargeItemList[0]);



const char* getLaborNameById(const char* id) {
    for (int i = 0; i < laborCount ; i++) {
        if (strcmp(id, laborList[i].id) == 0) {


            return laborList[i].name;
        }
    }
    return "DESCONOCIDO";
};


const char* getChargeNameById(const char* id) {
    for (int i = 0; i <  ChargeTimecount ; i++) {
        if (strcmp(id, ChargeItemList[i].id) == 0) {


            return ChargeItemList[i].name;
        }
    }
    return "DESCONOCIDO";
};

const char* getLossTimeID(const char* id){
      static char response[80];
    for (int i = 0; i < lossTimecount ; i++) {
        if (strcmp(id, LosstimeList[i].id) == 0) {
           
            snprintf(response, sizeof(response),"%s%s", LosstimeList[i].name," - T.P");
            return response;
        }
    }
    return "DESCONOCIDO";

};



bool existsLabor(const char* id) {
    for (int i = 0; i < laborCount; i++) {
        if (strcmp(id, laborList[i].id) == 0) {
            return true;
        }
    }
    return false;
};

bool existsTP(const char* id) {
    for (int i = 0; i < lossTimecount; i++) {
        if (strcmp(id, LosstimeList[i].id) == 0) {
            return true;
        }
    }
    return false;
};



bool existChargeTP(const char* id){
    for (int i = 0; i < ChargeTimecount; i++) {
        if (strcmp(id, ChargeItemList[i].id) == 0) {
            return true;
        }
    }
    return false;

};

