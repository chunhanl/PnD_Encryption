/*#########################################################################
#  P & D Embedded Systems and Multimedia [H09M0a]
#  Subband-Coding
#
#  Mohit Dandekar
#  John O'Callaghan
#########################################################################*/

#include "DSP_ADPCM.h"

/* Step multipliers stored in q2.13 format */
static sint16 stepTable2[2] ALIGN4 = {6553, 13107};
static sint16 stepTable3[4] ALIGN4 = {7372, 7372, 12288, 22528};
static sint16 stepTable4[8] ALIGN4 = {7372, 7372, 7372, 7372, 9830, 13107, 16384, 19660};
static sint16 stepTable5[16] ALIGN4 = {7372, 7372, 7372, 7372, 7782, 7782, 7782, 7782, 9830,
                         12288, 14745, 17203, 19660, 22118, 24576, 27033};

/*(1/x) taylor series approximation terms in range 0.5-1 in q15*/
                       /* q2.13 */
static sint16 one_by_x0[32] ALIGN4 = {16132, 15650, 15197, 14769, 14364, 13981, 13618, 13273,
                       12945, 12633, 12336, 12053, 11782, 11523, 11275, 11038,
                       10810, 10592, 10382, 10180, 9986, 9800, 9620, 9447, 
                       9279, 9118, 8962, 8812, 8666, 8525, 8389, 8257};  

                          /* q2.13 */
static sint16 one_by_x0sq[32] ALIGN4  = {31768, 29899, 28191, 26625, 25186, 23861, 22637, 21506,
                           20457, 19483, 18577, 17733, 16945, 16208, 15518, 14872, 
                           14265, 13694, 13157, 12651, 12174, 11723, 11297, 10893,
                           10511, 10149, 9805, 9478, 9167, 8872, 8590, 8322};


#if 0
static inline void RECIP(uint32 *dout,
                  uint32 *scr,
                  uint32 *scl,
                  uint32 din)
{
  register sint32 sr,indxr,sl,indxl ;
  register uint32 deltal,deltar;
  register sint32 xinl = _ext(din,16,16);
  register sint32 xinr = _ext(din,0,16);

  /*Normalize value to 0.5 - 1 in q15*/
  sr = _norm(xinr)-16; sl = _norm(xinl)-16;


  xinr = (xinr << sr); xinl = (xinl << sl);
  indxl = ((xinl - 16384)>>9); /*5 bit index*/
  indxr = ((xinr - 16384)>>9); /*5 bit index*/
  deltal = (xinl & ((1<<9)-1)) - (1<<8);/*q15*/
  deltar = (xinr & ((1<<9)-1)) - (1<<8);/*q15*/

  *dout = _pack2((one_by_x0[indxr] - ((one_by_x0sq[indxr]*deltar + 16384)>>15)) , (one_by_x0[indxl] - ((one_by_x0sq[indxl]*deltal + 16384)>>15))); /*output in (S+2).(13-S) format*/
  *scr = sr;
  *scl = sl;

  return;
}
#endif


void DSP_ADPCM_enc_init( DSP_ADPCM_enc_t *ADPCM_enc_obj,
                         adpcm_data_t *data_out,
                         sint16 mu,
                         sint8 nbit,
                         sint16 blk_len)
{
  /* Connecting output pointer to output buffer */
  ADPCM_enc_obj->data_out_ptr = data_out;

  /* Copying mu, nbit and block length */
  ADPCM_enc_obj->mu = mu;
  ADPCM_enc_obj->nbit = nbit;
  ADPCM_enc_obj->blk_len = blk_len;

  /* Initialising chunk / history variables */
  ADPCM_enc_obj->step.l = 16384;
  ADPCM_enc_obj->step.r = 16384;
  ADPCM_enc_obj->z_N.l = 0;
  ADPCM_enc_obj->z_N.r = 0;
  ADPCM_enc_obj->x_star.l = 0;
  ADPCM_enc_obj->x_star.r = 0;

  /* Connecting stepTable pointer to correct table depending on number of output ADPCM bits */
  if (nbit == 2){
    ADPCM_enc_obj->stepTable_ptr = stepTable2;
  }else if (nbit == 3){
    ADPCM_enc_obj->stepTable_ptr = stepTable3;
  }else if (nbit == 4){
    ADPCM_enc_obj->stepTable_ptr = stepTable4;
  }else if (nbit == 5){
    ADPCM_enc_obj->stepTable_ptr = stepTable5;
  }

return;
}

