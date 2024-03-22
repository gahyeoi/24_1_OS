#include <stdio.h>
#include <string.h>
#include <ctype.h>

struct Word{
	int point_starting;
	int point_ending;
	char word[50];
};

void printBrackets(char* String){

	char * ptr = String;
	
	int firstBracket = 0;
	int secondBracket = 0;

	char output[256] = "";

	while(*ptr != 0x0){
		if(!isspace(*ptr)){
			if(firstBracket){
				strcat(output, " ");
			} else {
				strcat(output, "[");
				strcat(output, " ");
				firstBracket = 1;
				secondBracket = 0;
			}
		} else {
			if(!secondBracket){
				strcat(output, "]");
				strcat(output, " ");
				secondBracket = 1;
				firstBracket = 0;
			} else {
				strcat(output, " ");
			}
		}

		ptr++;
	}

	strcat(output, "]");
	printf("%s\n", output);

}
int countWords(char* String){
	
	// If the input string is 0x0
	if(String == 0x0){
		return 0;
	}

	char copied_string[256];
	strcpy(copied_string, String);
	char * ptr = copied_string;
	ptr = strtok(ptr, " ");
	
	int count = 0;

	while(ptr != 0x0){
		count++;

		ptr = strtok(0x0, " ");
	}

	return count;
}

int divideWords(struct Word words[], char* String, int num_words){
	
	// If the input string is 0x0
	if(String == 0x0){
		return 0;
	}
	
	char copied_string[256];
	strcpy(copied_string, String);
	
	char * ptr = copied_string;
	ptr = strtok(ptr, " ");
	
	for(int i = 0; i < num_words; i++){
		strcpy(words[i].word, ptr);
		ptr = strtok(0x0, " ");
	}

	return 1;
}

void findPositions(struct Word words[], int num_words,  char* entireString){	
	
	char copied_string[256];
	strcpy(copied_string, entireString);

	char * ptr = copied_string;
	ptr = strtok(ptr, " ");

	for(int i = 0 ; i < num_words; i++){
		int num_beginning = strstr(ptr, words[i].word) - copied_string;
		words[i].point_starting = num_beginning;
		words[i].point_ending = num_beginning + strlen(words[i].word);
		ptr = strtok(0x0, " ");
	}
}

int main()
{
	char input[256];
	char output[256];

	printf("Input a text\n");

	fgets(input, sizeof(input), stdin);
	input[strlen(input)-1] = 0x0;

	int num_words = countWords(input);
	
	struct Word words[num_words];
	
	if((divideWords(words, input, num_words)==0)){
		printf("error");
	}
	
	printBrackets(input);

	findPositions(words, num_words, input);
	

	for(int i = 0 ; i < num_words; i++){
		printf("words[%d] = (%d, %d, %s)\n", i, words[i].point_starting, words[i].point_ending, words[i].word);
	}
	return 0;
}
