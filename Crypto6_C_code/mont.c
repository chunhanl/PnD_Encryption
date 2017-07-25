/*#########################################################################
#  P & D Embedded Systems and Multimedia [H09M0a]
#  En/Decryption
#  
#  Chun Han Lu
#  Yi Hsuan Li
#########################################################################*/
#include<stdio.h>
#include "mont.h"


size32_t n[SIZE]={
    0xC0CC3997,0x9DF80794,0x6161FDFD,0xB47D1AF4,0x7C3FDBB4,0x676DE0F0,0xA4B11193,0x5E006AF8,0x44950C26,0x1E310081,0x2C5B67D0,0x0CF2F890,0xFF0C811F,0x0829B713,0xB830626C,
    0xF0B867ED,0xA018F2D2,0x688C5671,0xF0165B3A,0x831C0E92,0x0E5B2352,0xC9E17001,0xB4E1A4F8,0xC968BE6A,0x78A81BD2,0xB6ABD083,0x99BCEFE1,0xF7C2A749,0x7F19436D,0x9B2062AA,
    0xC3F85F9A,0x08CB7EDF,0x374932E9,0x5D5C542D,0x8031C41F,0xE3230B4F,0x89944A61,0xB1D4ABF9,0xD905C404,0x40F3955E,0x7C0A8D66,0x1D0CB4EE,0x18C685E5,0x1A49BB96,0x8861CFD8,
    0x44DBBDED,0x388D4F06,0x38C947AA,0xBE677ADF,0x1C873980,0x8750D1E4,0x12F30FC8,0x77ED29B0,0x7FA2B583,0x0BF54A68,0xAED44773,0x5E05D8D5,0x45739200,0x3A639500,0x81127240,
    0x1617CFF1,0xE37FA897,0x9F924FAA,0x91BC9105};

size32_t nprime[SIZE] = {
    0xE0E629D9,0xF9AC1018,0x04F809EA,0xEE3F9CB0,0x0D9DF156,0xEC016D5A,0xD56F2DC4,0xB2F078A2,0xFA804F00,0xEFB6865B,0x2DCAC76E,0x432C4A75,0x81C3D29B,0x707DA315,0x1E9AD262,
    0x24BDC8F7,0xCD768097,0x395A83A1,0x51AED275,0x3D01EFD7,0xCC0112D1,0x500232D8,0xDA78FB82,0x5651599B,0x0BC6FA6C,0x2CAF06EB,0x24DFA4B3,0x0DD56FF6,0xDBD7898C,0x13F5EBD4,
    0x4DFC3485,0x213DD184,0x85DF0539,0xB8F861D7,0xFC03E906,0x89911B95,0xE02F003D,0x6914A4FE,0x0C61B83C,0x1FAAF63F,0x3C01F167,0xF7CD2566,0xAAC8BA71,0xF0E13B71,0x97A33B23,
    0xC62164F6,0x2F326F4B,0xF30929DF,0xC09A3B10,0x961FF6C3,0xCBD63DED,0x032E2B46,0x49603DB6,0x3BE4F9A0,0xB7D5FF88,0x32F6AE6B,0xBD007360,0x5A48FF1E,0xC94C546E,0xEA360CC8,
    0x52A47628,0xD6305E24,0xAFCD3EA5,0xAA53FBC9};


size32_t rmodn[SIZE]={
    0x3F33C669,0x6207F86B,0x9E9E0202,0x4B82E50B,0x83C0244B,0x98921F0F,0x5B4EEE6C,0xA1FF9507,0xBB6AF3D9,0xE1CEFF7E,0xD3A4982F,0xF30D076F,0x00F37EE0,0xF7D648EC,0x47CF9D93,
    0x0F479812,0x5FE70D2D,0x9773A98E,0x0FE9A4C5,0x7CE3F16D,0xF1A4DCAD,0x361E8FFE,0x4B1E5B07,0x36974195,0x8757E42D,0x49542F7C,0x6643101E,0x083D58B6,0x80E6BC92,0x64DF9D55,
    0x3C07A065,0xF7348120,0xC8B6CD16,0xA2A3ABD2,0x7FCE3BE0,0x1CDCF4B0,0x766BB59E,0x4E2B5406,0x26FA3BFB,0xBF0C6AA1,0x83F57299,0xE2F34B11,0xE7397A1A,0xE5B64469,0x779E3027,
    0xBB244212,0xC772B0F9,0xC736B855,0x41988520,0xE378C67F,0x78AF2E1B,0xED0CF037,0x8812D64F,0x805D4A7C,0xF40AB597,0x512BB88C,0xA1FA272A,0xBA8C6DFF,0xC59C6AFF,0x7EED8DBF,
    0xE9E8300E,0x1C805768,0x606DB055,0x6E436EFA};
