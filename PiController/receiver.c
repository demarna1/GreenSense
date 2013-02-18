#define F_CPU 16000000UL

#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>

//set desired baud rate
#define USART_BAUDRATE 4800

//define receiver parameters
#define DUMMY 0x24	// synchro signal
#define ADDR 0x75		// address 

//calculate UBRR value
#define BAUD_PRESCALE (( F_CPU / ( USART_BAUDRATE * 16UL )) - 1)

// Initializes ports, USART, and receive interrupts
void setup() {
	// LED off
	//PORTC |= (1 << 0);	
	
	//define port C pin 0 as output; Set PortB as output
	//DDRC=0x01;	
	DDRB = 0xFF;	

	//Set baud rate
	UBRR0H = (BAUD_PRESCALE >> 8);	//high byte
	UBRR0L = BAUD_PRESCALE;			//low byte
	
	// Data frame format: asynchronous mode, no parity, 1 stop bit, 8 bit size
	UCSR0C |= (1 << UCSZ01) | (1 << UCSZ00);
	//(1<<URSEL)|(0<<UMSEL)|(0<<UPM1)|(0<<UPM0)|(0<<USBS)|(0<<UCSZ2)|(1<<UCSZ01)|(1<<UCSZ00);	
	
	//Enable Receiver and Interrupt on receive complete
	UCSR0B = (1 << RXEN0) | (1 << RXCIE0) | (1 << TXEN0); 
	
	//enable global interrupts
	sei();
}

uint8_t receive_byte() {
	// Wait until a byte has been received
	while((UCSR0A & (1 << RXC0)) == 0);
    
	// Return received data
	return UDR0;
}

void transmit_byte(uint8_t data) {
	while ((UCSR0A & (1 << UDRE0)) == 0) {}
	UDR0 = data;
}

ISR(USART_RX_vect) {
	uint8_t byte = receive_byte();
	transmit_byte(byte);
	
	/**********************************************************
	// Actual code for parsing the data

	//define variables
	uint8_t raddress, data, chk;//transmitter address
	//receive destination address
	raddress=USART_vReceiveByte();
	//receive data
	data=USART_vReceiveByte();
	//receive checksum
	chk=USART_vReceiveByte();
	//compare received checksum with calculated
	if(chk==(raddress+data))//if match perform operations
	{
		//if transmitter address match
		if(raddress==RADDR)
		{
			if(on) {
				PORTC|=(1<<0);//LED OFF
				on = 0;
			}
			else {	
				PORTC&=~(1<<0);//LED ON
				on = 1;
			}
		}
	}
	**********************************************************/
}

int main() {
	unsigned char on = 0;

	// Initialization function	
	setup();
	
	while(1) {
		// interrupts are doing most of the work

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

