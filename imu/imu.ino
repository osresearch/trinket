/** \file
 * USB Seismagraph
 *
 * Turns the Trinket into a USB seismagraph for interfacing with scratch.
 */

#include <TrinketKeyboard.h>

// The built in LED on the trinket
#define LED_PIN 1

int last_keystroke;

void setup()
{
	TrinketKeyboard.begin();
	pinMode(LED_PIN, OUTPUT);
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
	TrinketKeyboard.pressKey(KEYCODE_MOD_LEFT_SHIFT, KEYCODE_A);
	TrinketKeyboard.pressKey(0, 0); // release
	last_keystroke = now;
	digitalWrite(LED_PIN, 0);
}
