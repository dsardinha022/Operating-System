/*
Damien Sardinha
Project D Improve Shell Program
December 11th, 2018
*/
void commandSearch(char*);
void extractFilename(char*, char*);
void copyFilename(char*, char*);
void createFile(char*);
void programDir();

main()
{
	while(1){
		char line[80];

		interrupt(0x21, 0, "Shell> \0", 0, 0);
		interrupt(0x21, 1, line, 0, 0);
		commandSearch(line);
		
	}
}

void commandSearch(char* input){
	char* type = "type\0";
	char* exec = "execute\0";
	char* dele = "delete\0";
	char* copy = "copy\0";
	char* dir = "dir\0";
	char* crte = "create\0";

	char buffer[13312];
	char fileName[7];
	char newfileName[7];

	int sector=0;
	int i=0; 
	int j=0, commandL = 0;

	if(input[0] == '\0'){
		interrupt(0x21, 0, "Help? COMMANDS: type execute\0", 0, 0);
	}else{
		for(i; input[i] != ' '; i++){
			commandL = i;
			/*interrupt(0x21, 0, "hit\0", 0, 0);*/

			if(commandL == 9){
				break;
			}
		}
		if(commandL == 2){
			if(input[0] == dir[0]){
			interrupt(0x21, 0, "\r\n\0", 0, 0);	
			interrupt(0x21, 9, 0, 0, 0);
			}
	
		}
		else if(commandL == 3){

			extractFilename(input, fileName);

			if(input[0] == type[0]){
				interrupt(0x21, 3, fileName, buffer, 0);
				interrupt(0x21, 0, buffer, 0, 0);

				
			}
			else if(input[0] == copy[0]){
				interrupt(0x21, 3, fileName, buffer, 0);

				copyFilename(input, newfileName);
				interrupt(0x21, 9, fileName, sector, 0);

				interrupt(0x21, 8, newfileName, buffer, 26);
			}
			else{
				interrupt(0x21, 0, "Unable to process file!\0", 0, 0);

			}

		}
		else if(commandL == 5){
			extractFilename(input, fileName);

			
			if(input[0] == dele[0]){
				interrupt(0x21, 7, fileName, 0, 0);
			}
			else if(input[0] == crte[0]){
				interrupt(0x21, 0, "\r\n\0", 0, 0);
				interrupt(0x21, 0, "Max file size 512 bytes: \r\n\0", 0, 0);
				createFile(fileName);
			
			}else{
				interrupt(0x21, 0, "Unable to delete file!\0", 0, 0);
			}
			
		}
		else if(commandL == 6){
			extractFilename(input, fileName);
	
			if(input[0] == exec[0]){
				interrupt(0x21, 4, fileName, 0x2000, 0);
	
			}
			else{
				interrupt(0x21, 0, "Invalid command!", 0, 0);
			}

		}
		else{

			interrupt(0x21, 0, "Unknown command! \0", 0, 0);
		}

	}
}
void extractFilename(char* input, char* output){
	int counter = 0, j = 0;

	do{
		counter++;

	}while(input[counter] != ' ');


	for(j; j < 6; j++){
		if(input[counter+j+1] != 0x00){
			output[j] = input[counter+j+1];
		}
		else{
			output[j] = 0x00;
		}
	}

	output[6] = '\0';
}
void copyFilename(char* input, char* output){
	int counter = 0, j = 0;

	do{
		counter++;
	}while(input[counter] != ' ');

	do{
		counter++;
	}while(input[counter] != ' ');

	counter++;

	for(j; j < 6; j++){
		if(input[counter+j] != 0x00){
			output[j] = input[counter+j];
		}
		else{
			output[j] = 0x00;
		}

	}
	output[6] = '\0';
}
void createFile(char* filename){
	int freeLine = 0, i=0, counter=0;
	char buffer[512];

	while(freeLine != 1){

	char line[80];

	interrupt(0x21, 0, "Line: \0", 0, 0);
	interrupt(0x21, 1, line, 0, 0);

		for(i=0; line[i] != 0; i++){
			buffer[counter] = line[i];
			counter++;
		}
	
	buffer[counter] = '\r';
	counter++;
	buffer[counter] = '\n';
	counter++;
	
	interrupt(0x21, 0, "\r\n\0", 0, 0);

		if(line[0] == 0){
			interrupt(0x21, 0, "End of file: Name| \0", 0, 0);
			freeLine = 1;
		}
	}

	interrupt(0x21, 0, filename, 0, 0);
	interrupt(0x21, 0, "\r\n\0", 0, 0);
	interrupt(0x21, 8, filename, buffer, 1);

}
