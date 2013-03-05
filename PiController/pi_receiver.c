#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <termios.h>

//NOTES: Alt-92 for \; Alt-124 for |; \ for #

#define HEAD 0x4B
#define DATA_SIZE 7

void read_remaining_bytes(int fd, unsigned char *old_buffer, int len, int index) {
	// Declare variables
	unsigned char new_buffer[DATA_SIZE];
	unsigned char data[DATA_SIZE];
	unsigned int temp, humid, soil;
	unsigned char checksum, chk;
	int i, new_len, count = 0;

	// Finish reading old_buffer
	for (i = index; (i < len) && (count < DATA_SIZE); i++) {
		data[count] = old_buffer[i];
		count++;
	}

	// Read into new buffer
	while (count < DATA_SIZE) {
		new_len = read(fd, (void*)new_buffer, DATA_SIZE);
		if (new_len > 0) {
			for (i = 0; (i < new_len) && (count < DATA_SIZE); i++) {
				data[count] = new_buffer[i];
				count++;
			}
		}
	}

	// All data has been received, place into variables
	temp = data[0] << 8;
	temp |= data[1];
	humid = data[2] << 8;
	humid |= data[3];
	soil = data[4] << 8;
	soil |= data[5];
	checksum = data[6];

	// Evaluate the checksum
	chk = temp + humid + soil;
	if (chk == checksum) {
		// Print data
		printf("temp = %d\n", temp);
		printf("humid = %d\n", humid);
		printf("soil = %d\n\n", soil);
	}
	else {
		printf("Bad checksum\n");
	}
}

int main() {
	// Declare variables
	int uart0_filestream = -1;
	int i, rx_length;
	unsigned char rx_buffer[256];

	// Open UART filestream
	uart0_filestream = open("/dev/ttyAMA0", O_RDWR | O_NOCTTY | O_NDELAY);
	if (uart0_filestream == -1) {
		printf("Error opening UART.");
	}

	// Configure UART
	struct termios options;
	tcgetattr(uart0_filestream, &options);
	options.c_cflag = B1200 | CS8 | CLOCAL | CREAD; //Sets BAUD rate
	options.c_iflag = IGNPAR | ICRNL;
	options.c_oflag = 0;
	options.c_lflag = 0;
	tcflush(uart0_filestream, TCIFLUSH);
	tcsetattr(uart0_filestream, TCSANOW, &options);

	// Start Reception
	printf("Starting reception...\n");
	while (1) {
		// Check to see file is still valid
		if (uart0_filestream != -1) {
			// Read up to 256 bytes into rx_buffer
			rx_length = read(uart0_filestream, (void*)rx_buffer, 256);
			// Check to see if any bytes successfully read
			if (rx_length > 0) {
				// Iterate through received bytes
				for (i = 0; i < rx_length; i++) {
					// Check for matching header byte
					if (rx_buffer[i] == HEAD) {
						read_remaining_bytes(uart0_filestream, rx_buffer, rx_length, i+1);
						// Breaks out of for loop
						break;
					}
				}
			}
		}
	}

	close(uart0_filestream);

	return 0;
}
