// CPU clock speed used for delays
#define F_CPU 15000000UL

// Include directories
#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include <avr/io.h>
#include <util/delay.h>

// Analog Pin 0 and wireless setting
#define ADC_PIN 0

// Define baud rate to be used
#define USART_BAUDRATE 4800

// Wireless transmission bytes
#define DUMMY 0x24
#define ADDR 0x75

#define BAUD_PRESCALE (( F_CPU / ( USART_BAUDRATE * 16UL )) - 1)

void setup() {
	// Set D as input, B as output
	DDRD = 0x00;
	DDRB = 0xFF;

	// USART setup for wireless or debugging
	UCSR0B |= (1 << TXEN0);
	// Datasheet says to use UMSEL bit also, but it doesn't work
	//UCSR0C |= (1 << UMSEL00) | (1 << UCSZ01) | (1 << UCSZ00);
	UCSR0C |= (1 << UCSZ01) | (1 << UCSZ00);

	// Set baud prescale value into low and high register
	UBRR0H = (BAUD_PRESCALE >> 8);
	UBRR0L = BAUD_PRESCALE;

	// Choose which ADC pin to read
	ADMUX |= (1 << REFS0);
	// No MUX values need to be changed to use ADC0 

	// Enable ADC and set prescalar for ADC clock
	ADCSRA |= (1 << ADEN);
	ADCSRA |= (1 << ADPS2) | (1 << ADPS1) | (1 << ADPS0);
}

void transmit_byte(uint8_t data) {
	// Wait if a byte is being transmitted
	while ((UCSR0A & (1 << UDRE0)) == 0) {}

	// Transmit data
	UDR0 = data;
}

void write_serial(char *string, int size) {
	// Send string to serial port
	int i;
	for (i = 0; i < size; i++) {
		transmit_byte(string[i]);
	}
}

void send_packet(uint8_t data) {
	// Transmit each portion of message
	transmit_byte(DUMMY);
	transmit_byte(ADDR);
	transmit_byte(data);
	transmit_byte(ADDR + data);
}

uint16_t read_adc() {
	// Variables	
	uint8_t low, high;

	// Start A2D Conversions
	ADCSRA |= (1 << ADSC); 

	// wait until complete
	while (ADCSRA & (1 << ADSC)) {}

	// Pull values from ADC register
	low = ADCL;
	high = ADCH;
	return (high << 8) | low;
}

int main (void) {
	// Initialize variables
	uint16_t temp_raw;
	unsigned char on = 0;

	/*************************
	// VARIABLES FOR DEBUGGING
	int size;
	char buf[25];
	*************************/

	// Initialization function
	setup();

	while(1) {
		// Read and store temperature
		temp_raw = read_adc();

		// Send temperature data packet
		send_packet(temp_raw >> 2);

		/***************************************************
		// USED ONLY FOR DEBUGGING - Send output to PC
		size = sprintf(buf, "tempraw=%d\n\r", temp_raw);
		write_serial(buf, size);
		***************************************************/

		// Blink LED
		if (on) {
			PORTB &= ~0x20;
			on = 0;
		} 
		else {
			PORTB |= 0x20;
			on = 1;
		}

		// Delay CPU for 1 second
		_delay_ms(1000);
	}
	return 0;
}


