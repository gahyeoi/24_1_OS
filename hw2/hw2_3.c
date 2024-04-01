#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <time.h>
#include <utime.h>
#include <stdlib.h>

void displayAttribute(struct stat st, struct tm* tm, char* fileName){
	
	time_t currentTime = time(0x0);
	struct tm* localTime = localtime(&st.st_mtime);

	printf("attributes of file \"%s\"\n", fileName);
	printf("\t st_dev = %ld\n", st.st_dev);
	printf("\t st_mode = %o\n", st.st_mode);
	printf("\t st_uid = %d\n", st.st_uid);
	printf("\t st_gid = %d\n", st.st_gid);
	printf("\t st_size = %d\n", st.st_blksize);
	printf("\t st_mtime = %ld\n", st.st_mtime);
	printf("\t modified time %d/%d/%d %d:%d:%d\n", localTime->tm_year+1990, localTime->tm_mon + 1, localTime->tm_mday, localTime->tm_hour, localTime->tm_min, localTime->tm_sec);

}

void copyAttribute(struct stat st, struct utimbuf buf, char* dstFile){

	if((chown(dstFile, st.st_uid, st.st_gid)) != 0){
		printf("Failed to change uid or gid\n");
		exit(1);
	}

	if((chmod(dstFile, st.st_mode)) != 0){
		printf("Failed to change mode\n");
		exit(1);
	}

	if((utime(dstFile, &buf)) != 0){
		printf("Failed to change time\n");
		exit(1);
	}
}
int main(int argc, char* argv[]){

	if(argc != 3){
		printf("Invalid number of input\n");
		return 1;
	}
	
	struct stat st; 
	struct tm* tm;
	struct utimbuf tmbuf; 
	
	if((stat(argv[1], &st) == -1)){
		printf("Failed to open %s\n", argv[1]);
		return 1;
	}

	tmbuf.actime = st.st_atime;
	tmbuf.modtime = st.st_mtime;
	
	displayAttribute(st, tm, argv[1]);
	copyAttribute(st, tmbuf, argv[2]);
	return 0;
}
