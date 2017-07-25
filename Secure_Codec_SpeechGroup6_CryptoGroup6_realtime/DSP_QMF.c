/*#########################################################################
#  P & D Embedded Systems and Multimedia [H09M0a]
#  Subband-Coding 
#  
#  Mohit Dandekar
#  John O'Callaghan
#########################################################################*/

#include "DSP_QMF.h"



void DSP_QMFA_bank_init( DSP_QMFA_bank_t *QMFA_bank_obj,
                     aud_data_t *data_out_LO,
                     aud_data_t *data_out_HI,
                     sint16 *filter,
                     sint32 block_len )
{
  sint32 i,j,N;
  aud_data_t *dptr, *dptr0, *dptr1;

  /* Connecting the output pointers to out buffers*/
  QMFA_bank_obj->data_out_LO_ptr = data_out_LO;
  QMFA_bank_obj->data_out_HI_ptr = data_out_HI;

  /* Connecting the filter pointer to the filter array*/
  QMFA_bank_obj->flt_ptr = filter;

  /*Copying Block length*/
  QMFA_bank_obj->blk_len = block_len;


  N = BLOCK_SIZE ; 
  /*Initializing the input data buffer array to 0*/
  dptr = QMFA_bank_obj->data_in_buffer;
  for(i=0;i<N;++i)
  {
    dptr[i].l = 0; 
    dptr[i].r = 0; 
  }

  N = BLOCK_SIZE/2 + FILT_LEN/2-1;
  /*Initializing histrory arrays to 0*/ 
  dptr0 = QMFA_bank_obj->history[0];
  dptr1 = QMFA_bank_obj->history[1];
  for(i=0;i<N;++i)
  {
    dptr0[i].l = 0;
    dptr0[i].r = 0;
    dptr1[i].l = 0;
    dptr1[i].r = 0;
  }

  uint32 L = block_len/2;
  uint32 M = FILT_LEN/2;
  for(i=0;i<L;++i)
  {
	  sint16 count = 0;
	  for(j=0; ((j<=i)&&(j<M)); j++) count++;

	  QMFA_bank_obj->iter_count[i] = count;
  }

  for(i=L;i<L+M-1;++i)
  {
 	 sint16 count = 0;
 	 for(j=(i-L+1); ((j<=i)&&(j<M)); j++) count++;

 	QMFA_bank_obj->iter_count[i] = count;
  }


  return;
}

