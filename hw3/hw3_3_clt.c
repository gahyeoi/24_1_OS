#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <fcntl.h>
#include <sys/shm.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <stdlib.h>

#define SIZE 512

int main(int argc, char* argv[]){
	
	// Retrieve the name of shared memory file from argv[1]
	char name[50] = "";
	strcpy(name, argv[1]);
	
	// Open the shared memory file
	int shm_fd = shm_open(name, O_RDWR, 0666);
	if(shm_fd == -1){
		perror("open the shared memory");
		exit(EXIT_FAILURE);
	}	

	// Map the shared memory to the current address space
	void* s = mmap(0, SIZE, PROT_WRITE, MAP_SHARED, shm_fd, 0);
	char* shm = (char*)s;
	if(shm == MAP_FAILED){
		perror("mmap");
		exit(EXIT_FAILURE);
	}

	printf("Welcome to my remote shell client!\n");
	printf("Connected to server via shared memory\n");

	char cmd[SIZE] = "";

	while(1){
		printf("$ ");
		fgets(cmd, SIZE, stdin);
		cmd[strlen(cmd)-1] = 0x0; 
		
		if(strcmp(cmd, "") != 0){

			while(strcmp((char*)shm, "") != 0){
                usleep(100000);
            }

			if(strcmp(cmd, "exit") == 0)
				break;

			// Copy the command to shared memory
            strncpy((char*)shm, cmd, SIZE);
			
			if(strcmp(cmd, "exit_svr") == 0)
				break;
		}

	}

	// Remove the shared memory file
	shm_unlink(name);

	// Dsplay a good-bye message
	printf("Bye!");	

	return 0;
}