size32_t r2modn[SIZE]={
    0x9C4F757D,0x8253CD7B,0x39D3993E,0x1E495D1E,0x603F9E03,0x7FEF0786,0x9B1C0A8E,0xC70C5991,0xECDC9FEC,0xCD776C8A,0xC395EE43,0xC8AD9641,0xADA675FF,0x7D8A51DB,0x8D1B3FE5,
    0xFC933D10,0x39C5959C,0x1F158B22,0xF55D0DB2,0xF017B22E,0x5ABC2B10,0xD0C517D1,0xBE91980F,0xAF0F2A36,0x6E284B4C,0xD32C585F,0x2E109487,0x5889565F,0x78D7FA03,0xF58737E3,
    0x61BFAABC,0x57AA4477,0xC906F674,0xE165CCF4,0xA87CD107,0xBC33FCB6,0xC2E334CA,0x67FB6453,0x71380969,0x7858E497,0xFCD11EC5,0xDFE57B4A,0x07BF7572,0x51026390,0x711BB094,
    0xAD712CFF,0x241FE6D3,0x6DE7F771,0x73A30DFD,0x5829B2E9,0xEB5E9BBE,0xF262DC8E,0x4F13CA01,0xC1A4371B,0x412E6E57,0xFAE7FA1E,0x96752968,0xBECFE120,0xEF9BB86A,0x96F0EA2A,
    0x8E9473D5,0x77869EB3,0xC2A987B1,0x5372FB70};

size32_t one[SIZE]= {
    0x00000001,0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,
    0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,
    0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,
    0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,
    0x00000000,0x00000000,0x00000000,0x00000000};
//-----





void mp_print(const size32_t *res){
    printf("\n");
    int i;
    for(i=0;i<SIZE;i++){
        printf("%08X",res[SIZE-i-1]);
    }
    printf("\n");
}


