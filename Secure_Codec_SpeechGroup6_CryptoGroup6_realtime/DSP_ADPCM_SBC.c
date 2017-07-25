/*#########################################################################
#  P & D Embedded Systems and Multimedia [H09M0a]
#  Subband-Coding 
#  
#  Mohit Dandekar
#  John O'Callaghan
#########################################################################*/

#include "DSP_ADPCM_SBC.h"


static sint16 analysis_filter[40] ALIGN4 = {   17,   -31,   -17,    64,    28,
                               -135,   -29,   256,     2,  -442,
                                 78,   716,  -255, -1121,   619,
                               1783, -1432, -3257,  4332, 15209,
                              15209,  4332, -3257, -1432,  1783,
                                619, -1121,  -255,   716,    78,
                               -442,     2,   256,   -29,  -135,
                                 28,    64,   -17,   -31,    17};

static sint16 synthesis_filter[40] ALIGN4 = {   34,   -61,   -34,   128,    56,
                                -271,   -58,   512,     5,  -885,
                                 155,  1432,  -510, -2242,  1237,
                                3565, -2865, -6513,  8664, 30419,
                               30419,  8664, -6513, -2865,  3565,
                                1237, -2242,  -510,  1432,   155,
                                -885,     5,   512,   -58,  -271,
                                  56,   128,   -34,   -61,    34};  


		
static sint16 opt_mu[5] ALIGN4 = {13222, 13956, 13120, 13160, 16361};



void DSP_ADPCM_SBCoder_init( DSP_ADPCM_SBCoder_t *ADPCM_SBCoder_obj,
                         bitstream *bitsout_L,
                         bitstream *bitsout_R,
                         sint32 inp_block_len)
{

  DSP_BitPacker_init( &(ADPCM_SBCoder_obj->BitPack),
                  bitsout_L,
                  bitsout_R );


 
  DSP_ADPCM_enc_init( &(ADPCM_SBCoder_obj->ADPCM_ENC_band0),
                  ADPCM_SBCoder_obj->BitPack.inp_band0,
                  opt_mu[0],
                  5,
                  inp_block_len/8); 

  DSP_ADPCM_enc_init( &(ADPCM_SBCoder_obj->ADPCM_ENC_band1),
                  ADPCM_SBCoder_obj->BitPack.inp_band1,
                  opt_mu[1],
                  5,
                  inp_block_len/8); 

  DSP_ADPCM_enc_init( &(ADPCM_SBCoder_obj->ADPCM_ENC_band2),
                  ADPCM_SBCoder_obj->BitPack.inp_band2,
                  opt_mu[2],
                  4,
                  inp_block_len/4); 

  DSP_ADPCM_enc_init( &(ADPCM_SBCoder_obj->ADPCM_ENC_band3),
                  ADPCM_SBCoder_obj->BitPack.inp_band3,
                  opt_mu[3],
                  3,
                  inp_block_len/4); 

  DSP_ADPCM_enc_init( &(ADPCM_SBCoder_obj->ADPCM_ENC_band4),
                  ADPCM_SBCoder_obj->BitPack.inp_band4,
                  opt_mu[4],
                  0,
                  inp_block_len/4); 


  DSP_SBC_AFB_init( &(ADPCM_SBCoder_obj->SBC_AFB),
                ADPCM_SBCoder_obj->ADPCM_ENC_band0.data_in_buffer,
                ADPCM_SBCoder_obj->ADPCM_ENC_band1.data_in_buffer,
                ADPCM_SBCoder_obj->ADPCM_ENC_band2.data_in_buffer,
                ADPCM_SBCoder_obj->ADPCM_ENC_band4.data_in_buffer,
                ADPCM_SBCoder_obj->ADPCM_ENC_band3.data_in_buffer,
                analysis_filter,
                inp_block_len);

  ADPCM_SBCoder_obj->inp_buffer = ADPCM_SBCoder_obj->SBC_AFB.inp_buffer;

                  
  return;
}


