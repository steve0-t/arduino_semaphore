#ifndef PINS_H
#define PINS_H

#ifdef ARDUINO
#include <Arduino.h>

#define RED    A1
#define YELLOW A2
#define GREEN  A3
#else

#define RED    1
#define YELLOW 2
#define GREEN  3
#endif

#endif