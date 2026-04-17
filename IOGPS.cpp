#include "esp32-hal.h"
#include <sys/_stdint.h>
#include "SerialPorts.h"
#include "DwinToolsInterface.h"
#include "toolsFunctionsScreen.h"
#include "IONGPS.h"



FrequencyTracker frequency = {
    .lastValue = 0,
    .lastChangeTime = 0,
    .windowStart = millis(),
    .eventCount = 0,
    .frequency = 0
};




static uint8_t keypadRawBuffer[256];
static size_t keypadRawIndex = 0;
static unsigned long keypadLastByteMs = 0;
static const unsigned long RAW_BLOCK_TIMEOUT = 40;


// ===== Serial RX buffer =====
static char rxBuffer[256];
static uint16_t rxIndex = 0;



//direcciones  vp de la pantalla

// ===== PANTALLA GPS / VEHICULO =====

static const uint16_t VP_SPEED           = 0x1400; // U16 - km/h
static const uint16_t VP_RPM             = 0x1402; // U16 - RPM

static const uint16_t VP_ODOMETER        = 0x1404; // U32 - odometro
static const uint16_t VP_ENGINE_HOURS    = 0x1408; // U32 - horas motor

static const uint16_t VP_BATTERY_MAIN    = 0x140C; // U16 - mV
static const uint16_t VP_BATTERY_GPS     = 0x140E; // U16 - mV

static const uint16_t VP_FUEL_LEVEL      = 0x1410; // U16 - %





bool getIO_4B(const uint8_t* data, size_t len, uint16_t ioId, uint32_t &value) {

    uint8_t idHigh = (ioId >> 8) & 0xFF;
    uint8_t idLow  = ioId & 0xFF;

    for (size_t i = 0; i + 5 < len; i++) {

        if (data[i] == idHigh && data[i + 1] == idLow) {

            value =
                ((uint32_t)data[i + 2] << 24) |
                ((uint32_t)data[i + 3] << 16) |
                ((uint32_t)data[i + 4] << 8)  |
                ((uint32_t)data[i + 5]);

            return true;
        }
    }

    return false;
};

bool getIO_2B(const uint8_t* data, size_t len, uint16_t ioId, uint16_t &value) {

    uint8_t idHigh = (ioId >> 8) & 0xFF;
    uint8_t idLow  = ioId & 0xFF;

    for (size_t i = 0; i + 3 < len; i++) {

        if (data[i] == idHigh && data[i + 1] == idLow) {

            value =
                ((uint16_t)data[i + 2] << 8) |
                ((uint16_t)data[i + 3]);

            return true;
        }
    }

    return false;
};


bool getIO_1B_FromSection(const uint8_t* data, size_t len, size_t startOffset, uint8_t targetId, uint8_t &value) {

    if (data == NULL || startOffset >= len) {
        return false;
    }

    size_t i = startOffset;

    if (i >= len) return false;
    uint8_t count1 = data[i++];

    for (uint8_t j = 0; j < count1; j++) {
        if (i + 2 > len) return false;

        uint8_t id = data[i++];
        uint8_t val = data[i++];

        if (id == targetId) {
            value = val;
            return true;
        }
    }

    return false;
};

bool getGPSSpeed176(const uint8_t* data, size_t len, uint8_t &speed) {
    if (!data || len < 6) {
        return false;
    }

    for (size_t offset = 0; offset + 1 < len; offset++) {
        size_t i = offset;

        uint8_t count1 = data[i++];

        // filtro razonable
        if (count1 == 0 || count1 > 20) {
            continue;
        }

        // la seccion 1B necesita count1 * 2 bytes
        if (i + (count1 * 2) > len) {
            continue;
        }

        bool found = false;
        uint8_t tempSpeed = 0;

        for (uint8_t j = 0; j < count1; j++) {
            uint8_t id = data[i++];
            uint8_t value = data[i++];

            if (id == 176) {
                found = true;
                tempSpeed = value;
            }
        }

        // validacion extra:
        // despues de la seccion 1B deberia existir al menos el contador de 2B
        if (i >= len) {
            continue;
        }

        uint8_t count2 = data[i];

        // filtro basico para evitar coincidencias absurdas
        if (count2 > 20) {
            continue;
        }

        if (found) {
            speed = tempSpeed;

            DebugSerial.print("[IO176 encontrada en offset ");
            DebugSerial.print(offset);
            DebugSerial.println("]");

            return true;
        }
    }

    return false;
};

bool getGPSSpeed176_2BID(const uint8_t* data, size_t len, uint8_t &speed) {
    if (!data || len < 3) {
        return false;
    }

    for (size_t i = 0; i + 2 < len; i++) {
        // ID de 2 bytes en big-endian
        uint16_t id = ((uint16_t)data[i] << 8) | data[i + 1];
        uint8_t value = data[i + 2];

        if (id == 176) {   // 0x00B0
            speed = value;

            DebugSerial.println("----- IO176 MATCH -----");
            DebugSerial.print("Offset: ");
            DebugSerial.println(i);
            DebugSerial.print("ID: ");
            DebugSerial.println(id);
            DebugSerial.print("Speed: ");
            DebugSerial.println(speed);

            DebugSerial.print("Contexto: ");
            int start = (i >= 6) ? (int)i - 6 : 0;
            int end   = ((i + 8) < len) ? (int)i + 8 : (int)len - 1;

            for (int k = start; k <= end; k++) {
                if (data[k] < 0x10) DebugSerial.print("0");
                DebugSerial.print(data[k], HEX);
                DebugSerial.print(" ");
            }
            DebugSerial.println();
            DebugSerial.println("-----------------------");

            return true;
        }
    }

    return false;
};


