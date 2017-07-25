/*
 * transmit.h
 *
 *  Created on: 2016/5/6
 *      Author: Leo
 */

#ifndef TRANSMIT_H_
#define TRANSMIT_H_

#define bufferSIZE 32

#include "type.h"
int queueInData(const size32_t* in);  //1 success 0 full
int recieveData(      size32_t* out); //1 success 0 empty


#endif /* TRANSMIT_H_ */
