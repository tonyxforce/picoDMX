/*
 * Copyright (c) 2021 Jostein Løwer 
 *
 * SPDX-License-Identifier: BSD-3-Clause
 * 
 * Description: 
 * Starts a DMX Input on GPIO pin 15 and read channel 1-3 repeatedly
 */

#include <Arduino.h>
#include "DmxInput.h"
DmxInput dmxInput;

uint startChannel = 1;
#define NUM_CHANNELS 3

volatile uint8_t buffer[512+1];

void setup()
{
    // Setup our DMX Input to read on GPIO 0, from channel 1 to 3
    dmxInput.begin(15, startChannel, 512);

    // Setup the onboard LED so that we can blink when we receives packets
    pinMode(LED_BUILTIN, OUTPUT);
		Serial.setTimeout(100);
}

void loop()
{
    // Wait for next DMX packet
    dmxInput.read(buffer);

		if(Serial.available()){
			delay(100);
			// Read the new start channel from the serial input, if available and discard the rest of the input buffer for a cleaner read
			startChannel = (Serial.parseInt());
			startChannel = constrain(startChannel, 1, 510);
			while(Serial.available()){
				Serial.read();  // Discard the rest of the input buffer for a cleaner read
			}
		}

    // Print the DMX channels
    for (uint i = startChannel; i < startChannel + NUM_CHANNELS; i++)
    {
        Serial.print(buffer[i]);
        Serial.print(", ");
    }
    Serial.println("");

    // Blink the LED to indicate that a packet was received
		analogWrite(LED_BUILTIN, buffer[1]);
}