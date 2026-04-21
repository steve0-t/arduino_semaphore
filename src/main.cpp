#include "main.h"
#include <cstdint>

#define CAPACITY 512

int main() {
#ifdef ARDUINO
    init();

#if defined(USBCON)
    USBDevice.attach();
#endif

    Serial.begin(115200);
#endif

    XMLParser   parser = XMLParser(commands, states);

    static char input_buf[INPUT_MAX_LEN] = {0};
    rstr        wrapper                  = {.data = input_buf, .len = 0};

    int8_t      caution_sem                           = 0;
    static char response_buffer[RESPONSE_BUFFER_SIZE] = {0};

    State       current_state = SCOUNT;
    State       state         = SCOUNT;
    Command     cmd_type      = CCOUNT;

    // blinking led stuff
    int8_t         caution_led_state = 0;
    uint32_t       previous_milis = 0, current_millis = 0;
    const uint32_t blink_interval = 1000;

    const char *   attr1 = nullptr, *attr2 = nullptr;
    for (;;) {
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

        wrapper.clear_str();
        if (get_user_input(wrapper) == 0)
            continue;

        if (strcmp(wrapper.data, "quit") == 0)
            break;

        if (!parser.set_buffer(wrapper)) {
            provide_err_msg(BAD_FORMAT, response_buffer);
            continue;
        }

        attr1 = parser.get_attribute("type");
        if (attr1 == nullptr) {
            provide_err_msg(BAD_FORMAT, response_buffer);
            continue;
        }

        cmd_type = parse_value(attr1);
        if (cmd_type == CCOUNT) {
            provide_err_msg(UNKNOWN_CMD, response_buffer);
            continue;
        }

        switch (cmd_type) {
            case SET:
                attr2 = parser.get_attribute("state");
                if (attr2 == nullptr) {
                    provide_err_msg(BAD_FORMAT, response_buffer);
                    continue;
                }

                state = parse_state(attr2);
                if (state == SCOUNT) {
                    provide_err_msg(BAD_STATE, response_buffer);
                    continue;
                }

                caution_sem = set_state(response_buffer, state);
                if (caution_sem < 0) {
                    log("TRIED TO SET INVALID STATE");
                    continue;
                }
                current_state = state;
                break;
            case PING: print_pong(response_buffer); break;
            case GET: provide_curr_state(current_state, response_buffer);
            default: break;
        }
        // std::cout << NLN;
    }
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

int8_t set_state(char* response_buffer, const State state) {
    int8_t ret = 0;
    switch (state) {
        case STOP: stop_sem(); break;
        case READY: ready_sem(); break;
        case GO: go_sem(); break;
        case OFF: sem_off(); break;
        case CAUTION: ret = 1; break;
        default: return -1;
    }

    memset(response_buffer, 0, RESPONSE_BUFFER_SIZE);
    snprintf(response_buffer, RESPONSE_BUFFER_SIZE,
             "<rsp status=\"ok\" state=\"%s\"/>", states[state]);
    log(response_buffer);
    return ret;
}

void provide_err_msg(Retval id, char* response_buffer) {
    memset(response_buffer, 0, RESPONSE_BUFFER_SIZE);
    snprintf(response_buffer, RESPONSE_BUFFER_SIZE,
             "<rsp status=\"error\" code=\"%s\"/>", responses[id]);
    log(response_buffer);
}

void print_pong(char* response_buffer) {
    memset(response_buffer, 0, RESPONSE_BUFFER_SIZE);
    snprintf(response_buffer, RESPONSE_BUFFER_SIZE,
             "<rsp status=\"ok\" msg=\"PONG\"/>");
    log(response_buffer);
}

void provide_curr_state(State curr_state, char* response_buffer) {
    if (curr_state == SCOUNT)
        return;
    memset(response_buffer, 0, RESPONSE_BUFFER_SIZE);
    snprintf(response_buffer, RESPONSE_BUFFER_SIZE,
             "<rsp status=\"ok\" state=\"%s\"/>", states[curr_state]);
    log(response_buffer);
}

int8_t get_user_input(rstr& buffer) {
#ifdef ARDUINO
    if (!Serial.available())
        return 0;
    buffer.len =
        (uint16_t)Serial.readBytesUntil('\n', buffer.data, INPUT_MAX_LEN);
#else
    if (fgets(buffer.data, INPUT_MAX_LEN, stdin)) {
        buffer.len = strlen(buffer.data);
        if (buffer.len > 0 && buffer.data[buffer.len - 1] == ENDL) {
            buffer.data[buffer.len - 1] = 0;
            buffer.len--;
        }
    } else
        return 0;
#endif
    return 1;
}

Command parse_value(const char* ptr) {
    // std::cout << "PARSE VALUE" << NLN;
    // std::cout << idx << NLN;
    // std::cout << buf.str_view(idx) << NLN;
    if (ptr != nullptr) {
        for (int8_t i = 0; i < CCOUNT; i++) {
            uint8_t len = (uint8_t)strlen(commands[i]);
            if (strncmp(ptr, commands[i], len) == 0)
                return (Command)i;
        }
    }
    return CCOUNT;
}

State parse_state(const char* ptr) {
    if (ptr != nullptr) {
        for (int8_t i = 0; i < SCOUNT; i++) {
            uint8_t len = (uint8_t)strlen(states[i]);
            if (strncmp(ptr, states[i], len) == 0)
                return (State)i;
        }
    }
    return SCOUNT;
}
