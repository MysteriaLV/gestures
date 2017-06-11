#include <Arduino.h>

#include "softi2c_paj7620.h"
#define GES_REACTION_TIME         500                // You can adjust the reaction time according to the actual circumstance.
#define GES_ENTRY_TIME            800                // When you want to recognize the Forward/Backward gestures, your gestures' reaction time must less than GES_ENTRY_TIME(0.8s).
#define GES_QUIT_TIME            1000

extern void modbus_setup();
extern void modbus_loop();
extern unsigned int holdingRegs[];

void setup() {
	Serial.begin(115200);
	modbus_setup();

	uint8_t error = 0;

	Serial.println("\nPAJ7620U2 TEST DEMO: Recognize 9 gestures.");

	error = paj7620Init();            // initialize Paj7620 registers
	if (error) {
		Serial.print("INIT ERROR,CODE:");
		Serial.println(error);
	} else {
		Serial.println("INIT OK");
	}
	Serial.println("Please input your gestures:\n");
}

void do_modbus(unsigned int delay_time = 0) {
	long start = millis();
	do {
		modbus_loop();
	} while (millis() - start < delay_time);
}

void gesture_loop() {
	uint8_t data = 0, data1 = 0, error;
	error = paj7620ReadReg(0x43, 1, &data);                // Read Bank_0_Reg_0x43/0x44 for gesture result.

	if (!error) {
		switch (data)                                    // When different gestures be detected, the variable 'data' will be set to different values by paj7620ReadReg(0x43, 1, &data).
		{
			case GES_RIGHT_FLAG:
				do_modbus(GES_ENTRY_TIME);
				paj7620ReadReg(0x43, 1, &data);
				if (data == GES_FORWARD_FLAG) {
					Serial.println("Forward");
					do_modbus(GES_QUIT_TIME);
				} else if (data == GES_BACKWARD_FLAG) {
					Serial.println("Backward");
					do_modbus(GES_QUIT_TIME);
				} else {
					Serial.println("Right");
				}
				break;
			case GES_LEFT_FLAG:
				do_modbus(GES_ENTRY_TIME);
				paj7620ReadReg(0x43, 1, &data);
				if (data == GES_FORWARD_FLAG) {
					Serial.println("Forward");
					do_modbus(GES_QUIT_TIME);
				} else if (data == GES_BACKWARD_FLAG) {
					Serial.println("Backward");
					do_modbus(GES_QUIT_TIME);
				} else {
					Serial.println("Left");
				}
				break;
			case GES_UP_FLAG:
				do_modbus(GES_ENTRY_TIME);
				paj7620ReadReg(0x43, 1, &data);
				if (data == GES_FORWARD_FLAG) {
					Serial.println("Forward");
					do_modbus(GES_QUIT_TIME);
				} else if (data == GES_BACKWARD_FLAG) {
					Serial.println("Backward");
					do_modbus(GES_QUIT_TIME);
				} else {
					Serial.println("Up");
				}
				break;
			case GES_DOWN_FLAG:
				do_modbus(GES_ENTRY_TIME);
				paj7620ReadReg(0x43, 1, &data);
				if (data == GES_FORWARD_FLAG) {
					Serial.println("Forward");
					do_modbus(GES_QUIT_TIME);
				} else if (data == GES_BACKWARD_FLAG) {
					Serial.println("Backward");
					do_modbus(GES_QUIT_TIME);
				} else {
					Serial.println("Down");
				}
				break;
			case GES_FORWARD_FLAG:
				Serial.println("Forward");
				do_modbus(GES_QUIT_TIME);
				break;
			case GES_BACKWARD_FLAG:
				Serial.println("Backward");
				do_modbus(GES_QUIT_TIME);
				break;
			case GES_CLOCKWISE_FLAG:
				Serial.println("Clockwise");
				break;
			case GES_COUNT_CLOCKWISE_FLAG:
				Serial.println("anti-clockwise");
				break;
			default:
				paj7620ReadReg(0x44, 1, &data1);
				if (data1 == GES_WAVE_FLAG) {
					Serial.println("wave");
				}
				break;
		}
	}
}

void loop() {
	do_modbus();
	gesture_loop();
}