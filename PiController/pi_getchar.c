#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <termios.h>

//NOTES: Alt-92 for \; Alt-124 for |; \ for #

#define HEAD 0x4B
#define DATA_SIZE 7

// Execute a bash script and pass data to it. Return -1 if unsuccessful, 
// and 0 if successful.
int bash_upload(unsigned int temp, unsigned int humid, unsigned int soil) {
	// Declare variables
	int status;
	pid_t pid;	
	char data_arg[50];
	char *script = "./upload.sh";

	// Create argument array to be passed to bash
	sprintf(data_arg, "%d,%d,%d", temp, humid, soil);

	// Fork a new process which will run bash script
	pid = fork();
	if (pid == -1) {
		// Unable to fork a new process
		fprintf(stderr, "C: Error forking.\n");
		return -1;
	}

	// Child's process id will be zero
	else if (pid == 0) {
		// Execute bash script; won't return if successful
		if (execl(script, script, data_arg, NULL) == -1) {
			// Unsuccessful execution
			fprintf(stderr, "C: Error executing ./upload.sh.\n");
			return -1;
		}
	}
	
	// This block contains the parent code
	else {
		// Wait for bash script to finish
		printf("C: Waiting...\n");
		waitpid(-1, &status, 0);
		printf("C: Script finished successfully.\n");
	} 

	// Return successfully
	return 0;
}

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
		printf("soil = %d\n", soil);
		// Upload data via bash script
		if (bash_upload(temp, humid, soil) == 0) {
			printf("Data sent to bash script.\n\n");
		} 
		else {
			printf("Data failed to upload.\n\n");
		}
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
		fprintf(stderr, "Error opening UART.\n");
		exit(1);
	}

	// Configure UART
	struct termios options;
	tcgetattr(uart0_filestream, &options);
	// Sets BAUD rate and other options 
	options.c_cflag = B1200 | CS8 | CLOCAL | CREAD;
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
			// Iterate through received bytes
			for (i = 0; i < rx_length; i++) {
				// Check for matching header byte
				if (rx_buffer[i] == HEAD) {
					read_remaining_bytes(uart0_filestream, 
						rx_buffer, rx_length, i+1);
					// Breaks out of for loop
					break;
				}
			}
		} 
	}

	// Should never get here but close file descriptor as a best practice
	close(uart0_filestream);
	return 0;
}

