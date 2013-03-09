
#define STDIN_FILENO 0

int main() {
	int n, child_parent_pipe[2];
	pid_t pid;	

	// Create pipe
	if (pipe(child_parent_pipe) < 0) {
		fprintf(stderr, "Error creating pipe.\n");
	}

	// Fork a new process
	if ((pid = fork()) < 0) {
		fprintf(stderr, "Error forking.\n");
	} 
	
	else if (pid > 0) {
		// Parent code
		
	} 

	else {
		// Child code
	}
	
	return 0;
}
