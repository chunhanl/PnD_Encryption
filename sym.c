#include <stdio.h>
#include <stdlib.h>
#include "aes.h"
#include <string.h>
#include <time.h>  //for time random
#include "type.h"
//sha2
#include <string.h>



#include "sha_convert.h"


void AES128_Encrypt2048(unsigned char* cipher,unsigned char* MAC,const unsigned char* message,const unsigned char*sharekey,const unsigned char* IV){
	size8_t i,j;
   	size32_t k;


    unsigned char m[16][16];
	for(i=0;i<16;i++){
		for(j=0;j<16;j++){			
			m[i][j]=message[i*16+j];
		}
	}

    	//------key 16byte 128bit------
	aes_key key;
	aes_set_encrypt_key(&key, sharekey , 128);
    	//-----------------------------

 
    	unsigned char m_XOR[16][16]; //cant be delete by replacing m
    	unsigned char c[16][16]; //ciphers
  
    
    	//----------XOR------
    	const unsigned char* XOR_data;
    	for(i=0;i<16;i++){
       		XOR_data=(i==0)?IV:c[i-1];
       		for(j=0;j<16;j++){
        	    m_XOR[i][j]=XOR_data[j]^m[i][j];
        	}
    	//------encrypt message 16byte 128bit------
        	aes_encrypt(&key, m_XOR[i], c[i]);
    	//-----------------------------------------
//        	printf("cipher %d:",i);for(k=0;k<16;k++)printf("%02X ",c[i][k]);printf("\n");
    	}
    	//--------------------
       
    

    	for(i=0;i<16;i++)
    	    for(j=0;j<16;j++)
		cipher[i*16+j]=c[i][j];
  
    
    
    
    	//==================SHA256========================
    	//unsigned int BUFLEN = 16384;
    	unsigned char   buf[256]={0};
       	for(k=0;k<256;k++)buf[k]=cipher[k];

    	sha256(buf,256);
	for(i=0;i<16;i++)MAC[i]=buf[i];
    	//==================SHA256========================
}  
    
    
    

int AES128_Decrypt2048(unsigned char* message,const unsigned char* MAC,const unsigned char* cipher,const unsigned char*sharekey,const unsigned char*IV){   
	size8_t i,j;
	size32_t k;
    	//==================SHA256========================
    	//unsigned int BUFLEN = 16384;
    	unsigned char   buf[256]={0};
	for(k=0;k<256;k++)buf[k]=cipher[k];
    	sha256(buf,256);
	int flag=0;
	for(i=0;i<16;i++){
		if(MAC[i]!=buf[i]){
			flag=1;
			break;
		}
	}
	
	if(flag==1)return 0;



   	//==========DECRYPT===========
    	unsigned char d[16][16];
	unsigned char c[16][16];
	for(i=0;i<16;i++)
		for(j=0;j<16;j++)
		c[i][j]=cipher[i*16+j];

	/*for(i=0;i<16;i++){
		printf("rec_cipher %d:",i);
	 	for(k=0;k<16;k++)
			printf("%x ",c[i][k]);printf("\n");
    	}*/
	

 	aes_key key;
 	aes_set_decrypt_key(&key, sharekey, 128);
    
	const unsigned char *XOR_data;
    	for(i=0;i<16;i++){
        	XOR_data=(i==15)?IV:c[15-i-1];
        	aes_decrypt(&key, c[15-i] , d[15-i]);
        	for(j=0;j<16;j++){
            	d[15-i][j]=XOR_data[j]^d[15-i][j];
        	}
    	}
/*    
	for(i=0;i<16;i++){
    	    printf("dec_cipher %d:",i);
    	    for(k=0;k<16;k++)printf("%x ",d[i][k]);printf("\n");
    	}
*/
    	//==========DECRYPT===========
    	for(i=0;i<16;i++)
    	    for(j=0;j<16;j++)
		message[i*16+j]=d[i][j];


	return 1;
}



int AES128_Check2048(unsigned char* message,const unsigned char* decrypt ){
	size32_t i=0;
    	for(i=0;i<256;i++){
        	if(message[i]!=decrypt[i]){
			return 0;
        	}
    	}
	return 1;
}

