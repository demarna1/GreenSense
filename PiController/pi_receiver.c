#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <termios.h>

//NOTES: Alt-92 for backslash; Alt-124 for bar

int main() {
	unsigned char HEAD = 0x4B;
	//Setup UART
	int uart0_filestream = -1;
	uart0_filestream = open("/dev/ttyAMA0", O_RDWR | O_NOCTTY | O_NDELAY);
	if (uart0_filestream == -1) {
		printf("Error opening UART.");
	}

	//Configure UART
	struct termios options;
	tcgetattr(uart0_filestream, &options);
	options.c_cflag = B1200 | CS8 | CLOCAL | CREAD;
	options.c_iflag = IGNPAR | ICRNL;
	options.c_oflag = 0;
	options.c_lflag = 0;
	tcflush(uart0_filestream, TCIFLUSH);
	tcsetattr(uart0_filestream, TCSANOW, &options);

	//Start Reception
	printf("Starting reception.\n");
	printf("HEAD = %c\n", HEAD);
	while (1) {
		//Check to see file is still valid
		if (uart0_filestream != -1) {
			int i, rx_length;
			unsigned char rx_buffer[256];
			rx_length = read(uart0_filestream, (void*)rx_buffer, 255);
			//Check to see if any bytes successfully read
			if (rx_length > 0) {
				for (i = 0; i < rx_length; i++) {
					if (rx_buffer[i] == HEAD) {
						printf("Header found: len=%d, i=%d\n", rx_length, i);
					}
				}
			}
		}
	}

	close(uart0_filestream);

	return 0;
}
