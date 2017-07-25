/*#########################################################################
#  P & D Embedded Systems and Multimedia [H09M0a]
#  En/Decryption
#  
#  Chun Han Lu
#  Yi Hsuan Li
#########################################################################*/
#ifndef ENCRYPT_H
#define ENCRYPT_H



void Initialize(unsigned char* AESkey,unsigned char* IV);
void AES128_Encrypt(unsigned char* cipher ,      unsigned char* MAC,const unsigned char* message,const unsigned char*sharekey,const unsigned char*IV);   //for public use
void AES128_Decrypt(unsigned char* message,const unsigned char* MAC,const unsigned char* cipher ,const unsigned char*sharekey,const unsigned char*IV);   //for public use
int  AES128_Check(unsigned char* message,const unsigned char* decrypt );   


#endif
