/*
 * SignalState.cpp
 *
 *  Created on: 2019 m. sausio 08 d.
 *      Author: Vilius Bilinkevicius
 *
 */

#include <SignalState.h>

/*
 * Default constructor, takes arduino input PIN as parameter
 */
SignalState::SignalState(uint8_t input) {
	_input_pin		= input;
}

void SignalState::loop() {

	unsigned long ms = millis();

	//	call only if time from last call is > 100 ms
	if (ms - _loop_call_time >= SIGNALSTATE_READ_INTERVAL){

		processRealState();
		processFixedState();
		_loop_call_time = ms;
	}

}


void SignalState::processFixedState(){
	if (_blocked){
		return;
	}
	bool state_changed = false;
	long time = millis();
	if (!_state_fixed && !_ack_required) {
		if (_state_real) {
			if (!_counter_started) { //
				_counter_started = true;
				_fix_start_time = time;
				return;
			}
		} else {
			_counter_started = false;
		}

		if ( _counter_started && ((time - _fix_start_time) > _fix_time * 1000)){
			_state_fixed = true;
			state_changed = true;
		}
	} else {
		if (!_state_real && !_ack_required){
				state_changed = true;
//			}
			_state_fixed = false;
			_fix_start_time = time;
		}
	}

	if (state_changed){
		_ack_required = true;
		if (onStateChanged != nullptr) onStateChanged(_state_fixed);
	}
}


/*
 * Read and store real state, invert if required
 */
void SignalState::processRealState(){
	int8_t state = readRealState();

	// if state == -1, this means it neither 1 or 0, so leave value untouched
	if (state >=0){
		_state_real = (state == 1);
		if (_inverted) {
			_state_real = !_state_real;
		}
	}
}

/*
 * Reads real state.
 * Retunrs
 *  1 if logical 1
 *  0 if logical 0
 * -1 if simple digital filtering fails
 *
 * Actually inverted value here. digitalRead(A1) with pull up is 1, and shorted to ground is 0
 * but what we want is logical state.
 * this can be changed back with setInverted(true)
 *
 * if _input_pin is not set, always return 0
 *
 */
int8_t SignalState::readRealState(){
	if (_input_pin == 0) {
		return 0;
	}
  uint8_t one = 0;
  uint8_t zero = 0;
  uint8_t state;

  for (uint8_t i = 0; i< SIGNALSTATE_READ_SAMPLES; i++) {
    state = digitalRead(_input_pin);
    if (state == 0) one++;
    else if (state == 1) zero++;
#ifdef SIGNALSTATE_READ_DELAY
    delay(SIGNALSTATE_READ_DELAY);
#endif
  }
    if (one >= SIGNALSTATE_MIN_SAMPLES) return(1);
    else if (zero >= SIGNALSTATE_MIN_SAMPLES) return(0);

    return(-1);

}
