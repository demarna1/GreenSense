#define F_CPU 16000000UL

#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include <stdio.h>

//set desired baud rate
#define USART_BAUDRATE 1200

//define receiver parameters
#define DUMMY 0x00	// synchro signal
#define HEAD 0x4B		// header 

//calculate UBRR value
#define BAUD_PRESCALE (( F_CPU / ( USART_BAUDRATE * 16UL )) - 1)

// Initializes ports, USART, and receive interrupts
void setup() {
	//Set baud rate
	UBRR0H = (BAUD_PRESCALE >> 8);	//high byte
	UBRR0L = BAUD_PRESCALE;			//low byte
	
	// Data frame format: asynchronous mode, no parity, 1 stop bit, 8 bit size
	UCSR0C |= (1 << UCSZ01) | (1 << UCSZ00);
	//(1<<URSEL)|(0<<UMSEL)|(0<<UPM1)|(0<<UPM0)|(0<<USBS)|(0<<UCSZ2)|(1<<UCSZ01)|(1<<UCSZ00);	
	
	//Enable Receiver and Interrupt on receive complete
	UCSR0B = (1 << RXEN0) | (1 << TXEN0);// | (1 << RXCIE0);
}

uint8_t receive_byte() {
	// Wait until a byte has been received
	while ((UCSR0A & (1 << RXC0)) == 0) {}
	// Return received data
	return UDR0;
}

void transmit_byte(uint8_t data) {
	// Wait until previous byte has finished transmitting
	while ((UCSR0A & (1 << UDRE0)) == 0) {}
	// Transmit byte
	UDR0 = data;
}

void write_serial(char *string, int size) {
	// Send string to serial port
	int i;
	for (i = 0; i < size; i++) {
		transmit_byte(string[i]);
	}
}

int main() {
	uint16_t temp, humid, soil;
	uint8_t checksum, chk;
	char buf[25];
	int size;

	// Initialization function	
	setup();
	
	while(1) {
		// Simple filter on header
		if (receive_byte() == HEAD) {
			// Receive next remaining bytes
			temp = receive_byte() << 8;
			temp |= receive_byte();
			humid = receive_byte() << 8;
			humid |= receive_byte();
			soil = receive_byte() << 8;
			soil |= receive_byte();
			checksum = receive_byte();
			// Print output to screen
			size = sprintf(buf, "temp=%d\n", temp);
			write_serial(buf, size);
			size = sprintf(buf, "humid=%d\n", humid);
			write_serial(buf, size);
			size = sprintf(buf, "soil=%d\n", soil);
			write_serial(buf, size);
			// Check that data is valid
			chk = temp + (temp >> 8);		
			if (checksum == chk) {
				size = sprintf(buf, "Chk is good.\n\r");
				write_serial(buf, size);
			}
		}
	}

	return 0;
}

