#include "Arduino.h"
#include "HardwareSerial.h"
#include "pins_arduino.h"
#include <avr/interrupt.h>
#include "../lib/Rstr/Rstr.h"
#include "../lib/XMLParser/XMLParser.h"
#include "../lib/Pins/Pins.h"

#define RESPONSE_BUFFER_SIZE 128

void sem_off();
void stop_sem();
void ready_sem();
void go_sem();
i8   set_state(char* response_buffer, const State state, const char* state_str);
void provide_err_msg(Retval id, char* response_buffer);
void print_pong(char* response_buffer);

int  main() {
    init();

#if defined(USBCON)
    USBDevice.attach();
#endif

    Serial.begin(115200);

    XMLParser parser = XMLParser();
    rstr      input  = {._str = (char*)malloc(256 * sizeof(char)), .len = 0};
    Retval    res;
    i8        caution_sem                           = 0;
    char      response_buffer[RESPONSE_BUFFER_SIZE] = {0};

    // blinking led stuff
    i8        caution_led_state = 0;
    u32       previous_milis = 0, current_millis = 0;
    const u32 blink_interval = 1000;

    for (;;) {
        input.clear_str();
        parser.reset();

        if (caution_sem) {
            current_millis = millis();
            if (current_millis - previous_milis >= blink_interval) {
                previous_milis    = current_millis;
                caution_led_state = (caution_led_state == 0) ? 1 : 0;
                digitalWrite(YELLOW, caution_led_state);
            }
        }

        if (Serial.available()) {
            input.len = (u64)Serial.readBytesUntil('\n', input._str, 255);
            // Serial.println(input._str);
            if ((res = parser.parse(input)) < VALID) {
                // Serial.println("invalid!");
                if (res > 0 && res < VALID)
                    provide_err_msg(res, response_buffer);
            } else {
                if (res == COMMENT)
                    Serial.println("Comment");

                if (parser.getCmpType() == SET) {
                    caution_sem = set_state(response_buffer, parser.getState(),
                                            parser.getFields().state);
                } else if (parser.getCmpType() == PING) {
                    print_pong(response_buffer);
                } else if (parser.getCmpType() == GET) {
                    memset(response_buffer, 0, RESPONSE_BUFFER_SIZE);
                    snprintf(response_buffer, RESPONSE_BUFFER_SIZE,
                             "<rsp status=\"ok\" state=\"%s\"/>",
                             parser.getFields().state);
                    Serial.println(response_buffer);
                }
            }
        }
    }

    free(input._str);
    return 0;
}

void sem_off() {
    digitalWrite(RED, LOW);
    digitalWrite(YELLOW, LOW);
    digitalWrite(GREEN, LOW);
}

void stop_sem() {
    sem_off();
    digitalWrite(RED, HIGH);
}

void ready_sem() {
    sem_off();
    digitalWrite(RED, HIGH);
    digitalWrite(YELLOW, HIGH);
}
void go_sem() {
    sem_off();
    digitalWrite(GREEN, HIGH);
}

i8 set_state(char* response_buffer, const State state, const char* state_str) {
    i8 ret = 0;
    switch (state) {
        case STOP: stop_sem(); break;
        case READY: ready_sem(); break;
        case GO: go_sem(); break;
        case OFF: sem_off(); break;
        case CAUTION: ret = 1; break;
        default: break;
    }

    memset(response_buffer, 0, RESPONSE_BUFFER_SIZE);
    snprintf(response_buffer, RESPONSE_BUFFER_SIZE,
             "<rsp status=\"ok\" state=\"%s\"/>", state_str);
    Serial.println(response_buffer);
    return ret;
}

void provide_err_msg(Retval id, char* response_buffer) {
    memset(response_buffer, 0, RESPONSE_BUFFER_SIZE);
    snprintf(response_buffer, RESPONSE_BUFFER_SIZE,
             "<rsp status=\"error\" code=\"%s\"/>", responses[id - 1]);
    Serial.println(response_buffer);
}

void print_pong(char* response_buffer) {
    memset(response_buffer, 0, RESPONSE_BUFFER_SIZE);
    snprintf(response_buffer, RESPONSE_BUFFER_SIZE,
             "<rsp status=\"ok\" msg=\"PONG\"/>");
    Serial.println(response_buffer);
}
