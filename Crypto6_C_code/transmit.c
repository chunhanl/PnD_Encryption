/*#########################################################################
#  P & D Embedded Systems and Multimedia [H09M0a]
#  En/Decryption
#  
#  Chun Han Lu
#  Yi Hsuan Li
#########################################################################*/
#include "transmit.h"
#include <stdio.h>

size32_t buffer[bufferSIZE]={0x00000000};
size8_t spaceLeft = bufferSIZE;
size8_t dataCount = 1;

int queueInData(const size32_t* in){
	if(spaceLeft !=0){
		buffer[dataCount-1] = *in;
		dataCount++;
		spaceLeft--;
		return 1;
	}
	else{
		printf("Transmit Buffer is Full !!\n");
		return 0;
	}
}

int recieveData(size32_t* out){
	if(dataCount !=0){
		(*out) =buffer[dataCount-1];
		dataCount--;
		spaceLeft++;
		return 1;
	}
	else{
		printf("No data in Transmit Buffer !!\n");
		return 0;
	}
}
