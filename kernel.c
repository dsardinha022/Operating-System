/*
Damien Sardinha
Project D Improved Kernel
December 11th, 2018
*/
void printString(char*);
void printChar(char);
void readString(char*);
void readSector(char*, int);
void writeSector(char*, int);
void printSector(char*);
void readFile(char*, char*);
void writeFile(char*, char*, int);
void deleteFile(char*);
void executeProgram(char*, int);
void handleInterrupt21(int, int, int, int);
void programDir();
void terminate();
int mod(int, int);
int div(int, int);


main()
{


	makeInterrupt21();

	interrupt(0x21, 4, "shell\0", 0x2000, 0);
	
	while(1)
	{
	}

}

void printString(char* chars)
{
	int i;

	for(i = 0; chars[i] != 0; i++){
	 
		printChar(chars[i]);
	}

}
void printSector(char* charj){

	int i = 0;

	do{
		printChar(charj[i]);
		i++;
	}while(i != 512);

}
void readString(char* charz)
{
	int enter = 0xd,backspace = 0x8, arrayValue = 0, returnValue; 

	while(returnValue != enter){

	returnValue = interrupt(0x16, 0, 0, 0, 0);
	

	if(returnValue == enter){
		charz[arrayValue] = 0xa;
		charz[arrayValue++] = 0x0;
		}
	else if(returnValue == backspace){
		printChar(backspace);
		printChar(255);
		printChar(backspace);

			if(arrayValue == 0){
				
				}
			else{
			  arrayValue--;
				}
		}	

	else{
		charz[arrayValue] = returnValue;
		printChar(returnValue);
		arrayValue++;
		}

	}




}
void executeProgram(char* name, int segment){
	char bufferTemp[13312];
	int i = 0;
	
	readFile(name, bufferTemp);

	for(i; i<13312; i++){

		putInMemory(segment, i, bufferTemp[i]);
	}

	launchProgram(segment);

}
void printChar(char c)
{
	char al = c;
	char ah = 0xe;
	int ax = ah*256 +al;
	interrupt(0x10,ax,0,0,0);

}
void readFile(char* filename, char* buffer){
	
	char dir[512];
	char dirEntry[32];

	int* sectors;

	int i = 0, j = 0, k = 0;

	readSector(dir, 2);

	for(i; i != 512; i++){
		if((dir[i] == filename[0]) && (dir[i+1] == filename[1])){
			for(j; j !=26; j++){
				 dirEntry[j] = dir[i+j+6];
				 
				}
		}
	}

	for(k; dirEntry[k] != 0; k++){
		readSector(buffer, dirEntry[k]);
		buffer = buffer + 512;

	}
}
void writeFile(char* name, char* buffer, int numberOfSectors){
	char map[512];
	char dir[512];
	char sBuffer[512];
	char sectorEntry[26];

	int i=0, j=0, k=0, l=0;
	int dirStart;
	int spaceFound = 0, sectorCounter = 0, post = 0;

	readSector(map, 1);
	readSector(dir, 2);

	for(i; i != 16; i++){

		if(dir[i*32] == 0x00){
			dirStart = i *32;
			spaceFound = 1;
			break;
		}
	}

	if(spaceFound == 0){
		printString("Can't find an empty location!\0");
	}

	for(j; j < 6; j++){
		if(name[j] == 0x00){
			dir[dirStart+j] = 0x00;
		}
		else{
			dir[dirStart+j] = name[j];
		}
	}

	for(k; numberOfSectors != sectorCounter; k++){
		if(map[k+4] == 0x00){
			sectorEntry[sectorCounter] = k+4;
			
			if(sectorCounter  == numberOfSectors){
				break;	
			}
			else{
				sectorCounter++;
			}
			
			map[k+4] = 0xFF;
		}
	}

	for(l; l != 26; l++){
		if(sectorCounter != post){
			dir[dirStart+6+l] = sectorEntry[post];
			
			for(i = 0; i < 512 ; i++){

				sBuffer[i] = buffer[(l+1)*i];
	
			}

			writeSector(sBuffer, sectorEntry[post]);
			post++;
		}
		else{
			dir[dirStart+6+l] = 0x00;
		}

	}
	writeSector(map, 1);
	writeSector(dir, 2);

}
void deleteFile(char* name){
	char dir[512];
	char map[512];
	char fileSectors[26];


	int i=0, j=0, k=0;
	int sector;

	readSector(dir, 2);
	readSector(map, 1);

	for(i; i != 512; i++){
		if((dir[i] == name[0]) && (dir[i+1] == name[1])){
			dir[i] = 0x00;

			for(j; j !=26; j++){
				 fileSectors[j] = dir[i+j+6];
				}
		}
	}

	for(k; fileSectors[k] != 0; k++){
		sector = fileSectors[k];

		map[sector] = 0x00;
	}

	writeSector(dir, 2);
	writeSector(map, 1);

}
void readSector(char* buffer,int sector){
		int ah=2, al=1, ch =0, cl=0, dh=0, dl=0;
		int ax=0,cx=0, dx=0;
		
	
		ch = sector/36;
		cl = mod(sector, 18) +1;
		dh = mod(div(sector, 18), 2);

		ax=ah*256+al;
		cx = ch *256+cl;
		dx = dh *256+dl;

		interrupt(0x13, ax, buffer, cx, dx);
}
void writeSector(char* buffer,int sector){
		int ah=3, al=1, ch =0, cl=0, dh=0, dl=0;
		int ax=0,cx=0, dx=0;

		ch = sector/36;
		cl = mod(sector, 18) +1;
		dh = mod(div(sector, 18), 2);

		ax=ah*256+al;
		cx = ch *256+cl;
		dx = dh *256+dl;

		interrupt(0x13, ax, buffer, cx, dx);

}
void programDir(){
	char temp[512];
	char out[512];
	char convert;

	int i=0, j=0, counter = 0, sectors = 0;
	
	readSector(temp, 2);

	for(i; i != 16; i++){
		if(temp[i*32] != 0x0){
			for(j=0; j<6; j++){
				if(temp[j+32*i] == 0x0){
				}
				else{
					out[counter] = temp[j+32*i];
					counter++;
				}
			}


			for(j=0; j<26; j++){
				if(temp[j+6+32*i] != 0x0){
				sectors++;
				}
			}

			if(sectors <= 9){
				out[counter] = ' ';
				counter++;

				convert = sectors + '0';

				out[counter] = convert;
				counter++;

			}else if(sectors <= 19){
				sectors = sectors-10;

				out[counter] = ' ';
				counter++;

				convert = sectors + '0';

				out[counter] = '1';
				counter++;
				out[counter] = convert;
				counter++;
			}else{
				sectors = sectors - 20;

				out[counter] = ' ';
				counter++;

				convert = sectors + '0';

				out[counter] = '2';
				counter++;
				out[counter] = convert;
				counter++;
			}
			sectors = 0;

			out[counter] = '\r';
			counter++;
			out[counter] = '\n';
			counter++;
		}
	}

	out[counter] = '\0';

	printString(out);

}
void handleInterrupt21(int ax, int bx, int cx, int dx){

	if(ax == 0){
		printString(bx);
	}
	else if(ax == 1){
		readString(bx);
	}
	else if(ax == 2){
		readSector(bx, cx);
	}
	else if(ax == 3){
		readFile(bx, cx);
	}
	else if(ax == 4){
		executeProgram(bx, cx);
	}
	else if(ax == 5){
		terminate();
	}
	else if(ax == 6){
		writeSector(bx, cx);
	}
	else if(ax == 7){
		deleteFile(bx);
	}
	else if(ax == 8){
		writeFile(bx, cx, dx);
	}
	else if(ax ==9){
		programDir();
	}
	else{
		printString("handleInterrupt21 error has occurred!\n"); 
	}
}
int mod(int a, int b){
	while(a >=b){
		a = a-b;
		}

	return a;	

}
int div(int a,int b){
	int q = 0;

	while(q *b <= a){
		q = q+1;
		}

	return q-1;
}
void terminate(){
	char shell[6];

	shell[0] = 's';
	shell[1] = 'h';
	shell[2] = 'e';
	shell[3] = 'l';
	shell[4] = 'l';
	shell[5] = '\0';

	executeProgram(shell, 0x2000);
	
}