void DSP_QMFA_process(DSP_QMFA_bank_t *QMFA_bank_obj)
{
  sint32 i,j;  

  uint32 * restrict filter = (uint32 *)QMFA_bank_obj->flt_ptr;
  uint32 * restrict data_ptr = (uint32 *)(QMFA_bank_obj->data_in_buffer + 0); /*input */

  uint32 L = QMFA_bank_obj->blk_len/2;
  uint32 M = FILT_LEN/2;
  uint32 * restrict hist_rd_ptr0 = (uint32 *)QMFA_bank_obj->history[0];
  uint32 * restrict hist_rd_ptr1 = (uint32 *)QMFA_bank_obj->history[1];
  uint32 * restrict hist_wr_ptr0 = (uint32 *)QMFA_bank_obj->history[0];
  uint32 * restrict hist_wr_ptr1 = (uint32 *)QMFA_bank_obj->history[1];

  uint32 * restrict data_out_lo_ptr = (uint32 *)QMFA_bank_obj->data_out_LO_ptr;
  uint32 * restrict data_out_hi_ptr = (uint32 *)QMFA_bank_obj->data_out_HI_ptr;

  for(i=0;i<L;++i)
  {
    /*Iterations 0 to (L-1) produce the current block of output*/


    register uint32 o0,o1,out0,out1;

    register __int40_t accum0l,accum1l,accum0r,accum1r; /*accumulators for polyphase filter commponent 0 and 1*/
    register long long temp;

    accum0l = 0;accum0r = 0;
    accum1l = 0;accum1r = 0;
    uint32 *dptr = data_ptr + 2*i + 1;
    uint32 *fptr = filter;

    #pragma MUST_ITERATE( 1)
    for(j=0;j<QMFA_bank_obj->iter_count[i];++j)
    {
     /* Limit for j is beginning at the rightmost overlap 
        position to the leftmost overlap position*/
      register uint32 d0,d1;

      register uint32 filt_coef10 = *fptr++;  /*Loads the decimated filter coeficients*/
      register uint32 filt_coef0 = _packh2(filt_coef10,filt_coef10);  /*in the correct order i.e. with reversal*/
      register uint32 filt_coef1 = _pack2(filt_coef10,filt_coef10);  /*in the correct order i.e. with reversal*/

      d1 = *dptr--;//data_ptr1[(2*(i-j))];  /*load the data right to left with 1:2 split*/
      d0 = *dptr--;//data_ptr0[(2*(i-j))];  /*load the data right to left with 1:2 split*/


      temp = _mpy2ll(d0, filt_coef0);
      accum0l = _lsadd(_loll(temp), accum0l); accum0r = _lsadd(_hill(temp), accum0r);

      temp = _mpy2ll(d1, filt_coef1);
      accum1l = _lsadd(_loll(temp), accum1l); accum1r = _lsadd(_hill(temp), accum1r);

    }

    o0 = _pack2( _sat((accum0r + 16384)>>15), _sat((accum0l + 16384)>>15));
    o1 = _pack2( _sat((accum1r + 16384)>>15), _sat((accum1l + 16384)>>15));
    o0 = _add2(o0, hist_rd_ptr0[i]);  /*add history sample filter 0 overlap add*/
    o1 = _add2(o1, hist_rd_ptr1[i]);  /*add history sample filter 1 overlap add*/

    out0 = _add2(o0,o1) ;
    data_out_lo_ptr[i] = out0; /*out bank0 is out0 + out1*/
    out1 = _sub2(o0,o1) ;
    data_out_hi_ptr[i] = out1; /*out bank1 is out0 - out1*/
  }

  for(i=L;i<L+M-1;++i)
  {

    /*Iterations L to (L + (M-1) -1) produce the 
      history for overlap add for next block*/

	  register uint32 o0,o1;

	  register __int40_t accum0l,accum1l,accum0r,accum1r; /*accumulators for polyphase filter commponent 0 and 1*/
	  register long long temp;

	  accum0l = 0;accum0r = 0;
	  accum1l = 0;accum1r = 0;

    uint32 *dptr = data_ptr + 2*(L-1) + 1;
    uint32 *fptr = filter + (i-L+1);

    #pragma MUST_ITERATE( 1)

    for(j=0;j<QMFA_bank_obj->iter_count[i];++j)
    {
     /*same logic for j starting at the 
       rightmost point of overlap to leftmost*/
        register sint32 d0,d1;

        register sint32 filt_coef10 = *fptr++;;  /*Loads the decimated filter coeficients*/
        register sint32 filt_coef0 = _packh2(filt_coef10,filt_coef10);  /*in the correct order i.e. with reversal*/
        register sint32 filt_coef1 = _pack2(filt_coef10,filt_coef10);  /*in the correct order i.e. with reversal*/

        d1 = *dptr--;  /*load the data right to left with 1:2 split*/
        d0 = *dptr--;  /*load the data right to left with 1:2 split*/


        temp = _mpy2ll(d0, filt_coef0);
        accum0l = _lsadd(_loll(temp), accum0l); accum0r = _lsadd(_hill(temp), accum0r);

        temp = _mpy2ll(d1, filt_coef1);
        accum1l = _lsadd(_loll(temp), accum1l); accum1r = _lsadd(_hill(temp), accum1r);
    }

    o0 = _pack2( _sat((accum0r + 16384)>>15), _sat((accum0l + 16384)>>15));
    o1 = _pack2( _sat((accum1r + 16384)>>15), _sat((accum1l + 16384)>>15));

    o0 = _add2(o0, hist_rd_ptr0[i]);  /*add history sample filter 0 overlap add*/
    o1 = _add2(o1, hist_rd_ptr1[i]);  /*add history sample filter 1 overlap add*/

    hist_wr_ptr0[(i-L)] = o0; /* write out overlap add history history filter 0*/
    hist_wr_ptr1[(i-L)] = o1; /* write out overlap add history history filter 1*/
  }

  return;
}



