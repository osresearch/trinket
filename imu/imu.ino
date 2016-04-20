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
int min_x = +65536;
int max_x = -65536;

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

	// track the maximum/min value read during this period
	MMA8451_read();
	if (x < min_x)
		min_x = x;
	if (x > max_x)
		max_x = x;

	// Send an update at 4 Hz
	int now = millis();
	if (now - last_keystroke < 250)
		return;
	last_keystroke = now;


	digitalWrite(LED_PIN, 1);

	// convert the current 14-bit X acceleration into a value
	// from +/- 4096 to the letters A-Z

	// pick the larger magnitude of the readings
	int v = max_x > -min_x ? max_x : min_x;
	max_x = -65536;
	min_x = +65536;
	

	char buf[8] = {
		hexdigit(v >> 12),
		hexdigit(v >>  8),
		hexdigit(v >>  4),
		hexdigit(v >>  0),
		'\n',
		0
	};
		
	//TrinketKeyboard.pressKey(KEYCODE_MOD_LEFT_SHIFT, KEYCODE_A);
	//TrinketKeyboard.pressKey(0, 0); // release
	TrinketKeyboard.print(buf);
	digitalWrite(LED_PIN, 0);
}
