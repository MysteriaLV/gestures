#include <Arduino.h>

#include "paj7620.h"
#include "gestures_modbus.h"
#include "led_matrix.h"
#include "Automaton.h"

/*
Serial1: RX0, TX1
TWI: 2 (SDA) and 3 (SCL). Support TWI communication using the Wire library.
*/

#define GES_REACTION_TIME         100                // You can adjust the reaction time according to the actual circumstance.
Atm_timer led_feedback_timer;
Atm_bit led_feedback_is_showing;
Atm_led magnet;

uint8_t gesture_data = 0, gesture_error;

void setup() {
    Serial.begin(115200);
    modbus_setup();
    led_setup();

    led_feedback_is_showing
            .begin()
            .onChange(true, led_feedback_timer, Atm_timer::EVT_START);
    led_feedback_timer
            .begin(1500)
            .onFinish(led_feedback_is_showing, Atm_bit::EVT_OFF);

    magnet
            .begin(PIN_A0)
            .on();

    gesture_error = paj7620Init();            // initialize Paj7620 registers
    if (gesture_error) {
        Serial.print("PAJ7620U2 INIT ERROR, CODE: ");
        Serial.println(gesture_error);
    } else {
        Serial.println("PAJ7620U2 INIT OK");
    }
}

void do_modbus(unsigned int delay_time = 0) {
    long start = millis();
    do {
        modbus_loop();
    } while (millis() - start < delay_time);
}

int riddle_state = 0;

void riddle_controller(word event) {
    if (riddle_state == 0 && event == LEFT) riddle_state++;
    else if (riddle_state == 1 && event == UP) riddle_state++;
    else if (riddle_state == 2 && event == UP) riddle_state++;
    else if (riddle_state == 3 && event == DOWN) completePuzzle();
    else if (event == LEFT) riddle_state = 1;
    else
        riddle_state = 0;

    Serial.print("Riddle state: ");
    Serial.println(riddle_state);
}

void completePuzzle() {
    modbus_set(COMPLETE, 1);
    Serial.println("Complete");
    magnet.off();
}

void resetPuzzle() {
    modbus_set(COMPLETE, 0);
    modbus_set(LEFT, 0);
    modbus_set(RIGHT, 0);
    modbus_set(UP, 0);
    modbus_set(DOWN, 0);
    magnet.on();
}

void gesture_loop() {
    gesture_error = paj7620ReadReg(0x43, 1, &gesture_data);   // Read Bank_0_Reg_0x43/0x44 for gesture result.

    if (!gesture_error) {
        switch (gesture_data)                                 // When different gestures be detected, the variable 'gesture_data' will be set to different values by paj7620ReadReg(0x43, 1, &gesture_data).
        {
            case GES_RIGHT_FLAG:
                modbus_set(RIGHT, 1);
                modbus_set(LEFT, 0);
                modbus_set(UP, 0);
                modbus_set(DOWN, 0);
                riddle_controller(RIGHT);

                Serial.println("Right");
                led_show_picture(LED_RIGHT);
                led_feedback_is_showing.on();

                break;
            case GES_LEFT_FLAG:
                modbus_set(RIGHT, 0);
                modbus_set(LEFT, 1);
                modbus_set(UP, 0);
                modbus_set(DOWN, 0);
                riddle_controller(LEFT);

                Serial.println("Left");
                led_show_picture(LED_LEFT);
                led_feedback_is_showing.on();
                break;
            case GES_UP_FLAG:
                modbus_set(RIGHT, 0);
                modbus_set(LEFT, 0);
                modbus_set(UP, 1);
                modbus_set(DOWN, 0);
                riddle_controller(UP);

                Serial.println("Up");
                led_show_picture(LED_UP);
                led_feedback_is_showing.on();
                break;
            case GES_DOWN_FLAG:
                modbus_set(RIGHT, 0);
                modbus_set(LEFT, 0);
                modbus_set(UP, 0);
                modbus_set(DOWN, 1);
                riddle_controller(DOWN);

                Serial.println("Down");
                led_show_picture(LED_DOWN);
                led_feedback_is_showing.on();
                break;
            default:
                do_modbus(GES_REACTION_TIME);
                break;
        }
    }
}

void loop() {
    do_modbus();
    automaton.run();

    if (led_feedback_is_showing.state() == Atm_bit::ON)
        led_loop();
    else
        gesture_loop();
}

