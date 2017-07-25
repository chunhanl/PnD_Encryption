/*#########################################################################
#  P & D Embedded Systems and Multimedia [H09M0a]
#  Subband-Coding 
#  
#  Mohit Dandekar
#  John O'Callaghan
#########################################################################*/


#ifndef __DSP_BITPACKER_H__
#define __DSP_BITPACKER_H__

#include "common.h"

/* Template for packer object */
typedef struct DSP_BitPack_t
{

/*
                 index(nbit)
                 
                  --------       -------------
  bank0          |  0(5)  |---->|             |
                  --------      |             | 
                  --------      |             |
  bank1          |  1(5)  |---->|             |
                  --------      |             |     <-- MSB -------------  24 bits --------------- LSB -->
         -----------------      |             |     -----------------------------------------------------
  bank2 | 2_1(4) | 2_0(4) |---->|  BitPacker  |--->| 2_1(4) | 2_0(4) |3_1(3)|   1(5)   |3_0(3)|   0(5)   |
         -----------------      |             |     -----------------------------------------------------
         -----------------      |             |
  bank3 | 3_1(3) | 3_0(3) |---->|             |
         -----------------      |             |
         -----------------      |             |
  bank4 |  empty subband  |---->|             |
         -----------------       -------------


*/



  /* Inputs from each sub-band */
  adpcm_data_t inp_band0[BLOCK_SIZE/8] ALIGN4;
  adpcm_data_t inp_band1[BLOCK_SIZE/8] ALIGN4;

  adpcm_data_t inp_band2[BLOCK_SIZE/4] ALIGN4;
  adpcm_data_t inp_band3[BLOCK_SIZE/4] ALIGN4;
  adpcm_data_t inp_band4[BLOCK_SIZE/4] ALIGN4;

  /* Delay buffers for adpcm data (bank 2) */
  adpcm_data_t delay_buffer2[BLOCK_SIZE/4 + FILT_LEN-2] ALIGN4;
  sint16 delay_buffer2_rd, delay_buffer2_wr, delay_buffer2_lim;

  /* Delay buffers for adpcm data (bank 3) */
  adpcm_data_t delay_buffer3[BLOCK_SIZE/4 + FILT_LEN-2] ALIGN4;
  sint16 delay_buffer3_rd, delay_buffer3_wr, delay_buffer3_lim;

  bitstream *out_ptr_L;
  bitstream *out_ptr_R;
}DSP_BitPack_t;

void DSP_BitPacker_init(DSP_BitPack_t *BitPack_obj,
                    bitstream *bitsout_L,
                    bitstream *bitsout_R);


void DSP_BitPacker_process(DSP_BitPack_t *BitPack_obj);

/* Template for depacker object */
typedef struct DSP_BitDepack_t
{

/*
                                                                                  index(nbit)
                                                                                  
                                                                 -------------               --------               
                                                                |             |------------>|  0(5)  |----> bank0 
                                                                |             |              --------             
                                                                |             |              --------             
                                                                |             |------------>|  1(5)  |----> bank1 
     <-- MSB -------------  24 bits --------------- LSB -->     |             |              --------             
     -----------------------------------------------------      |             |     -----------------             
--->| 2_1(4) | 2_0(4) |3_1(3)|   1(5)   |3_0(3)|   0(5)   |---->| BitDepacker |--->| 2_1(4) | 2_0(4) |----> bank2 
     -----------------------------------------------------      |             |     -----------------             
                                                                |             |     -----------------             
                                                                |             |--->| 3_1(3) | 3_0(3) |----> bank3 
                                                                |             |     -----------------             
                                                                |             |     -----------------             
                                                                |             |--->|  empty subband  |----> bank4 
                                                                 -------------      -----------------             


*/

  /* Input buffers for packed data */
  bitstream inp_buffer0[3*BLOCK_SIZE/8] ALIGN4;
  bitstream inp_buffer1[3*BLOCK_SIZE/8] ALIGN4;

  /* Pointers for ADPCM output of each sub-band */
  adpcm_data_t *out_band0_ptr  ;
  adpcm_data_t *out_band1_ptr  ;
  adpcm_data_t *out_band2_ptr  ;
  adpcm_data_t *out_band3_ptr  ;
  adpcm_data_t *out_band4_ptr  ;

}DSP_BitDepack_t;

void DSP_BitDepacker_init( DSP_BitDepack_t *BitDepack_obj,
                       adpcm_data_t *band0_out,
                       adpcm_data_t *band1_out,
                       adpcm_data_t *band2_out,
                       adpcm_data_t *band3_out,
                       adpcm_data_t *band4_out);

void DSP_BitDepacker_process(DSP_BitDepack_t *BitDepack_obj);


#endif /*__DSP_BITPACKER_H__*/
