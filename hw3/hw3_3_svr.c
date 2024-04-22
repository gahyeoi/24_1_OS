#include <sys/wait.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <fcntl.h>
#include <sys/shm.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <stdlib.h>

#define SIZE 512

void parseArgument(char* string, char* array[], int *count){
	char * ptr = string; 
	
	int cnt = 0 ;

	ptr = strtok(ptr, " "); 
	
	while(ptr != 0x0){
		array[cnt] = ptr;
		cnt++;
		ptr = strtok(0x0, " ");
	}
	array[cnt] = 0x0;

	*count = cnt;
}

int main(int argc, char* argv[]){
	
	// Retrieve the name of shared memory file from argv[1]
	char name[50] = "";
	strcpy(name, argv[1]);
	
	// Create a shared memory file
	int shm_fd = shm_open(name, O_CREAT | O_RDWR, 0666);
	if(shm_fd == -1){
		perror("Failed to create a shared memory");
		exit(EXIT_FAILURE);
	}	

	// Set the size of the shared memory file to 512
	if((ftruncate(shm_fd, SIZE)) == -1){
		perror("failed to set the size of shared memory\n");
		exit(EXIT_FAILURE);
	}

	// Map the shared memory to the current address space
	void* s = mmap(0, SIZE, PROT_WRITE | PROT_READ, MAP_SHARED, shm_fd, 0);
	char* shm = (char*)s;

	if(shm == MAP_FAILED){
		perror("mmap");
		exit(EXIT_FAILURE);
	}
	// Initialize the shared memory
	memset(shm, 0, SIZE);
	
	printf("Welcome to my remote shell server!\n");
	printf("Connected to client via shared memory\n");

	char cmd[SIZE] = "";
	char *arguments[64];
	int count = 0 ; 

	while(1){

		printf("Waiting for a command...\n");

		// Repeat to call usleep(100000) until shm[0] != 0
		while(shm[0] == 0){
			usleep(100000);
		}
		
		strcpy(cmd, shm);
	
		// Reset the shared memory to an empty string 
		memset(shm, 0, SIZE);

		// If the command is "exit_svr", break 
		if(strcmp(cmd, "exit_svr") == 0)
			break;
			
		// Cut the command into an arugment vector and execute it 
		parseArgument(cmd, arguments, &count);

		if(strcmp(arguments[0], "cd")==0){
			if((chdir(arguments[1]) != 0)){
				printf("cd failed\n");
			}
		} else if(strcmp(arguments[0], "echo") == 0){
			for(int i = 1; i < count; i++){
				printf("%s ", arguments[i]);
			}
			printf("\n");
		}else {

			pid_t child_pid = fork(); 
			if(child_pid < 0){
				fprintf(stderr, "fork failed\n"); 
			} else if(child_pid == 0){
				execvp(arguments[0], arguments);
				exit(0);
			} else {
				wait(0x0);
			}
		}
	}

	// Remove the shared memory file
	shm_unlink(name);

	// Dsplay a good-bye message
	printf("Bye!");	

	return 0;
}
