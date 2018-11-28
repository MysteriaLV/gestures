//
// Created by VermutMac on 8/24/2017.
//

#ifndef GESTURES_LED_MATRIX_H
#define GESTURES_LED_MATRIX_H

#include <Arduino.h>
#include "LEDMatrix.h"

extern void led_setup();
extern void led_loop();
extern void led_show_picture(int picture);

// We rotated the sensor, so we fix it in code
#define LED_UP    2 // REAL: 0
#define LED_DOWN  3 // REAL: 1
#define LED_RIGHT 1 // REAL: 2
#define LED_LEFT  0 // REAL: 3

#endif //GESTURES_LED_MATRIX_H
