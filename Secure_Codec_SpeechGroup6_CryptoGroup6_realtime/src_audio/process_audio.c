#include "process_audio.h"

/* Include your header files here */
#include<string.h>
#include<stdio.h>
#include<stdlib.h>

/* Declare your variables globally here */

#include "Encrypt.h"
#include "DSP_ADPCM_SBC.h"



static aud_data_t out_buffer_dut[BLOCK_SIZE] ALIGN4;

DSP_ADPCM_SBCoder_t ADPCM_SBCoder_dut;
DSP_ADPCM_SBDecoder_t ADPCM_SBDecoder_dut;


/*Temporary buffers to store the encoded bitstream*/
bitstream bit_buffer_toencrypt[2][128] ALIGN4; // 2 channels 3 bits per sample average for 24kbps. Buffer allocation size to fit 2048 bit input for crypto

unsigned char AESkey[16] ALIGN4;
unsigned char IV[16] ALIGN4;
unsigned char AESMAC[16] ALIGN4;

bitstream bit_buffer_encrypted[256] ALIGN4; // 2 channels 3 bits per sample average for 24kbps. Buffer allocation size to fit 2048 bit input for crypto

bitstream bit_buffer_decrypted[256] ALIGN4; // 2 channels 3 bits per sample average for 24kbps. Buffer allocation size to fit 2048 bit input for crypto



void init_audio(){

    /* Include all your initialization code. This function is called once only */

	DSP_ADPCM_SBCoder_init(&ADPCM_SBCoder_dut,
			bit_buffer_toencrypt[0],
			bit_buffer_toencrypt[1],
	                       BLOCK_SIZE);

	DSP_ADPCM_SBDecoder_init(&ADPCM_SBDecoder_dut,
		                       out_buffer_dut,
		                       BLOCK_SIZE);


	  printf("\n\n Key Establishment \n\n");
	                 Initialize(AESkey, IV);
	                 printf("\n\n Key Establishment Done !!!\n\n");

	printf("Init done\n");


}


// Multi-iteration version



void process_audio(int* input_buf, int* output_buf, int nbytes)
{
  int i;
  /*----------------------------- SUBBAND ENCODER  ----------------------------*/
               /*Writing into the subband coder input buffer*/
  for(i=0;i<(nbytes/4);++i)
  {

	  ADPCM_SBCoder_dut.inp_buffer[i].l =  _ext(input_buf[i],16,16);
	  ADPCM_SBCoder_dut.inp_buffer[i].r =   _ext(input_buf[i], 0,16);

  }

  DSP_ADPCM_SBCoder_process(&ADPCM_SBCoder_dut);
  /*---------------------------------------------------------------------------*/

  /*-----------------------------    ENCRYPTION    ----------------------------*/

  AES128_Encrypt( bit_buffer_encrypted  , AESMAC, (unsigned char *)(&bit_buffer_toencrypt[0][0]) , AESkey, IV);

  /*---------------------------------------------------------------------------*/


  /*-----------------------------    DECRYPTION    ----------------------------*/

  AES128_Decrypt( bit_buffer_decrypted  , AESMAC, bit_buffer_encrypted , AESkey, IV);

  /*---------------------------------------------------------------------------*/



  /*---------------------------- SUBBAND DECODER  -----------------------------*/
                /*Writing into the subband decoder input buffer*/
  for(i=0;i<3*(nbytes/4)/8;++i)
  {
    ADPCM_SBDecoder_dut.inp_buffer_L[i] =   bit_buffer_decrypted[  0 + i];
    ADPCM_SBDecoder_dut.inp_buffer_R[i] =   bit_buffer_decrypted[128 + i];
  }


  DSP_ADPCM_SBDecoder_process(&ADPCM_SBDecoder_dut);

  /*---------------------------------------------------------------------------*/



  /*Reading data out to stereo output buffer*/
  for(i=0;i<(nbytes/4);++i)
  {

	 output_buf[i] = _pack2(out_buffer_dut[i].r,out_buffer_dut[i].l);

  }

}






