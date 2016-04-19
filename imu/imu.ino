/** \file
 * USB Seismagraph
 *
 * Turns the Trinket into a USB seismagraph for interfacing with scratch.
 */

#include <TrinketKeyboard.h>

#include <Wire.h>
#include "MMA8451.h"

// The built in LED on the trinket
#define LED_PIN 1

int last_keystroke;

void setup()
{
	TrinketKeyboard.begin();
	MMA8451_begin();
	pinMode(LED_PIN, OUTPUT);
}


char hexdigit(unsigned x)
{
	x &= 0xF;
	if (x < 10)
		return x + '0';
	else
		return x + 'A' - 0xA;
}

void loop()
{
	// The USB port must be polled at least every 10ms
	// to let the computer know that we are still here.
	TrinketKeyboard.poll();

	int now = millis();
	if (now - last_keystroke < 1000)
		return;

	digitalWrite(LED_PIN, 1);
	MMA8451_read();

	char buf[8] = {
		hexdigit(x >> 12),
		hexdigit(x >>  8),
		hexdigit(x >>  4),
		hexdigit(x >>  0),
		'\n',
		0
	};
		
	//TrinketKeyboard.pressKey(KEYCODE_MOD_LEFT_SHIFT, KEYCODE_A);
	//TrinketKeyboard.pressKey(0, 0); // release
	TrinketKeyboard.print(buf);
	last_keystroke = now;
	digitalWrite(LED_PIN, 0);
}