void print32(const size32_t res){
    printf("\n");
    printf("%08X",res);
    printf("\n");
}
void print64(const size64_t res){
    printf("\n");
    printf("%08X%08X",(size32_t)(res>>32),(size32_t)res);
    printf("\n");
}
void mont( size32_t *c,const  size32_t *a,const  size32_t *b){
    
    size32_t m[SIZE]={0x00000000};
    
    
    size32_t t0 =0;
    size32_t t1 =0;
    size32_t t2 =0;
    
    size64_t s0 =0;
    size64_t s1 =0;
    
    
    size64_t i =0;
    size64_t I =0;
    size64_t j =0;
    size64_t k =0;
    
    size64_t tmp   =0;
    size64_t tmp_2 =0;
    size64_t tmp_3 =0;
    
    size32_t n0 = n[0];
    size32_t np0 = nprime[0];
    
    s_size64_t signed_i =0;
    s_size64_t signed_c =0;
    s_size64_t signed_tmp =0;
    
    size32_t S =0;
    size32_t m_tmp =0;
    
    
    for(i=0;i<SIZE;i++){
        k =i;
        for(j=0;j<i;j++){
            tmp=(size64_t)t0+(size64_t)a[j]*(size64_t)b[k];
            //===ADD==== ADD(t,1,C,SIZE_T);
            tmp_2 = (size64_t)t1+(tmp>>32);
            t1= tmp_2;
            t2 = (size64_t)t2+(tmp_2>>32);
            //==========
            
            
            tmp_3=(tmp&0xFFFFFFFF)+(size64_t)m[j]*(size64_t)n[k];
            t0=tmp_3;
            
            
            //===ADD==== ADD(t,1,C,SIZE_T);
            tmp_2 = (size64_t)t1+(tmp_3>>32);
            t1=tmp_2;
            t2 = t2+(tmp_2>>32);
            //==========
            k--;
            
        }
        
        tmp=(size64_t)t0+(size64_t)a[i]*(size64_t)b[0];
        S=tmp;
        
        //===ADD==== ADD(t,1,C,SIZE_T);
        tmp_2 = (size64_t)t1+(tmp>>32);
        t1=tmp_2;
        t2 = (size64_t)t2+(tmp_2>>32);
        //========================================
        
        //m[i] =S*nprime[0];
        //tmp=S+m[i]*n[0];
        m_tmp =(size64_t)S*(size64_t)np0;
        tmp=(size64_t)S+(size64_t)m_tmp*(size64_t)n0;
        m[i]=m_tmp;
        
        S=tmp;
        //===ADD==== ADD(t,1,C,SIZE_T);
        tmp_2 = (size64_t)t1+(tmp>>32);
        t1=tmp_2;
        t2 = (size64_t)t2+(tmp_2>>32);
        //========================================
        
        t0=t1;
        t1=t2;
        t2=0;
        
        
    }
    
    /* printf("\n=========\n");
     printf("a:\n");
     mp_print(a);
     printf("b:\n");
     mp_print(b);
     printf("m:\n");
     mp_print(m);
     printf("\n=========\n");*/
    
    s0=SIZE+SIZE;
    s1=SIZE-1;
    
    for(I=SIZE;I<s0;I++){
        
        for(j=I-s1;j<SIZE;j++){
            k= I-j;
            tmp=(size64_t)t0+(size64_t)a[j]*(size64_t)b[k];
            //===ADD==== ADD(t,1,C,SIZE_T);
            tmp_2 = (size64_t)t1+(tmp>>32);
            t1=tmp_2;
            t2 = t2+(tmp_2>>32);
            //========================================
            
            
            
            tmp_3=(tmp&0xFFFFFFFF)+(size64_t)m[j]*(size64_t)n[k];
            t0=tmp_3;
            //===ADD==== ADD(t,1,C,SIZE_T);
            tmp_2 = (size64_t)t1+(tmp_3>>32);
            t1=tmp_2;
            t2 = t2+(tmp_2>>32);
            //========================================
            
        }
        
        m[I-SIZE]=t0;
        t0=t1;
        t1=t2;
        t2=0;
        
    }
    
    for(signed_i=SIZE-1;signed_i>0;signed_i--){
        if(m[signed_i]>n[signed_i]){
            //Subtraction
            signed_tmp = 0;
            signed_c=0;
            for(i=0;i<=(SIZE-1);i++){
                signed_tmp =(size64_t)m[i]-(size64_t)n[i]+signed_c;
                m[i] = signed_tmp;
                if(signed_tmp>=0)signed_c=0;
                else signed_c=-1;
                
            }
            break;
        }
        else if(m[signed_i]<n[signed_i])
            break;
    }
    
    
    for(i=0;i<SIZE;i++)c[i]=m[i];
}





void mont_exp( size32_t *cipher,const size32_t *message,const size32_t *e,const int EbitNum){
    
    size32_t xbar[SIZE]={0x00000000};
    
    size32_t A[SIZE]= {0x00000000};
    
    size32_t arrayCt=0;
    size32_t flag=0;
    
    mont(xbar,message,r2modn);
    int i;
    for(i=0;i<SIZE;i++)A[i]=rmodn[i];
    
    
    int aryCt = (EbitNum-1)/32;
    int bitCt = EbitNum;
    
    for(i=bitCt;i>0;i--){
        mont(A,A,A);
        
        arrayCt = e[(i-1)/32];
        flag = (arrayCt>>((i-1)%32))&0x01; //8= 1byte
        
        if(flag){
            mont(A,A,xbar);
        }
        
        //printf("MontExp Progressing [ %1f %% ]\r",((float)i/(float)EbitNum)*100);
        
        //printf("\n%d flag:%d ",i,flag!=0);mp_print(A);
    }
    
    mont(A,A,one);
    for(i=0;i<SIZE;i++)cipher[i]=A[i];
}



