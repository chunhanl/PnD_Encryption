/*#########################################################################
#  P & D Embedded Systems and Multimedia [H09M0a]
#  En/Decryption
#  
#  Chun Han Lu
#  Yi Hsuan Li
#########################################################################*/
#include "sha_convert.h"
#include "sha2.h"
#include <string.h>
#include <stdio.h>



void OS2IP(uint8_t * inputstr){
    uint8_t count = 0;
    int i;
    for(i = 0 ;i < 64; i++){
        if((int)inputstr[i]>=(int)'0'&&(int)inputstr[i]<=(int)'9'){
            if(i%2==0){
                inputstr[count] = (int)inputstr[i]-(int)'0';
                //printf("hihi");
            }
            else{
                inputstr[count] = (int)inputstr[i]-(int)'0'+inputstr[count]*16;
                count = count+1;
            }
        }
        else{
            if(i%2==0){
                inputstr[count] = (int)inputstr[i]-(int)'a'+10;
                //printf("yaya");
            }
            else{
                inputstr[count] = (int)inputstr[i]-(int)'a'+10+inputstr[count]*16;
                count = count+1;
            }

        }
        
    }

    for(i = 32 ;i < 256; i++)inputstr[i]=0;  //!! SHA256 only 32 BYTE
}
/*
void sha256(uint8_t * buf){
    	SHA256_CTX	ctx256;
    	SHA256_Init(&ctx256);
    	SHA256_Update(&ctx256, (unsigned char*)buf, 16384);
    	SHA256_End(&ctx256, buf);
	OS2IP(buf);
}*/

void sha256(uint8_t * buf,int size){
	int i=0;
	/*printf("\nBefore SHA2:\n");
	for(i=0 ;i<256;i++)printf("%02X",buf[i]);*/

    	SHA256_CTX	ctx256;
    	SHA256_Init(&ctx256);
    	SHA256_Update(&ctx256, buf, size);
    	SHA256_End(&ctx256, buf);
	/*printf("\nAfter SHA2:%s\n",buf);*/
	OS2IP(buf);

	/*printf("\nAfter SHA2:\n");
	for(i=0 ;i<256;i++)printf("%02X",buf[i]);*/
}
