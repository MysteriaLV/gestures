//#include <Arduino.h>

#include "softi2c_paj7620.h"
#include "gestures_modbus.h"

#define GES_REACTION_TIME         100                // You can adjust the reaction time according to the actual circumstance.

uint8_t gesture_data = 0, gesture_error;

void aa_setup() {
	Serial.begin(115200);
	modbus_setup();

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
				Serial.println("Right");
				break;
			case GES_LEFT_FLAG:
				modbus_set(RIGHT, 0);
				modbus_set(LEFT, 1);
				modbus_set(UP, 0);
				modbus_set(DOWN, 0);
				Serial.println("Left");
				break;
			case GES_UP_FLAG:
				modbus_set(RIGHT, 0);
				modbus_set(LEFT, 0);
				modbus_set(UP, 1);
				modbus_set(DOWN, 0);
				Serial.println("Up");
				break;
			case GES_DOWN_FLAG:
				modbus_set(RIGHT, 0);
				modbus_set(LEFT, 0);
				modbus_set(UP, 0);
				modbus_set(DOWN, 1);
				Serial.println("Down");
				break;
			default:
				do_modbus(GES_REACTION_TIME);
				break;
		}
	}
}

void aa_loop() {
	do_modbus();
	gesture_loop();
}