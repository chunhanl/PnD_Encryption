/*#########################################################################
#  P & D Embedded Systems and Multimedia [H09M0a]
#  Subband-Coding 
#  
#  Mohit Dandekar
#  John O'Callaghan
#########################################################################*/

#ifndef __DSP_ADPCM_SBC_H__
#define __DSP_ADPCM_SBC_H__

#include "common.h"
#include "DSP_SBC.h"
#include "DSP_ADPCM.h"
#include "DSP_BitPacker.h"



typedef struct DSP_ADPCM_SBCoder_t
{
/*                                         ----------
              |------->|coder band0|----->|          |
         -----|------->|coder band1|----->|          |
   ---->| AFB |------->|coder band2|----->| BitPacker|------>          
         -----|------->|coder band4|----->|          |
              |------->|coder band3|----->|          |
                                           ----------
*/

	DSP_SBC_AFB_t SBC_AFB;  /*Instantiate the analysis filterbank object*/

	DSP_ADPCM_enc_t ADPCM_ENC_band0;/*instantiate the coder band0*/
	DSP_ADPCM_enc_t ADPCM_ENC_band1;/*instantiate the coder band1*/
	DSP_ADPCM_enc_t ADPCM_ENC_band2;/*instantiate the coder band2*/
	DSP_ADPCM_enc_t ADPCM_ENC_band3;/*instantiate the coder band3*/
	DSP_ADPCM_enc_t ADPCM_ENC_band4;/*instantiate the coder band4*/
   
	DSP_BitPack_t BitPack; /*instantiate the bitpacker*/

   aud_data_t *inp_buffer; 

   sint32 blk_len; 
}DSP_ADPCM_SBCoder_t;

void DSP_ADPCM_SBCoder_init( DSP_ADPCM_SBCoder_t *ADPCM_SBCoder_obj,
                         bitstream *bitsout_L,
                         bitstream *bitsout_R,
                         sint32 inp_block_len);

void DSP_ADPCM_SBCoder_process( DSP_ADPCM_SBCoder_t *ADPCM_SBCoder_obj);



typedef struct DSP_ADPCM_SBDecoder_t
{

/* 
        -------------                             ----                    
       |             |----->|decoder band0|----->|    \
       |             |----->|decoder band1|----->|     \
  ---->| BitDepacker |----->|decoder band2|----->| SFB  ------>          
       |             |----->|decoder band4|----->|     /
       |             |----->|decoder band3|----->|    /
        -------------                             ----            
*/

	DSP_SBC_SFB_t SBC_SFB;

	DSP_ADPCM_dec_t ADPCM_DEC_band0;
	DSP_ADPCM_dec_t ADPCM_DEC_band1;
	DSP_ADPCM_dec_t ADPCM_DEC_band2;
	DSP_ADPCM_dec_t ADPCM_DEC_band3;
	DSP_ADPCM_dec_t ADPCM_DEC_band4;

	DSP_BitDepack_t BitDepack;

   bitstream *inp_buffer_L; 
   bitstream *inp_buffer_R; 

   sint32 blk_len; 
}DSP_ADPCM_SBDecoder_t;

void DSP_ADPCM_SBDecoder_init(DSP_ADPCM_SBDecoder_t *ADPCM_SBDecoder_obj,
                          aud_data_t *data_out_ptr,
                          sint32 out_block_len);

void DSP_ADPCM_SBDecoder_process( DSP_ADPCM_SBDecoder_t *ADPCM_SBDecoder_obj);




#endif /*__DSP_ADPCM_SBC_H__*/
