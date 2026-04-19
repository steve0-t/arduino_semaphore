#include "main.h"

#define CAPACITY 512

#ifdef ARDUINO
#define custom_println(str)                                                    \
    do {                                                                       \
        Serial.println((str));                                                 \
    } while (0)
#else
#define custom_println(str)                                                    \
    do {                                                                       \
        std::cout << (str) << ENDL;                                            \
    } while (0)
#endif

int main() {
#ifdef ARDUINO
    init();

#if defined(USBCON)
    USBDevice.attach();
#endif

    Serial.begin(115200);
#endif

    XMLParser parser = XMLParser();
    rstr   input = {.data = (char*)malloc(CAPACITY * sizeof(char)), .len = 0};
    Retval res;
    int8_t caution_sem                           = 0;
    char   response_buffer[RESPONSE_BUFFER_SIZE] = {0};

    // blinking led stuff
    int8_t         caution_led_state = 0;
    uint32_t       previous_milis = 0, current_millis = 0;
    const uint32_t blink_interval = 1000;

    State          current_state = (State)-1;

    for (;;) {
        input.clear_str();
        parser.reset();

#ifdef ARDUINO
        if (caution_sem) {
            current_millis = millis();
            if (current_millis - previous_milis >= blink_interval) {
                previous_milis    = current_millis;
                caution_led_state = (caution_led_state == 0) ? 1 : 0;
                digitalWrite(YELLOW, caution_led_state);
            }
        }
#endif

#ifdef ARDUINO
        if (!Serial.available())
            continue;
        input.len =
            (uint16_t)Serial.readBytesUntil('\n', input.data, CAPACITY - 1);
#else
        if (fgets(input.data, CAPACITY - 1, stdin)) {
            input.len = strlen(input.data);
            if (input.len > 0 && input.data[input.len - 1] == ENDL) {
                input.data[input.len - 1] = 0;
                input.len--;
            }
        }
#endif

        // Serial.println(input._str);
        if ((res = parser.parse(input)) < VALID) {
            // Serial.println("invalid!");
            if (res > 0 && res < VALID)
                provide_err_msg(res, response_buffer);
        } else {
            if (res == COMMENT)
                custom_println("Comment");

            if (parser.getCmpType() == SET) {
                sem_off();
                caution_sem   = set_state(response_buffer, parser.getState(),
                                          parser.getFields().state);
                current_state = parser.getState();
            } else if (parser.getCmpType() == PING) {
                print_pong(response_buffer);
            } else if (parser.getCmpType() == GET) {
                if (current_state >= 0 && current_state < SCOUNT) {
                    memset(response_buffer, 0, RESPONSE_BUFFER_SIZE);
                    snprintf(response_buffer, RESPONSE_BUFFER_SIZE,
                             "<rsp status=\"ok\" state=\"%s\"/>",
                             states[current_state]);
                    custom_println(response_buffer);
                }
            }
        }
    }

    free(input.data);
    return 0;
}

void sem_off() {
#ifdef ARDUINO
    digitalWrite(RED, LOW);
    digitalWrite(YELLOW, LOW);
    digitalWrite(GREEN, LOW);
#endif
}

void stop_sem() {
#ifdef ARDUINO
    digitalWrite(RED, HIGH);
#endif
}

void ready_sem() {
#ifdef ARDUINO
    digitalWrite(RED, HIGH);
    digitalWrite(YELLOW, HIGH);
#endif
}

void go_sem() {
#ifdef ARDUINO
    digitalWrite(GREEN, HIGH);
#endif
}

int8_t set_state(char* response_buffer, const State state,
                 const char* state_str) {
    int8_t ret = 0;
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
    custom_println(response_buffer);
    return ret;
}

void provide_err_msg(Retval id, char* response_buffer) {
    memset(response_buffer, 0, RESPONSE_BUFFER_SIZE);
    snprintf(response_buffer, RESPONSE_BUFFER_SIZE,
             "<rsp status=\"error\" code=\"%s\"/>", responses[id - 1]);
    custom_println(response_buffer);
}

void print_pong(char* response_buffer) {
    memset(response_buffer, 0, RESPONSE_BUFFER_SIZE);
    snprintf(response_buffer, RESPONSE_BUFFER_SIZE,
             "<rsp status=\"ok\" msg=\"PONG\"/>");
    custom_println(response_buffer);
}
