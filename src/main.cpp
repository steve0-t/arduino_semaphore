#include "HardwareSerial.h"
#include <Arduino.h>
#include <avr/interrupt.h>

int main() {
    init();

#if defined(USBCON)
    USBDevice.attach();
#endif

    Serial.begin(115200);

    for (;;) {
        if (Serial.available()) {
            String input = Serial.readStringUntil('\n');
            Serial.println(input);
            if (input.equals("hello")) {
                Serial.println("secret discovered");
            }
        }
    }

    return 0;
}
