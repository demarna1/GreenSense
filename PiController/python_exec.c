#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main() {
	// Declare variables
	int status;
	pid_t pid;	
	char data_arg[50];
	unsigned int temp = 799;
	unsigned int humid = 2444;
	unsigned int soil = 800;

	// Create argument array to be passed to bash script
	char *script = "./upload.sh";
	sprintf(data_arg, "%d,%d,%d", temp, humid, soil);

	// Fork a new process
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
			fprintf(stderr, "C: Error executing ./upload.sh.");
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

