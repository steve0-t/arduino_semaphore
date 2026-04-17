#include <Arduino.h>

int main() {
    init();

#if defined(USBCON)
    USBDevice.attach();
#endif

    Serial.begin(115200);
    String input = Serial.readString();
    Serial.println(input);

    return 0;
}
