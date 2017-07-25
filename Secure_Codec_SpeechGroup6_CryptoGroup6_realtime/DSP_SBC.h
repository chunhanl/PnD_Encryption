/*#########################################################################
#  P & D Embedded Systems and Multimedia [H09M0a]
#  Subband-Coding 
#  
#  Mohit Dandekar
#  John O'Callaghan
#########################################################################*/

#ifndef __DSP_SBC_H__
#define __DSP_SBC_H__

#include "common.h"
#include "DSP_QMF.h"

typedef struct DSP_SBC_AFB_t
{
/*
                                     -------------> (band0)      
                                    |
                       ------------>| (bank000)
                      |             |       
           ---------->| (bank00)     -------------> (band1)
          |           |
          |            ---------------------------> (band2)
    ----->| (bank0)
          |            ---------------------------> (band4)
          |           |             
           ---------->| (bank01)
                      |
                       ---------------------------> (band3)
*/


   /*Constituent QMF banks in the tree*/
	DSP_QMFA_bank_t QMF_bank0;   /* instantiate the structure for bank0*/
	DSP_QMFA_bank_t QMF_bank00;  /* instantiate the structure for bank00*/
	DSP_QMFA_bank_t QMF_bank01;  /* instantiate the structure for bank01*/
	DSP_QMFA_bank_t QMF_bank000; /* instantiate the structure for bank000*/

   aud_data_t *inp_buffer; /* points to the input buffer of QMF_bank0*/

   sint32 inp_blk_len;
}DSP_SBC_AFB_t;


void DSP_SBC_AFB_init( DSP_SBC_AFB_t *SBC_AFB_obj,
                   aud_data_t *out_buffer0_0_0, /* should point to the band0 encoder input buffer*/
                   aud_data_t *out_buffer0_0_1, /* should point to the band1 encoder input buffer*/
                   aud_data_t *out_buffer0_1,   /* should point to the band2 encoder input buffer*/
                   aud_data_t *out_buffer1_0,   /* should point to the band4 encoder input buffer*/
                   aud_data_t *out_buffer1_1,   /* should point to the band3 encoder input buffer*/
                   sint16 *filter,
                   sint32 inp_block_len);

void DSP_SBC_AFB_process( DSP_SBC_AFB_t *SBC_AFB_obj);


typedef struct DSP_SBC_SFB_t
{
/*
   (band0) ----->|
                 |
        (bank000)|-------->|
                 |         |
   (band1) ----->| (bank00)|---------->|
                           |           |
   (band2) --------------->|           |
                                (bank0)|-------> 
   (band4) --------------->|           |
                           |           |
                   (bank01)|---------->|
                           |          
   (band3) --------------->|

*/
	DSP_QMFS_bank_t QMF_bank000; /*Instantiate QMF synthesis bank000*/
	DSP_QMFS_bank_t QMF_bank00;  /*Instantiate QMF synthesis bank00*/
	DSP_QMFS_bank_t QMF_bank01;  /*Instantiate QMF synthesis bank01*/
	DSP_QMFS_bank_t QMF_bank0;   /*Instantiate QMF synthesis bank0*/
 
  aud_data_t *in_buffer0_0_0; /*points to input buffer0 of bank000*/
  aud_data_t *in_buffer0_0_1; /*points to input buffer1 of bank000*/
  aud_data_t *in_buffer0_1;   /*points to input buffer1 of bank00*/
  aud_data_t *in_buffer1_0;   /*points to input buffer0 of bank01*/
  aud_data_t *in_buffer1_1;   /*points to input buffer1 of bank01*/
 
  sint32 out_blk_len;
   
}DSP_SBC_SFB_t;


void DSP_SBC_SFB_init( DSP_SBC_SFB_t *SBC_SFB_obj,
                   aud_data_t *data_out,
                   sint16 *filter,
                   sint32 out_block_len);
                 
void DSP_SBC_SFB_process( DSP_SBC_SFB_t *SBC_SFB_obj);

#endif /*__DSP_SBC_H__*/