void DSP_ADPCM_enc_process(DSP_ADPCM_enc_t *ADPCM_enc_obj)
{
  sint32 bufPos; /* Iterator for position in buffer */

  register uint32 step, x_star, d, d_dash, inv_step, t0, stepTableVal, limu,limn; /* 16 bit stereo variables */
  register sint32 wl,wr;
  register uint32 indx,sl,sr;


  uint32 * restrict data_ptr = (uint32 *)ADPCM_enc_obj->data_in_buffer; /* Connect pointer for input */
  adpcm_data_t * restrict data_out_ptr = ADPCM_enc_obj->data_out_ptr; /* Connect pointer for adpcm output */

  uint16 * restrict stepTable = (uint16 *)ADPCM_enc_obj->stepTable_ptr; /* Connect pointer for stepTable */

  /* Copy required variables from encoder object */
  sint16 nbit = ADPCM_enc_obj->nbit;
  register uint32 mu = _pack2((sint32)ADPCM_enc_obj->mu, (sint32)ADPCM_enc_obj->mu);
  sint16 blk_len = ADPCM_enc_obj->blk_len;

  /* Copy history from chunk variables */
  step   = _pack2((sint32)ADPCM_enc_obj->step.r,   (sint32)ADPCM_enc_obj->step.l);
  x_star = _pack2((sint32)ADPCM_enc_obj->x_star.r, (sint32)ADPCM_enc_obj->x_star.l);

  /* Calculate initial d' = step * previous quantized output */
  t0 = _pack2((sint32)ADPCM_enc_obj->z_N.r, (sint32)ADPCM_enc_obj->z_N.l);
  wl = _mpy(step, t0); wr = _mpyh(step, t0);
  d_dash = _pack2(wr,wl);


  limu = _pack2(((1<<(nbit-1))-1), ((1<<(nbit-1))-1));
  limn = _pack2(-((1<<(nbit-1))), -((1<<(nbit-1))));
  /* Set initial index for stepTable lookup based on previous quantised output */
  indx = _max2(_min2(_abs2(t0),limu),0);

  /* If no bits assigned to this bank, return empty data out without further processing */
  if(nbit == 0)
  {
      for(bufPos=0;bufPos<blk_len;++bufPos)
      {
        data_out_ptr[bufPos].l = 0;
        data_out_ptr[bufPos].r = 0;
      }
    return; 
  }

  for(bufPos=0;bufPos<blk_len;++bufPos)
    {
      /* Predictor: x* = mu( x* + d' )    */
      t0 = _add2(x_star, d_dash);//DADD(t0,x_star,d_dash); /* (x* + d') */
      wl = _mpy(t0,mu); wr = _mpyh(t0,mu);
      x_star = _pack2(((wr + 16384)>>15),((wl+16384)>>15)); /* Pack result stored in accumulator to Q15 */

      /* Prediction error: d = x - x*     */
      register uint32 data = data_ptr[bufPos];
      d = _sub2(data, x_star);//      DSUB(d,data_ptr[bufPos],x_star);

      /* Step: step = step * stepTableVal */
      stepTableVal = _pack2(stepTable[_ext(indx,0,16)], stepTable[_ext(indx,16,16)]); /* Right/Left Channel step multiplier (Q2.13) */

      wl = _mpy(step,stepTableVal); wr = _mpyh(step,stepTableVal);
      step = _pack2((wr+4096)>>13 , (wl+4096)>>13); /* step * stepTableVal (Q2.13) */



      /* Calculate step reciprocal, inverse step */
       register sint32 indxr,indxl ;
       register uint32 deltal,deltar;
       register sint32 xinl = _ext(step,16,16);
       register sint32 xinr = _ext(step,0,16);

       /*Normalize value to 0.5 - 1 in q15*/
       sr = _norm(xinr)-16; sl = _norm(xinl)-16;

       xinr = (xinr << sr); xinl = (xinl << sl);
       indxl = ((xinl - 16384)>>9); /*5 bit index*/
       indxr = ((xinr - 16384)>>9); /*5 bit index*/
       deltal = (xinl & ((1<<9)-1)) - (1<<8);/*q15*/
       deltar = (xinr & ((1<<9)-1)) - (1<<8);/*q15*/

       inv_step = _pack2((one_by_x0[indxr] - ((one_by_x0sq[indxr]*deltar + 16384)>>15)) , (one_by_x0[indxl] - ((one_by_x0sq[indxl]*deltal + 16384)>>15))); /*output in (S+2).(13-S) format*/


      /* Quantizer: Output = d * inv_step */
      wl = _mpy(d, inv_step); wr = _mpyh(d, inv_step);
      t0 = _pack2(((wr + (1<<(15+13-sr-1)))>>(15+13-sr)),((wl + (1<<(15+13-sl-1)))>>(15+13-sl)));


      /* Saturate / limit output depending on nbit */
      t0 = _max2(_min2(t0,limu),limn);


      /* Write result to output pointer */
      //data_out_ptr[bufPos] = (uint16)(_packl4(t0,t0)&0x0000ffff);

      data_out_ptr[bufPos].l = (sint8)_ext(t0,16,16);
      data_out_ptr[bufPos].r = (sint8)_ext(t0,0,16);

      /* Set index of step multiplier for next iteration based on new quantized output */
      indx = _max2(_min2(_abs2(t0),limu),0);


      /* Dequantizer: d' = step * quantized output */
      wl = _mpy(step, t0); wr = _mpyh(step, t0);
      d_dash = _pack2(wr,wl);

    }

  /* Write history to chunk variables in encoder object */

  ADPCM_enc_obj->step.l = (sint16)(_ext(step,16,16));
  ADPCM_enc_obj->step.r = (sint16)(_ext(step,0,16));

  ADPCM_enc_obj->z_N.l = (sint8)(_ext(t0,16,16));
  ADPCM_enc_obj->z_N.r = (sint8)(_ext(t0,0,16));


  ADPCM_enc_obj->x_star.l = (sint16)(_ext(x_star,16,16));
  ADPCM_enc_obj->x_star.r = (sint16)(_ext(x_star,0,16));

  return;
}


