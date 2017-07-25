/*#########################################################################
#  P & D Embedded Systems and Multimedia [H09M0a]
#  En/Decryption
#  
#  Chun Han Lu
#  Yi Hsuan Li
#########################################################################*/
#ifndef SYM_H
#define SYM_H

void AES128_Encrypt2048(unsigned char* cipher ,      unsigned char* MAC,const unsigned char* message,const unsigned char*sharekey,const unsigned char*IV);   //for initialize() use

// return 0 MAC invalid /retun 1 MAC valid
int AES128_Decrypt2048(unsigned char* message,const unsigned char* MAC,const unsigned char* cipher ,const unsigned char*sharekey,const unsigned char*IV);   //for initialize() use
int AES128_Check2048(unsigned char* message,const unsigned char* decrypt );   //for initialize() use
#endif
