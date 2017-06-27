//
// Created by VermutMac on 6/15/2017.
//

#ifndef GESTURES_GESTURES_MODBUS_H
#define GESTURES_GESTURES_MODBUS_H

#include <Arduino.h>
#define USE_HOLDING_REGISTERS_ONLY
#include <Modbus.h>

//////////////// registers of GESTURES ///////////////////
enum {
	// The first register starts at address 0
	ACTIONS,      // Always present, used for incoming actions

	// Any registered events, denoted by 'triggered_by_register' in rs485_node of Lua script, 1 and up
	SOLVE,
	LEFT,
	RIGHT,
	UP,
	DOWN,

	TOTAL_ERRORS,     // leave this one, error counter
	TOTAL_REGS_SIZE   // INTERNAL: total number of registers for function 3 and 16 share the same register array
};

extern void modbus_setup();
extern void modbus_loop();
extern void modbus_set(word event, word value);
#endif //GESTURES_GESTURES_MODBUS_H