void DSP_ADPCM_dec_init( DSP_ADPCM_dec_t *ADPCM_dec_obj,
                     aud_data_t *data_out,
                     sint16 mu,
                     sint8 nbit,
                     sint16 blk_len)
{
  /* Connecting output pointer to output buffer */
  ADPCM_dec_obj->data_out_ptr = data_out;

  /* Copying mu, nbit and block length */
  ADPCM_dec_obj->mu = mu;
  ADPCM_dec_obj->nbit = nbit;
  ADPCM_dec_obj->blk_len = blk_len;

  /* Initialising chunk / history variables */
  ADPCM_dec_obj->step.l = 16384;
  ADPCM_dec_obj->step.r = 16384;
  ADPCM_dec_obj->z_N.l = 0;
  ADPCM_dec_obj->z_N.r = 0;
  ADPCM_dec_obj->x_star.l = 0;
  ADPCM_dec_obj->x_star.r = 0;

  /* Connecting stepTable pointer to correct table depending on number of output ADPCM bits */
  if (nbit == 2){
    ADPCM_dec_obj->stepTable_ptr = stepTable2;
  }else if (nbit == 3){
    ADPCM_dec_obj->stepTable_ptr = stepTable3;
  }else if (nbit == 4){
    ADPCM_dec_obj->stepTable_ptr = stepTable4;
  }else if (nbit == 5){
    ADPCM_dec_obj->stepTable_ptr = stepTable5;
  }

return;
}


