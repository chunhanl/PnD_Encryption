/*#########################################################################
#  P & D Embedded Systems and Multimedia [H09M0a]
#  En/Decryption
#  
#  Chun Han Lu
#  Yi Hsuan Li
#########################################################################*/

#ifndef MONT_H
#define MONT_H

#include "type.h"


void mp_print(const size32_t *res);
void mont( size32_t *c,const size32_t *a,const size32_t *b);
void mont_exp( size32_t *cipher,const size32_t *message,const size32_t  *e,const int EbitNum);



#endif