float updateFrequency(FrequencyTracker &tracker, uint32_t currentValue, uint32_t windowMs = 3000) {

    uint32_t now = millis();

    if (currentValue != tracker.lastValue) {
        if ((now - tracker.lastChangeTime) > 5) {
            tracker.eventCount++;
            tracker.lastChangeTime = now;
            tracker.lastValue = currentValue;
        }
    }

    if ((now - tracker.windowStart) >= windowMs) {
        float elapsedSec = (now - tracker.windowStart) / 1000.0f;

        if (elapsedSec > 0.0f) {
            tracker.frequency = tracker.eventCount / elapsedSec;
        } else {
            tracker.frequency = 0.0f;
        }

        tracker.windowStart = now;
        tracker.eventCount = 0;
    }

    return tracker.frequency;
};

void printImportantIO(const uint8_t* data, size_t len) {


    uint32_t value4 = 0;
    uint16_t value2 = 0;
    uint8_t value1 = 0;

    // 🔥 offset fijo (ajústalo una vez y listo)
    size_t ioOffset = 28;

    

    DebugSerial.println("====== IO VALUES ======");

    // ===== 4 BYTES =====
    if (getIO_4B(data, len, 65, value4)) {
        DebugSerial.print("Virtual Odometer (65): ");
        DebugSerial.println(value4);

        writeU32(VP_ODOMETER, value4 / 100);
    }

    if (getIO_4B(data, len, 580, value4)) {
        DebugSerial.print("Din Hours (580): ");
        DebugSerial.println(value4);

        // ✔ SIN FLOAT (más seguro)
        uint32_t hoursInt = value4 / 3600;
        uint32_t rem = value4 % 3600;
        uint32_t hoursX10 = hoursInt * 10 + (rem * 10) / 3600;

        DebugSerial.print("Engine Hours x10: ");
        DebugSerial.println(hoursX10);

        writeU32(VP_ENGINE_HOURS, hoursX10);
    }

    if (getIO_4B(data, len, 150, value4)) {
        DebugSerial.print("Operator (150): ");
        DebugSerial.println(value4);
    }

    if (getIO_4B(data, len, 48, value4)) {
        DebugSerial.print("IO 48: ");
        DebugSerial.println(value4);
    }

        if (getIO_2B(data, len, 18, value2)) {
    DebugSerial.print("IO 18: ");
    DebugSerial.println(value2);

    float freq = updateFrequency(frequency, value2, 3000);

    DebugSerial.print("FREQ IO18: ");
    DebugSerial.print(freq, 2);
    DebugSerial.println(" Hz");

    }

    // ===== 2 BYTES =====
    DebugSerial.println("====== VOLTAGES ======");

    if (getIO_2B(data, len, 29, value2)) {
        DebugSerial.print("Power Supply (29): ");
        DebugSerial.print(value2);
        DebugSerial.print(" mV | ");
        DebugSerial.print(value2 / 1000.0, 3);
        DebugSerial.println(" V");

        writeU32(VP_BATTERY_MAIN, value2 / 10);
    }

    if (getIO_2B(data, len, 30, value2)) {
        DebugSerial.print("Battery (30): ");
        DebugSerial.print(value2);
        DebugSerial.print(" mV | ");
        DebugSerial.print(value2 / 1000.0, 3);
        DebugSerial.println(" V");

        writeU32(VP_BATTERY_GPS, value2 / 10);
    }

uint8_t speed176 = 0;

if (getGPSSpeed176_2BID(data, len, speed176)) {
    DebugSerial.print("Velocidad IO176: ");
    DebugSerial.println(speed176);
    writeU32(VP_SPEED,speed176 * 10);
} else {
    DebugSerial.println("IO176 no encontrado");
}

    DebugSerial.println("========================");
};




void readHardwareKeypad() {

    while (HardwareKeypad.available() > 0) {
        uint8_t b = HardwareKeypad.read();

        if (keypadRawIndex < sizeof(keypadRawBuffer)) {
            keypadRawBuffer[keypadRawIndex++] = b;
        } else {
            DebugSerial.println("[WARN] Buffer lleno");
            keypadRawIndex = 0;
        }

        keypadLastByteMs = millis();
    }

    if (keypadRawIndex > 0 && (millis() - keypadLastByteMs) > RAW_BLOCK_TIMEOUT) {
        DebugSerial.println("---- RAW BLOCK ----");

        for (size_t i = 0; i < keypadRawIndex; i++) {
            if (keypadRawBuffer[i] < 0x10) DebugSerial.print("0");
            DebugSerial.print(keypadRawBuffer[i], HEX);
            DebugSerial.print(" ");

            if ((i + 1) % 16 == 0) {
                DebugSerial.println();
            }
        }

        if (keypadRawIndex % 16 != 0) {
            DebugSerial.println();
        }

        DebugSerial.print("ASCII: ");
        for (size_t i = 0; i < keypadRawIndex; i++) {
            char c = (char)keypadRawBuffer[i];
            if (c >= 32 && c <= 126) {
                DebugSerial.print(c);
            } else {
                DebugSerial.print('.');
            }
        }
        DebugSerial.println();
DebugSerial.println("-------------------");

// 🔥 aquí agregas la lectura de IO
printImportantIO(keypadRawBuffer, keypadRawIndex);

keypadRawIndex = 0;
    }
};