void DSP_ADPCM_dec_process(DSP_ADPCM_dec_t *ADPCM_dec_obj)
{
  sint32 bufPos; /* Iterator for position in buffer */

  aud_data_t step, x_star, d_dash, t0, stepTableVal, zin, out;  /* 16 bit stereo variables */
  aud_data_w_t accum0; /* Accumulator for mul results */
  sint16 indxl,indxr;

  adpcm_data_t *data_ptr = ADPCM_dec_obj->data_in_buffer; /* Connect pointer for adpcm input */
  aud_data_t *data_out_ptr = ADPCM_dec_obj->data_out_ptr; /* Connect pointer for output */

  sint16 *stepTable = ADPCM_dec_obj->stepTable_ptr; /* Connect pointer for stepTable */

  /* Copy required variables from encoder object */
  sint16 nbit = ADPCM_dec_obj->nbit;
  sint16 mu = ADPCM_dec_obj->mu;
  sint16 blk_len = ADPCM_dec_obj->blk_len;

  /* Copy history from chunk variables */
  step = ADPCM_dec_obj->step;
  x_star = ADPCM_dec_obj->x_star;

  /* Calculate initial d' = step * previous quantized output */
  DMUL(accum0,step,ADPCM_dec_obj->z_N);
  DPACKL(d_dash,accum0);

  /* Set initial index for stepTable lookup based on previous quantised output */
  SATN(indxl, abs(ADPCM_dec_obj->z_N.l),(0),((1<<(nbit-1))-1));
  SATN(indxr, abs(ADPCM_dec_obj->z_N.r),(0),((1<<(nbit-1))-1));

  /* If no bits assigned to this bank, return empty data out without further processing */
  if(nbit == 0)
  {
      for(bufPos=0;bufPos<blk_len;++bufPos)
      {
        data_out_ptr[bufPos].l = 0;
        data_out_ptr[bufPos].r = 0;
      }
    return; 
  }

  for(bufPos=0;bufPos<blk_len;++bufPos)
    {

      /* Predictor: x* = mu( x* + d' )    */
      DADD(t0,x_star,d_dash);
      DSMUL(accum0,t0,mu);
      DPACKV(x_star,accum0,15,15); /* Pack result stored in accumulator to Q15 */

      /* Step: step = step * stepTableVal */
      stepTableVal.l = stepTable[indxl];  /* Left Channel step multiplier (Q2.13) */
      stepTableVal.r = stepTable[indxr];  /* Right Channel step multiplier (Q2.13) */ 

      DMUL(accum0,step,stepTableVal); /* step * stepTableVal (Q2.13) */
      DPACKV(step,accum0,13,13);   

      /* Copy quantised data to variable */
      zin.l = data_ptr[bufPos].l;     
      zin.r = data_ptr[bufPos].r;     

      /* Dequantizer: d' = step * quantized data */
      DMUL(accum0,step,zin);
      DPACKL(d_dash,accum0);

      /* Output = x* + d' */
      DADD(out,x_star,d_dash);
      data_out_ptr[bufPos] = out;

      /* Set index of step multiplier for next iteration based on new quantized output */
      SATN(indxl, abs(zin.l),(0),((1<<(nbit-1))-1));
      SATN(indxr, abs(zin.r),(0),((1<<(nbit-1))-1));
    }

    /* Write history to chunk variables in encoder object */
    ADPCM_dec_obj->step = step;
    ADPCM_dec_obj->z_N.l = zin.l;
    ADPCM_dec_obj->z_N.r = zin.r;
    ADPCM_dec_obj->x_star = x_star;

  return;
}