void DSP_QMFS_bank_init( DSP_QMFS_bank_t *QMFS_bank_obj,
                     aud_data_t *data_out,
                     sint16 *filter,
                     sint32 block_len)
{
  sint32 i,j;
  aud_data_t *dptr0, *dptr1;

  /*Connecting output pointers to output buffers*/
  QMFS_bank_obj->data_out_ptr = data_out;
 
  /*Connecting filter pointer to filter*/
  QMFS_bank_obj->flt_ptr = filter;

  QMFS_bank_obj->blk_len = block_len;

  /*Initializing input buffer arrays to 0*/
  dptr0 = QMFS_bank_obj->data_in_buffer_LO;
  dptr1 = QMFS_bank_obj->data_in_buffer_HI;
  for(i=0;i<BLOCK_SIZE/2;++i)
  {
    dptr0[i].l = 0;
    dptr0[i].r = 0;
    dptr1[i].l = 0;
    dptr1[i].r = 0;
  } 

  /*Initializing overlap add history arrays to 0*/
  dptr0 = QMFS_bank_obj->history[0];
  dptr1 = QMFS_bank_obj->history[1];
  for(i=0;i<BLOCK_SIZE/2+FILT_LEN/2-1;++i)
  {
    dptr0[i].l = 0;
    dptr0[i].r = 0;
    dptr1[i].l = 0;
    dptr1[i].r = 0;
  }


  sint32 L = block_len;
  sint32 M = FILT_LEN/2;
  for(i=0;i<L;++i)
  {
	  sint16 count = 0;
	  for(j=0; ((j<=i)&&(j<M)); j++) count++;

	  QMFS_bank_obj->iter_count[i] = count;
  }

  for(i=L;i<L+M-1;++i)
  {
 	 sint16 count = 0;
 	 for(j=(i-L+1); ((j<=i)&&(j<M)); j++) count++;

 	QMFS_bank_obj->iter_count[i] = count;
  }
  return;
}



