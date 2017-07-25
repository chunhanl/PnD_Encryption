/*#########################################################################
#  P & D Embedded Systems and Multimedia [H09M0a]
#  Subband-Coding 
#  
#  Mohit Dandekar
#  John O'Callaghan
#########################################################################*/

#ifndef __DSP_QMF_H__
#define __DSP_QMF_H__

#include <c6x.h>

#include "common.h"

/* Template of QMF bank object*/
typedef struct DSP_QMFA_bank_t
{
  /*scratch/history buffer*/
  aud_data_t history[2][BLOCK_SIZE/2 + FILT_LEN/2 -1] ALIGN4;

  /*Input Buffer to be written into by the previous stage*/
  aud_data_t data_in_buffer[BLOCK_SIZE] ALIGN4;
  /*TODO : QMF at increasing depth in the tree have smaller block size, but the
           memory allocation is fixed to worse case
  */ 

  aud_data_t *data_out_LO_ptr; /* pointer to data out LO branch*/
  aud_data_t *data_out_HI_ptr; /* pointer to data out HI branch*/

  sint16 *flt_ptr; /*pointer to filter*/

  sint32 blk_len;

  sint16 iter_count[BLOCK_SIZE/2+FILT_LEN/2-1] ALIGN4;

}DSP_QMFA_bank_t;


void DSP_QMFA_bank_init( DSP_QMFA_bank_t *QMFA_bank_obj,
                     aud_data_t *data_out_LO, 
                     aud_data_t *data_out_HI,
                     sint16 *filter,
                     sint32 block_len );


void DSP_QMFA_process(DSP_QMFA_bank_t *QMFA_bank_obj);



/* Template of QMF bank object*/
typedef struct DSP_QMFS_bank_t
{
  /*scratch/history buffer*/
  aud_data_t history[2][BLOCK_SIZE/2 + FILT_LEN/2 -1] ALIGN4;

  /*Input Buffer to be written into by the previous stage*/
  aud_data_t data_in_buffer_LO[BLOCK_SIZE/2] ALIGN4;

  aud_data_t data_in_buffer_HI[BLOCK_SIZE/2] ALIGN4;
  /*TODO : QMF at increasing depth in the tree have smaller block size, but the
           memory allocation is fixed to worse case
  */ 

  aud_data_t *data_out_ptr; /* pointer to data out */

  sint16 *flt_ptr; /*pointer to filter*/

  sint32 blk_len;

  sint16 iter_count[BLOCK_SIZE/2+FILT_LEN/2-1] ALIGN4;
}DSP_QMFS_bank_t;


void DSP_QMFS_bank_init( DSP_QMFS_bank_t *QMFS_bank_obj,
                     aud_data_t *data_out,
                     sint16 *filter,
                     sint32 block_len);

void DSP_QMFS_process(DSP_QMFS_bank_t *QMFS_bank_obj);

#endif /*__QMF_H__*/
