#include "Arduino.h"
#include <SignalState.h>

#define LED (char) 13

#define OFF false
#define ON  true

int inputPin = A1;

SignalState ts1(inputPin);

bool realState	   = false;
bool lastRealState = false;

void setLed(bool state){
	if (state) {
		digitalWrite(LED, HIGH);
	} else{
		digitalWrite(LED, LOW);
	}
}

void callback(bool state){
	Serial.print("Fixed state changed to ");
	Serial.println(state);
	setLed(state);

}

void setup()
{

	pinMode(inputPin, INPUT_PULLUP);
	pinMode(LED, OUTPUT);
	digitalWrite(LED, LOW);

	ts1.setStateChangeCallback(&callback);
	ts1.setFixTime(2); // fix after 2 sec.
	ts1.setAck();
//	ts1.setInverted(true);
//	ts1.setBlockedState(true);

	Serial.begin(115200);

	Serial.println(F("SignalState demo"));
	Serial.println(F("Input signal is A1"));
	Serial.print(F("Fix time = "));
	Serial.println(ts1.getFixTime());
}

// The loop function is called in an endless loop
void loop()
{
	ts1.loop();

	lastRealState = realState;
	realState=ts1.getRealState();

	if (realState != lastRealState) {
		Serial.print("Real state changed from ");
		Serial.print(lastRealState);
		Serial.print(" to ");
		Serial.println(realState);
	}

	if (ts1.isAckRequired()) {
		Serial.print("ACK required");
		ts1.setAck();
		Serial.println(" ..ACK");
	}

}
