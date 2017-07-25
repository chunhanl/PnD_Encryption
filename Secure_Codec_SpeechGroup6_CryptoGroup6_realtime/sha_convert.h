/*#########################################################################
#  P & D Embedded Systems and Multimedia [H09M0a]
#  En/Decryption
#  
#  Chun Han Lu
#  Yi Hsuan Li
#########################################################################*/

#ifndef SHA_CONVERT_H
#define SHA_CONVERT_H
#include <stdint.h>
void OS2IP(uint8_t * inputstr);
void sha256(uint8_t * buf,int size);
#endif
