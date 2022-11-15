#ifdef PLATFORMIO
#include <Arduino.h>
#endif

#include <string.h>

constexpr auto g_kBufferSize = 128u;
constexpr auto g_kUnitPacketBufferSize = 10u;

char g_arrRecvBuffer[g_kBufferSize];
char g_arrUnitPacketBuffer[g_kUnitPacketBufferSize];

const char *g_kCMD_00 = "#CMDLED0!";
const char *g_kCMD_01 = "#CMDLED1!";
const char *g_ackCMD_00 = "#CMDACKLED0!";
const char *g_ackCMD_01 = "#CMDACKLED1!";

uint8_t idx;

void setup() {
    memset(&g_arrRecvBuffer[0], '\0', g_kBufferSize);
    memset(&g_arrUnitPacketBuffer[0], '\0', g_kUnitPacketBufferSize);

    Serial.begin(115200);
    Serial.setTimeout(50);
    pinMode(LED_BUILTIN, OUTPUT);
    digitalWrite(LED_BUILTIN, LOW);
}

void loop() {
    //!< Receiving
    if (Serial.available()) {
        Serial.readBytes(&g_arrRecvBuffer[0], g_kBufferSize);
    }

    if (g_arrRecvBuffer[0] != '\0') {
        //!< get data from buffer
        for (idx = 0u; idx < g_kBufferSize; ++idx) {
            if ((g_arrRecvBuffer[idx] == '\0') or (idx >= g_kUnitPacketBufferSize)) {
                break;
            }
            else {
                g_arrUnitPacketBuffer[idx] = g_arrRecvBuffer[idx];
            }
        }

        if ( strncmp(&g_arrUnitPacketBuffer[0], g_kCMD_00, strlen(g_kCMD_00)) == 0 ) {
            digitalWrite(LED_BUILTIN, LOW);
            Serial.println(g_ackCMD_00);
            memset(&g_arrUnitPacketBuffer[0], '\0', g_kUnitPacketBufferSize);
        }
        else if ( strncmp(&g_arrUnitPacketBuffer[0], g_kCMD_01, strlen(g_kCMD_01)) == 0 ) {
            digitalWrite(LED_BUILTIN, HIGH);
            Serial.println(g_ackCMD_01);
            memset(&g_arrUnitPacketBuffer[0], '\0', g_kUnitPacketBufferSize);
        }
    }
}