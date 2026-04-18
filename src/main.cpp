#include "HardwareSerial.h"
#include "pins_arduino.h"
#include <Arduino.h>
#include <avr/interrupt.h>
#include "../lib/Rstr/Rstr.h"
#include "../lib/XMLParser/XMLParser.h"

int main() {
    init();

#if defined(USBCON)
    USBDevice.attach();
#endif

    Serial.begin(115200);

    XMLParser parser = XMLParser();
    rstr input = {._str = (char*)malloc(256 * sizeof(char)),
                  .len  = 0};

    for (;;) {
        if (Serial.available()) {
            input.len = (u64)Serial.readBytesUntil(
                '\n', input._str, 256);
            Serial.println(input._str);
            if (parser.isXML(input)) {
                Serial.println("is xml");
                digitalWrite(A0, HIGH);
                delay(2000);
                digitalWrite(A0, LOW);
            }
            input.clear_str();
            // if (input.equals("hello")) {
            //     Serial.println("secret discovered");
            //     digitalWrite(A0, HIGH);
            //     delay(2000);
            //     digitalWrite(A0, LOW);
            // } else {
            //     digitalWrite(A1, HIGH);
            //     delay(2000);
            //     digitalWrite(A1, LOW);
            // }
        }
    }

    return 0;
}
