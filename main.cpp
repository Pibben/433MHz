/*
 * main.cpp
 *
 *  Created on: Sep 6, 2016
 *      Author: per
 */


#include <Arduino.h>

int rxPin = 12;
int ledPin = 13;

static void printHex8(uint8_t data)
{
	char tmp[16];
	sprintf(tmp, "0x%.2X", data);
	Serial.print(tmp);
}

int main() {
	init();

	pinMode(ledPin, OUTPUT);
	pinMode(rxPin, INPUT);
	Serial.begin(9600);

	while (true) {
		int i = 0;
		uint32_t data = 0;
		unsigned long t;
#ifdef DEBUG
		unsigned int pulses[25];
		unsigned char currPulse = 0;
#endif
		while (i < 25) {
			t = pulseIn(rxPin, HIGH, 2000);
#ifdef DEBUG
			pulses[currPulse] = t;
			currPulse++;
#endif
			if (t < 250 || t > 1250) {
				// pulse timing off or timeout - reset

				data = 0;
#ifdef DEBUG
				currPulse = 0;
#endif
				i = 0;

				continue;
			}

		    data = (data>>1) | (t > 500 ? 0x1UL << 24 : 0);

			++i;
		}

#ifdef DEBUG
		for(int j = 0; j < currPulse; ++j) {
			Serial.print(j);
			Serial.print(" ");
			Serial.println(pulses[j]);
		}

		Serial.println(data);
#endif
		printHex8(data >> 24);
		Serial.print(" ");
		printHex8((data >> 16) & 0xff);
		Serial.print(" ");
		printHex8((data >> 8) & 0xff);
		Serial.print(" ");
		printHex8(data & 0xff);
		Serial.println("");
	}
}

