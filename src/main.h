#ifndef MAIN_H
#define MAIN_H

#ifdef ARDUINO
#include "Arduino.h"
#include "HardwareSerial.h"
#include <avr/interrupt.h>
#else
#include <iostream>
#include <cstring>
#include <cstdint>
#include <cstdio>
#endif

#include "../lib/Pins/Pins.h"
#include "../lib/Rstr/Rstr.h"
#include "../lib/XMLParser/XMLParser.h"
#include "../lib/Pins/Pins.h"

#define RESPONSE_BUFFER_SIZE 128
#define ENDL                 '\n'

void                 sem_off();
void                 stop_sem();
void                 ready_sem();
void                 go_sem();
int8_t               set_state(char* response_buffer, const State state,
                               const char* state_str);
void                 provide_err_msg(Retval id, char* response_buffer);
void                 print_pong(char* response_buffer);
[[nodiscard]] int8_t get_user_input(rstr& buffer);

#endif
