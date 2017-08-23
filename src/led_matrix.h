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

#define LED_UP 0
#define LED_DOWN 1
#define LED_RIGHT 2
#define LED_LEFT 3

#endif //GESTURES_LED_MATRIX_H
