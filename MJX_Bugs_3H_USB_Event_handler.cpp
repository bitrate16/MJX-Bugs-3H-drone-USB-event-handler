#include <SoftwareSerial.h>

// Copyright 2020, bitrate16
// https://github.com/bitrate16/MJX-Bugs-3H-drone-USB-event-handler
// 
// The following dource code demonstrated usage of the Serial input 
//  reader to parse packets from the MJX Bugs 3H drone's micro usb port.
// Useful for people that do not want to buy the official camera and 
//  use this port and transmitter Photo buttor for their own needs.

// Tested on my drone with Arduino micro connected via soft serial.
// Baud:
// 300     output doesn't classify
// 1200    output doesn't classify
// 2400    output doesn't classify
// 4800    output codes noisy
// 9600    transmitter off end opcode match the photo opcode
// 19200   output codes noisy but classify
// 38400   output codes classify a bit noisy
// 57600   output codes classify, transmitter off end opcode match the photo opcode
// 74880   output codes classify a bit noisy
// 115200  output codes classify, transmitter off end opcode match the photo opcode
// 230400  output codes classify, transmitter off end opcode match the photo opcode
// 250000  output codes classify, transmitter off end opcode match the photo opcode
// 500000  output doesn't classify
// 1000000 output doesn't classify
// 2000000 output doesn't classify

// Delta used to split packets apart
#define MS_DELTA    10
// Serial output baud rate
#define HARD_BAUD   115200
// SoftSerial capture from drone rate
#define SOFT_BAUD   115200
// TX, RX for SoftSerial
#define SOFT_TX_PIN 8
#define SOFT_RX_PIN 9

// Using baud 115200 to capture stopcode indicating the camera button
// dT = 10ms
// During the test it was found that drone is sending different codes 
//  for long press and fot short press of the button.
SoftwareSerial mySerial(8, 9);

long old_ts = 0;
bool newlined = 0;

// Capture buffer
size_t buffer_size = 0;
unsigned char buffer[256];

// Constant sequences
const unsigned char photo_short_code[]         = { 224, 28, 252,  0, 0, 28, 0, 224, 0, 28 }; // Usually stable with 1/1000 error
const unsigned char photo_long_code[]          = { 224, 28, 252, 28, 0, 28, 0, 252, 0, 28 }; // Usually stable with 1/1000 error

const unsigned char drone_idle_code[]          = { 224, 28, 252, 28, 224, 252, 0, 224, 28, 252 }; // Unstable in low battery (optional opcode then)
const unsigned char transmitter_off_code[]     = { 224, 28, 252, 28, 224, 252, 0, 252, 252 }; // Unstable in low battery (optional opcode then)
const unsigned char drone_motors_on_code[]     = { 224, 28, 252, 28, 224, 252, 0,  28, 224, 252 }; // Unstable in low battery (optional opcode then)

// Checks if passed array match the buffer value
bool match_full(const unsigned char* sequence, size_t size) {
	if (size != buffer_size)
		return 0;
	for (size_t i = 0; i < size; ++i) 
		if (buffer[i] != sequence[i])
			return 0;
	return 1;
}

// Checks if passed array match the buffer preffix value
bool match_preffix(const unsigned char* sequence, size_t size) {
	if (size > buffer_size)
		return 0;
	for (size_t i = 0; i < size; ++i)
		if (buffer[i] != sequence[i])
			return 0;
	return 1;
}

// Checks if passed array match the buffer postfix value
bool match_postfix(const unsigned char* sequence, size_t size) {
	if (size > buffer_size)
		return 0;
	for (size_t i = 0; i < size; ++i)
		if (buffer[buffer_size - i - 1] != sequence[buffer_size - i - 1])
			return 0;
	return 1;
}

void setup() {
	  Serial.begin(HARD_BAUD);
	  mySerial.begin(SOFT_BAUD);
	  old_ts = millis();
}

void loop() {
	// If data available
	if (mySerial.available()) {
		// Update buffer value
		buffer[buffer_size] = mySerial.read();
		//Serial.print((char) buffer[buffer_size]);
		++buffer_size;
		
		old_ts = millis();
		newlined = 0;
	}

	// If data timeout reached
	if (millis() - old_ts > 10 && !newlined) {
		Serial.println();
		newlined = 1;
		
		// Try direct parse command
		if (match_full(photo_short_code, sizeof(photo_short_code)))
			photo_short();
		
		else if (match_full(photo_long_code, sizeof(photo_long_code)))
			photo_long();
		
		// Try preffix / postfix parse
		else if (match_preffix(photo_short_code, sizeof(photo_short_code)))
			photo_short();
		
		else if (match_postfix(photo_short_code, sizeof(photo_short_code)))
			photo_short();
		
		else if (match_preffix(photo_long_code, sizeof(photo_long_code)))
			photo_long();
		
		else if (match_postfix(photo_long_code, sizeof(photo_long_code)))
			photo_long();
		
		// Try direct parse drone codes
		else if (match_preffix(drone_idle_code, sizeof(drone_idle_code)))
			drone_idle();
		
		else if (match_preffix(transmitter_off_code, sizeof(transmitter_off_code)))
			transmitter_off();
		
		else if (match_preffix(drone_motors_on_code, sizeof(drone_motors_on_code)))
			drone_motors_on();
		
		// For people that want to tune opcodes if they do not match
		Serial.println("Code line:");
		for (size_t i = i; i < buffer_size; ++i) {
			Serial.print((int) buffer[i]);
			if (i != buffer_size - 1) {
				Serial.print(',');
				Serial.print(' ');
			}
		}
		Serial.println();
		
		buffer_size = 0;
	}
}

void photo_short() {
	Serial.println("photo_short()");
	
}

void photo_long() {
	Serial.println("photo_long()");
	
}

void drone_idle() {
	Serial.println("drone_idle()");
	
}

void transmitter_off() {
	Serial.println("transmitter_off()");
	
}

void drone_motors_on() {
	Serial.println("drone_motors_on()");
	
}
