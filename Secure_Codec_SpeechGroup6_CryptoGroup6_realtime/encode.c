/*#########################################################################
#  P & D Embedded Systems and Multimedia [H09M0a]
#  En/Decryption
#  
#  Chun Han Lu
#  Yi Hsuan Li
#########################################################################*/


#include <stdio.h>
#include <stdlib.h>
#include "encode.h"
#include <stdint.h>
#include <string.h>
#include "digest-info.h"
#include "sha_convert.h"


void encode(size32_t* EM_out ,const size32_t* keyA,const size32_t*keyB){
    int i;
	int emLen = 256;
	int TLen = 51;
	unsigned char EM[256];
	unsigned char T[51];//	unsigned char T[TLen];
    	//unsigned int BUFLEN = 16384;
    	unsigned char buf[512]={0};

        for(i=0;i<64;++i)
        {
          buf[  0 + i*4] = keyA[i];
          buf[  1 + i*4] = (keyA[i]>>8);
          buf[  2 + i*4] = (keyA[i]>>16);
          buf[  3 + i*4] = (keyA[i]>>24);

          buf[256 + i*4] = keyB[i];
          buf[257 + i*4] = (keyB[i]>>8);
          buf[258 + i*4] = (keyB[i]>>16);
          buf[259 + i*4] = (keyB[i]>>24);
        }


    	sha256(buf,512);
	//
	
	int count = 0;
	for(i=0;i<TLen;i++){
		if(i<19)
			T[i] = sha256_prefix[i];
		else{
			T[i] = buf[count];
			count = count + 1;
		}
	}
	//
	uint8_t PSLen = emLen-TLen-2;
    	EM[0] = 0x01;
    	count = 0;
    	for(i =1 ;i<emLen;i++){
        	if(i<PSLen+1)
            		EM[i] = 0xFF;
        	else if(i<PSLen+2)
            		EM[i] = 0x00;
        	else{
            		EM[i] = T[count];
            		count = count +1;
        	}
    	}

	

	for(i = 0;i<64;i++){
		EM_out[i]  =EM[i*4] + (EM[i*4+1]<<8)+(EM[i*4+2]<<16)+(EM[i*4+3]<<24);
	}

	
}
