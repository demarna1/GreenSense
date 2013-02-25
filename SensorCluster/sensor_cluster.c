// CPU clock speed used for delays and baud rate
#define F_CPU 15000000UL

// Include directories
#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>

// Define baud rate to be used
#define USART_BAUDRATE 1200

// Wireless transmission bytes
#define DUMMY 0x00
#define HEAD 0x4B

#define BAUD_PRESCALE (( F_CPU / ( USART_BAUDRATE * 16UL )) - 1)

// Define global variables for frequency capture
uint16_t new_edge = 0, prev_edge = 0, period = 0;

void setup() {
	// Set C as input for ADC
	DDRC = 0x00;
	// Set PB0 (pin 14) as input capture pin (ICP) and
	// set PB5 (pin 19) as output for digital control
	DDRB = (1 << DDB5);

	// Capture pulse on rising edge
	TCCR1B |= (1 << ICES1);
	// Enable input capture interrupt
	TIMSK1 |= (1 << ICIE1);

	// USART setup for wireless or debugging
	UCSR0B |= (1 << TXEN0);
	// Datasheet says to use UMSEL bit also, but it doesn't work
	//UCSR0C |= (1 << UMSEL00) | (1 << UCSZ01) | (1 << UCSZ00);
	UCSR0C |= (1 << UCSZ01) | (1 << UCSZ00);

	// Set baud prescale value used for timing
	UBRR0H = (BAUD_PRESCALE >> 8);
	UBRR0L = BAUD_PRESCALE;

	// Use AVcc with external capacitor at AREF pin? Not sure if needed
	ADMUX |= 0x40;
	// Enable ADC and set prescalar for ADC clock
	ADCSRA |= (1 << ADEN);
	ADCSRA |= (1 << ADPS2) | (1 << ADPS1) | (1 << ADPS0);

	// Enable global interrupts
	sei();
}

void transmit_byte(uint8_t data) {
	// Wait while previous byte is being transmitted
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

void send_packet(uint16_t temp, uint16_t humid, uint16_t soil) {
	// Transmit each portion of message
	transmit_byte(DUMMY);		// 0x00 - Synchro
	transmit_byte(DUMMY);		// 0x00 - Synchro
	transmit_byte(HEAD);		// 0x4B - Header
	transmit_byte(temp >> 8);	// High byte of temp
	transmit_byte(temp);		// Low byte of temp
	transmit_byte(humid >> 8);	// High byte of humid
	transmit_byte(humid);		// Low byte of humid
	transmit_byte(soil >> 8);	// High byte of soil
	transmit_byte(soil);		// Low byte of soil
	// Checksum of data
	transmit_byte(temp + (temp >> 8));	
}

uint16_t read_temp() {
	// Variables	
	uint8_t low, high;

	// Choose to read from ADC0 (pin 23)
	ADMUX = 0x40;

	// Start A2D Conversions
	ADCSRA |= (1 << ADSC); 

	// Wait until complete
	while (ADCSRA & (1 << ADSC)) {}

	// Pull values from ADC register
	low = ADCL;
	high = ADCH;
	return (high << 8) | low;
}

uint16_t read_soil() {
	// Variables	
	uint8_t low, high;

	// Choose to read from ADC2 (pin 25)
	ADMUX = 0x42;

	// Start A2D Conversions
	ADCSRA |= (1 << ADSC); 

	// Wait until complete
	while (ADCSRA & (1 << ADSC)) {}

	// Pull values from ADC register
	low = ADCL;
	high = ADCH;
	return (high << 8) | low;
}

// Interrupt occurs on positive edges
ISR(TIMER1_CAPT_vect) {
	new_edge = ICR1;
	period = new_edge - prev_edge;
	prev_edge = new_edge;
}

int main (void) {
	// Initialize variables
	uint16_t temp_raw, humid_raw, soil_raw;

	/*************************
	// VARIABLES FOR DEBUGGING
	int size;
	char buf[25];
	*************************/

	// Initialization function
	setup();

	while(1) {

		// Activate circuit - switch is active low
		// Set Pin 19 low (on ATMega328p)	
		PORTB &= ~0x20;	

		// Turn on internal timer1
		TCCR1B |= (1 << CS10);
		
		// Delay allows humidity capactior to adjust after standby
		_delay_ms(4000);

		// Read and store temperature, humidity, and soil moisture
		temp_raw = read_temp();
		humid_raw = period;
		soil_raw = read_soil();

		// Send sensor data packet
		send_packet(temp_raw, humid_raw, soil_raw);

		/***************************************************
		// USED ONLY FOR DEBUGGING - Send output to PC
		size = sprintf(buf, "tempraw=%d\n\r", temp_raw);
		write_serial(buf, size);
		size = sprintf(buf, "humidraw=%d\n\r", humid_raw);
		write_serial(buf, size);
		size = sprintf(buf, "soilraw=%d\n\r", soil_raw);
		write_serial(buf, size);
		***************************************************/

		// Turn off internal timer1
		TCCR1B &= ~(1 << CS10);

		// Deactivate circuit to save power
		// Set PB5 (pin 19) high
		PORTB |= 0x20;

		// Delay CPU for 2 seconds
		_delay_ms(2000);
	}
	return 0;
}