void DSP_ADPCM_SBCoder_process( DSP_ADPCM_SBCoder_t *ADPCM_SBCoder_obj)
{

	DSP_SBC_AFB_process( &(ADPCM_SBCoder_obj->SBC_AFB));

	DSP_ADPCM_enc_process( &(ADPCM_SBCoder_obj->ADPCM_ENC_band0));
	DSP_ADPCM_enc_process( &(ADPCM_SBCoder_obj->ADPCM_ENC_band1));
	DSP_ADPCM_enc_process( &(ADPCM_SBCoder_obj->ADPCM_ENC_band2));
	DSP_ADPCM_enc_process( &(ADPCM_SBCoder_obj->ADPCM_ENC_band3));
	DSP_ADPCM_enc_process( &(ADPCM_SBCoder_obj->ADPCM_ENC_band4));

	DSP_BitPacker_process( &(ADPCM_SBCoder_obj->BitPack));
  return;
}


void DSP_ADPCM_SBDecoder_init(DSP_ADPCM_SBDecoder_t *ADPCM_SBDecoder_obj,
                          aud_data_t *data_out_ptr,
                          sint32 out_block_len)
{


	DSP_SBC_SFB_init( &(ADPCM_SBDecoder_obj->SBC_SFB),
                data_out_ptr,
                synthesis_filter,
                out_block_len);

	DSP_ADPCM_dec_init( &(ADPCM_SBDecoder_obj->ADPCM_DEC_band0),
                  ADPCM_SBDecoder_obj->SBC_SFB.in_buffer0_0_0,
                  opt_mu[0],
                  5,
                  out_block_len/8);

	DSP_ADPCM_dec_init( &(ADPCM_SBDecoder_obj->ADPCM_DEC_band1),
                  ADPCM_SBDecoder_obj->SBC_SFB.in_buffer0_0_1,
                  opt_mu[1],
                  5,
                  out_block_len/8);

	DSP_ADPCM_dec_init( &(ADPCM_SBDecoder_obj->ADPCM_DEC_band2),
                  ADPCM_SBDecoder_obj->SBC_SFB.in_buffer0_1,
                  opt_mu[2],
                  4,
                  out_block_len/4);

	DSP_ADPCM_dec_init( &(ADPCM_SBDecoder_obj->ADPCM_DEC_band3),
                  ADPCM_SBDecoder_obj->SBC_SFB.in_buffer1_1,
                  opt_mu[3],
                  3,
                  out_block_len/4);

	DSP_ADPCM_dec_init( &(ADPCM_SBDecoder_obj->ADPCM_DEC_band4),
                  ADPCM_SBDecoder_obj->SBC_SFB.in_buffer1_0,
                  opt_mu[4],
                  0,
                  out_block_len/4);


	DSP_BitDepacker_init( &(ADPCM_SBDecoder_obj->BitDepack),
                    ADPCM_SBDecoder_obj->ADPCM_DEC_band0.data_in_buffer,
                    ADPCM_SBDecoder_obj->ADPCM_DEC_band1.data_in_buffer,
                    ADPCM_SBDecoder_obj->ADPCM_DEC_band2.data_in_buffer,
                    ADPCM_SBDecoder_obj->ADPCM_DEC_band3.data_in_buffer,
                    ADPCM_SBDecoder_obj->ADPCM_DEC_band4.data_in_buffer);

  ADPCM_SBDecoder_obj->inp_buffer_L = ADPCM_SBDecoder_obj->BitDepack.inp_buffer0;
  ADPCM_SBDecoder_obj->inp_buffer_R = ADPCM_SBDecoder_obj->BitDepack.inp_buffer1;
  

  return;
}

void DSP_ADPCM_SBDecoder_process( DSP_ADPCM_SBDecoder_t *ADPCM_SBDecoder_obj)
{

	DSP_BitDepacker_process( &(ADPCM_SBDecoder_obj->BitDepack));

	DSP_ADPCM_dec_process( &(ADPCM_SBDecoder_obj->ADPCM_DEC_band0));
	DSP_ADPCM_dec_process( &(ADPCM_SBDecoder_obj->ADPCM_DEC_band1));
	DSP_ADPCM_dec_process( &(ADPCM_SBDecoder_obj->ADPCM_DEC_band2));
	DSP_ADPCM_dec_process( &(ADPCM_SBDecoder_obj->ADPCM_DEC_band3));
	DSP_ADPCM_dec_process( &(ADPCM_SBDecoder_obj->ADPCM_DEC_band4));

	DSP_SBC_SFB_process( &(ADPCM_SBDecoder_obj->SBC_SFB));

  return;
}


