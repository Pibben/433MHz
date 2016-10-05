/*
 * main.cpp
 *
 *  Created on: Sep 6, 2016
 *      Author: per
 */


#include <Arduino.h>

int rxPin = 5;
int ledPin = 13;

#ifdef DEBUG
static void printHex8(uint8_t data)
{
	char tmp[16];
	sprintf(tmp, "0x%.2X", data);
	Serial.print(tmp);
}
#endif

bool receive433MHz(uint32_t code, uint8_t maxRetries) {

	int i = 0;
	uint32_t data = 0;
	unsigned long t;
	uint8_t retries = 0;
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
			retries++;

			if(retries > maxRetries) {
				return false;
			}

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
	printHex8(data >> 24);
	Serial.print(" ");
	printHex8((data >> 16) & 0xff);
	Serial.print(" ");
	printHex8((data >> 8) & 0xff);
	Serial.print(" ");
	printHex8(data & 0xff);
	Serial.println("");
#endif

	return data == code;
}

#if 0
int main() {
	init();

	pinMode(ledPin, OUTPUT);
	pinMode(rxPin, INPUT);
	Serial.begin(9600);

	bool on = true;
	int count = 0;
	while (true) {

		if(receive433MHz(0x00899381, 3)) {
			Serial.println("Ping!");
		}

		if(++count == 100) {
			on = !on;

			digitalWrite(ledPin, on);
			count = 0;
		}

		delay(10);
	}
}
#endif
