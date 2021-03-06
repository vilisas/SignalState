/*
 * SignalState.h
 *
 *  Created on: 2019 m. sausio 08 d.
 *      Author: Vilius Bilinkevicius
 */

#ifndef SIGNALSTATE_H_
#define SIGNALSTATE_H_

#include <Arduino.h>

// Digital filter
#define SIGNALSTATE_READ_SAMPLES 20
//#define SIGNALSTATE_READ_DELAY 1
#define SIGNALSTATE_MIN_SAMPLES  15

// read states if time since last read is greater than milliseconds below
#define TIMERELAY_CHECK_INTERVAL 100

class SignalState{

	private:
	uint8_t _input_pin;
	uint8_t _fix_time	= 0;
	unsigned long _loop_call_time = 0;
	unsigned long _fix_start_time = 0;


    bool _state_real	= false;
    bool _state_fixed	= false;
    bool _ack_required	= false;
    bool _inverted		= false;
    bool _blocked		= false;

    bool _counter_started=false;

	int8_t readRealState();
	void processRealState();
	void processFixedState();
	void (*onStateChanged)(bool fix_state) = nullptr;


    public:
	SignalState(uint8_t input);
	virtual ~SignalState(){}
	inline void setup(){};
	void loop();

	inline bool isBlocked() const {
		return _blocked;
	}

	// Don't change fixed state if _blocked is set to true
	inline void setBlockedState(bool blocked = false) {
		_blocked = blocked;
	}

	inline uint8_t getFixTime() const {
		return _fix_time;
	}

	// Time in seconds
	inline void setFixTime(uint8_t fixTime = 0) {
		_fix_time = fixTime;
	}

	inline bool isInverted() const {
		return _inverted;
	}

	// Invert input value
	inline void setInverted(bool inverted) {
		_inverted = inverted;
	}

	inline bool isAckRequired() const {
		return _ack_required;
	}

	// fixed state must be acknowledged before it can change again
	inline void setAck() {
		_ack_required = false;
	}
	// true = fixed, false = not fixed
	inline bool isFixed() const {
		return _state_fixed;
	}
	// returns last read result
	inline bool getRealState() const {
		return _state_real;
	}

	inline uint8_t getInputPin() const {
		return _input_pin;
	}

	inline void setInputPin(uint8_t tsPin) {
		_input_pin = tsPin;
	}

	// calls this if fixed state changes
	inline void setStateChangeCallback(
			void (* statusChangeCallback)(bool)) {
		this->onStateChanged = statusChangeCallback;
	}

};

#endif