void DSP_QMFS_process(DSP_QMFS_bank_t *QMFS_bank_obj)
{
  sint32 i,j;  

  uint32 * restrict filter = (uint32 *)QMFS_bank_obj->flt_ptr;
  uint32 * restrict data_ptr0 = (uint32 *)QMFS_bank_obj->data_in_buffer_LO; /*input buffer bank0*/
  uint32 * restrict data_ptr1 = (uint32 *)QMFS_bank_obj->data_in_buffer_HI; /*input buffer bank0*/

  sint32 L = QMFS_bank_obj->blk_len;
  sint32 M = FILT_LEN/2;
  uint32 * restrict hist_rd_ptr0 = (uint32 *)QMFS_bank_obj->history[0];
  uint32 * restrict hist_rd_ptr1 = (uint32 *)QMFS_bank_obj->history[1];
  uint32 * restrict hist_wr_ptr0 = (uint32 *)QMFS_bank_obj->history[0];
  uint32 * restrict hist_wr_ptr1 = (uint32 *)QMFS_bank_obj->history[1];

  uint32 * restrict dataout_ptr = (uint32 *)QMFS_bank_obj->data_out_ptr;
 
  for(i=0;i<L;++i)
  {
    /* iteration 0 to (L-1) produces the L outputs of the current block*/
    register uint32 t0,t1,out;
    register __int40_t accum0l, accum1l, accum0r, accum1r;
    register long long temp;

    accum0l = 0; accum0r = 0;
    accum1l = 0; accum1r = 0;

    uint32 *dptr0 = data_ptr0 + i ;
    uint32 *dptr1 = data_ptr1 + i ;

    uint32 *fptr = filter;

    #pragma MUST_ITERATE( 1)

    for(j=0; j<QMFS_bank_obj->iter_count[i];++j)
    {
     /*j indexes from the point of rightmost overlap to the leftmost overlap position */
      register uint32 d0,d1;

      register uint32 filt_coef10 = *fptr++;  /*Loads the decimated filter coeficients*/
      register uint32 filt_coef0 = _pack2(filt_coef10,filt_coef10);  /*in the correct order i.e. with reversal*/
      register uint32 filt_coef1 = _packh2(filt_coef10,filt_coef10);  /*in the correct order i.e. with reversal*/

      d1 = *dptr1--;//data_ptr1[(2*(i-j))];  /*load the data right to left with 1:2 split*/
      d0 = *dptr0--;//data_ptr0[(2*(i-j))];  /*load the data right to left with 1:2 split*/

      t0 = _add2(d0,d1);
      t1 = _sub2(d0,d1);

      temp = _mpy2ll(t0, filt_coef0);
      accum0l = _lsadd(_loll(temp), accum0l); accum0r = _lsadd(_hill(temp), accum0r);

      temp = _mpy2ll(t1, filt_coef1);
      accum1l = _lsadd(_loll(temp), accum1l); accum1r = _lsadd(_hill(temp), accum1r);

    }

    t0 = _pack2( _sat((accum0l + 16384)>>15), _sat((accum0l + 16384)>>15));
    t1 = _pack2( _sat((accum1l + 16384)>>15), _sat((accum1l + 16384)>>15));

    out = _add2(t0, hist_rd_ptr0[i]);
    dataout_ptr[2*i + 0] = out;      /*filter 0 produces the odd output*/

    out = _add2(t1, hist_rd_ptr1[i]);
    dataout_ptr[2*i + 1] = out;      /*filter 1 produces the even output*/

  }
 
  for(i=L;i<L+M-1;++i)
  {
	register uint32 t0,t1,out;
	register __int40_t accum0l, accum1l, accum0r, accum1r;
	register long long temp;

    accum0l = 0; accum0r = 0;
    accum1l = 0; accum1r = 0;

    uint32 *dptr0 = data_ptr0 + (L-1) ;
    uint32 *dptr1 = data_ptr1 + (L-1) ;

    uint32 *fptr = filter + (i-L+1);

    #pragma MUST_ITERATE( 1)

    for(j=0; j<QMFS_bank_obj->iter_count[i];++j)
    {

      /*same logic for j starting at the 
       rightmost point of overlap to leftmost*/

      register sint32 d0,d1;

      register sint32 filt_coef10 = *fptr++;;  /*Loads the decimated filter coeficients*/
      register sint32 filt_coef0 = _pack2(filt_coef10,filt_coef10);  /*in the correct order i.e. with reversal*/
      register sint32 filt_coef1 = _packh2(filt_coef10,filt_coef10);  /*in the correct order i.e. with reversal*/

      d1 = *dptr1--;  /*load the data right to left with 1:2 split*/
      d0 = *dptr0--;  /*load the data right to left with 1:2 split*/

      t0 = _add2(d0,d1);
      t1 = _sub2(d0,d1);

      temp = _mpy2ll(t0, filt_coef0);
      accum0l = _lsadd(_loll(temp), accum0l); accum0r = _lsadd(_hill(temp), accum0r);

      temp = _mpy2ll(t1, filt_coef1);
      accum1l = _lsadd(_loll(temp), accum1l); accum1r = _lsadd(_hill(temp), accum1r);

    }

    t0 = _pack2( _sat((accum0l + 16384)>>15), _sat((accum0l + 16384)>>15));
    t1 = _pack2( _sat((accum1l + 16384)>>15), _sat((accum1l + 16384)>>15));

    out = _add2(t0, hist_rd_ptr0[i]);
    hist_wr_ptr0[(i-L)] = out;         /* write out overlap add history history filter 0*/
    out = _add2(t1, hist_rd_ptr1[i]);
    hist_wr_ptr1[(i-L)] = out;         /* write out overlap add history history filter 1*/
  }

  return;
}


