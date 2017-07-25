


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <time.h>  //for time random

#include <string.h>


#include "sym.h"
#include "mont.h"
#include "digest-info.h"
#include "encode.h"
#include "Encrypt.h"
#include "sha_convert.h"



size32_t g[SIZE]={
    0x00000002,0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,
    0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,
    0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,
    0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,
    0x00000000,0x00000000,0x00000000,0x00000000};//Private Key of A

size32_t keyA[SIZE]={
    0xEEA51451,0x8FE114F1,0x6279F48C,0x114CF4DF,0x73B33016,0xE6F9FFCB,0x2688AAFC,0x960156CD,0xE937C881,0x1D5A52F0,0xCB74113D,0xC67D029E,0x015F0E05,0xC5FE9F06,0x8BF64A1C,
    0xB4CC8572,0x0BBB4953,0xB9D0FBAC,0xE9E600F5,0xDE82AB66,0x1F456774,0x40CDB805,0x17D4F81A,0x6D1F7F21,0x5EBB4E61,0x3CFFE15C,0xCFF07E37,0x3788EFE4,0xE33648CF,0x8CE7C24A,
    0x9056B6AC,0xED9000AF,0xB3D3EB1C,0x82BBEC15,0x33E87960,0xDDAC5023,0x5A1C75F5,0x3F2C1DF6,0xD7ED5C6E,0x0FFAF03F,0x1CA16341,0x9BF549E6,0x36EA4E0E,0x89749863,0xD2F4D3B5,
    0x9FA5ACA4,0xC7791DF3,0x2A38924C,0x8214EF24,0x073DE0EA,0xB774A311,0x8A445AA5,0x095BE5E0,0x933793F1,0x577A6A05,0x1E967A45,0x346AEABF,0xF5A64301,0x4A02C831,0xE8B3D157,
    0xC9CFC4D2,0xB608C560,0x3FC1AFBF,0x52CABFF5};

//Private Key of B
size32_t keyB[SIZE]={
    0x2DCDB89D,0x484B8418,0x6D3CAB9D,0x13138D90,0xEA174D2C,0x402516C7,0x26195B1A,0xB936BCC7,0x95DDCF23,0x21374CCF,0xC6A1376E,0x0281F9A0,0xD3A55DBB,0xB66A02F9,0x765F98CF,
    0x11848B8C,0xDA9B90B5,0xFC1FCE67,0x81B7000C,0x3BA384DE,0x3A140DE1,0x0FB1A88C,0xDE7E0349,0x4296B0B0,0x89FC4674,0x61AF81BF,0x03D0B632,0x675C6250,0xB3A47AE6,0xC871A045,
    0x8543683D,0x2EAA6FBD,0xA53E6D9D,0xFC74F9E7,0xE9E527FD,0x359DA51E,0xB304E9C6,0xA2B6175F,0xFBFF2A8E,0x589F7D97,0xC2788017,0xA3D4A7F5,0xBC7EC564,0x97817261,0x6F87A341,
    0x9B77DE61,0xD1C309CA,0x20EF5E0D,0xBCC9BB34,0x13C602A4,0x1EAAC076,0xC2FB518D,0x1C323027,0xD6F427C0,0x7E136908,0xAEEAC18D,0xC8E604CB,0xFF0CB992,0xF0811B66,0x9DCE7639,
    0xD63624A9,0xD458E141,0xCF899F89,0x805F0C1A};


size32_t modofA[SIZE]={0x00000000};


size32_t modofB[SIZE]={0x00000000};



void DH_Process(size32_t *sharedPrivateKey){
	//========DH station to station Process============================================================================================================================================

	mont_exp(modofA,g,keyA,2048);
	printf("\nMod from A(g^keyA mod n):\n");
	mp_print(modofA);
	printf("\n");

	mont_exp(modofB,g,keyB,2048);
	printf("\nMod from B(g^keyB mod n)::\n");
	mp_print(modofB);
	printf("\n");

	mont_exp(sharedPrivateKey,modofB,keyA,2048);
	printf("\nKey generated @A:\n");
	mp_print(sharedPrivateKey);
	printf("\n");

	mont_exp(sharedPrivateKey,modofA,keyB,2048);
	printf("\n\n\n\nKey generated @B:\n");
	mp_print(sharedPrivateKey);
	printf("\n");
	printf("DH station to station Process Done");

	//========DH station to station Process============================================================================================================================================

}

void KDF(const size32_t *sharedPrivateKey,unsigned char *AESkey,unsigned char *AES_IV){
	//==================KDF  create public key & Initial Vector=========================================================================================================================

    	unsigned char buf2[256]={0};
    	unsigned short i;
   	for(i=0;i<64;i++){
    		buf2[i*4]  =(sharedPrivateKey[i]);
    		buf2[i*4+1]=(sharedPrivateKey[i]>>8);
    		buf2[i*4+2]=(sharedPrivateKey[i]>>16);
    		buf2[i*4+3]=(sharedPrivateKey[i]>>24);
    	}
    	sha256(buf2,256);

	for(i=0 ;i<16;i++){AES_IV[i] =buf2[i];}
	for(i=16;i<32;i++){AESkey[i] =buf2[i];}

	printf("Symmetric Key by KDF Done");

	//==================KDF  create public key & Initial Vector=========================================================================================================================


}

void DigitalSign_Create(size32_t *Signature_A){
	//==================Digital Signature _Create=========================================================================================================================
    	//==================SHA256(gx||gy)========================
	
	size32_t EM[SIZE];
	encode(EM,keyA,keyB);

    mont_exp(Signature_A,EM,keyA,2048);

    printf("\n\n\n\nSignature generated @A:\n");
    mp_print(Signature_A);
    	//==================SHA256(gx||gy)========================
	//==================Digital Signature _Create=========================================================================================================================
}




void Initialize(unsigned char* out_AESkey,unsigned char* out_IV)
{
    unsigned short i;
	size32_t sharedPrivateKey[SIZE]= {0x00000000};
	DH_Process(sharedPrivateKey);

	unsigned char AESkey[16];
	unsigned char AES_IV[16];
	KDF(sharedPrivateKey,AESkey,AES_IV);

	size32_t Signature_A[SIZE]= {0x00000000};
	DigitalSign_Create(Signature_A);


	//=================Symmetric Encryption for Signature=======
	unsigned char AES_sig_M[256]; 
	for(i=0;i<SIZE;i++){
		AES_sig_M[  0 + i*4] = Signature_A[i];
        AES_sig_M[  1 + i*4] = (Signature_A[i]>>8);
    	AES_sig_M[  2 + i*4] = (Signature_A[i]>>16);
        AES_sig_M[  3 + i*4] = (Signature_A[i]>>24);
	}
	unsigned char AESMAC[16];
	unsigned char AES_sig_C[256];  
    unsigned char AES_sig_D[256];

	AES128_Encrypt2048(AES_sig_C,AESMAC,AES_sig_M,AESkey,AES_IV);

	//=================Symmetric Encryption for Signature=======




	//==================Digital Signature _Verify@B=========================================================================================================================
	AES128_Decrypt2048(AES_sig_D,AESMAC,AES_sig_C,AESkey,AES_IV);

	char isSame = AES128_Check2048(AES_sig_M,AES_sig_D);
    	if(isSame)printf("Signature Symmetric En/Decryption Process success\n");
    	else printf("Process failed\n"); 

    size32_t EM_ver[SIZE];
    size32_t Signature_A_ver[SIZE]= {0x00000000};
    encode(EM_ver,keyA,keyB);
    mont_exp(Signature_A_ver,EM_ver,keyA,2048);

    printf("\n\n\n\nSignature verify @A:\n");
    mp_print(Signature_A_ver);
	//==================Digital Signature _Verify@B=========================================================================================================================


	for(i=0 ;i<16;i++)out_AESkey[i]=AESkey[i];
	for(i=0 ;i<16;i++)out_IV[i]    =AES_IV[i];
}

void AES128_Encrypt(unsigned char* cipher ,      unsigned char* MAC,const unsigned char* message,const unsigned char*sharekey,const unsigned char*IV){
	AES128_Encrypt2048(cipher,MAC,message,sharekey,IV);
}
void AES128_Decrypt(unsigned char* message,const unsigned char* MAC,const unsigned char* cipher ,const unsigned char*sharekey,const unsigned char*IV){
	AES128_Decrypt2048(message,MAC,cipher,sharekey,IV);
}
int  AES128_Check(unsigned char* message,const unsigned char* decrypt ){
	return AES128_Check2048(message,decrypt);
}


int main(int argc, char *argv[]) {


	unsigned char AESkey[16];
   	unsigned char IV[16];
	Initialize(AESkey,IV);



	printf("Process Done\n");

    return 0;
}



/*16 bit Version
 *
size16_t g[SIZE]={
    0x0002,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,
    0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,
    0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,
    0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,
    0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,
    0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000};


size16_t keyA[SIZE]={
    0x1451,0xEEA5,0x14F1,0x8FE1,0xF48C,0x6279,0xF4DF,0x114C,0x3016,0x73B3,0xFFCB,0xE6F9,0xAAFC,0x2688,0x56CD,0x9601,0xC881,0xE937,0x52F0,0x1D5A,0x113D,0xCB74,0x029E,
    0xC67D,0x0E05,0x015F,0x9F06,0xC5FE,0x4A1C,0x8BF6,0x8572,0xB4CC,0x4953,0x0BBB,0xFBAC,0xB9D0,0x00F5,0xE9E6,0xAB66,0xDE82,0x6774,0x1F45,0xB805,0x40CD,0xF81A,0x17D4,
    0x7F21,0x6D1F,0x4E61,0x5EBB,0xE15C,0x3CFF,0x7E37,0xCFF0,0xEFE4,0x3788,0x48CF,0xE336,0xC24A,0x8CE7,0xB6AC,0x9056,0x00AF,0xED90,0xEB1C,0xB3D3,0xEC15,0x82BB,0x7960,
    0x33E8,0x5023,0xDDAC,0x75F5,0x5A1C,0x1DF6,0x3F2C,0x5C6E,0xD7ED,0xF03F,0x0FFA,0x6341,0x1CA1,0x49E6,0x9BF5,0x4E0E,0x36EA,0x9863,0x8974,0xD3B5,0xD2F4,0xACA4,0x9FA5,
    0x1DF3,0xC779,0x924C,0x2A38,0xEF24,0x8214,0xE0EA,0x073D,0xA311,0xB774,0x5AA5,0x8A44,0xE5E0,0x095B,0x93F1,0x9337,0x6A05,0x577A,0x7A45,0x1E96,0xEABF,0x346A,0x4301,
    0xF5A6,0xC831,0x4A02,0xD157,0xE8B3,0xC4D2,0xC9CF,0xC560,0xB608,0xAFBF,0x3FC1,0xBFF5,0x52CA};


size16_t keyB[SIZE]={
    0xB89D,0x2DCD,0x8418,0x484B,0xAB9D,0x6D3C,0x8D90,0x1313,0x4D2C,0xEA17,0x16C7,0x4025,0x5B1A,0x2619,0xBCC7,0xB936,0xCF23,0x95DD,0x4CCF,0x2137,0x376E,0xC6A1,0xF9A0,
    0x0281,0x5DBB,0xD3A5,0x02F9,0xB66A,0x98CF,0x765F,0x8B8C,0x1184,0x90B5,0xDA9B,0xCE67,0xFC1F,0x000C,0x81B7,0x84DE,0x3BA3,0x0DE1,0x3A14,0xA88C,0x0FB1,0x0349,0xDE7E,
    0xB0B0,0x4296,0x4674,0x89FC,0x81BF,0x61AF,0xB632,0x03D0,0x6250,0x675C,0x7AE6,0xB3A4,0xA045,0xC871,0x683D,0x8543,0x6FBD,0x2EAA,0x6D9D,0xA53E,0xF9E7,0xFC74,0x27FD,
    0xE9E5,0xA51E,0x359D,0xE9C6,0xB304,0x175F,0xA2B6,0x2A8E,0xFBFF,0x7D97,0x589F,0x8017,0xC278,0xA7F5,0xA3D4,0xC564,0xBC7E,0x7261,0x9781,0xA341,0x6F87,0xDE61,0x9B77,
    0x09CA,0xD1C3,0x5E0D,0x20EF,0xBB34,0xBCC9,0x02A4,0x13C6,0xC076,0x1EAA,0x518D,0xC2FB,0x3027,0x1C32,0x27C0,0xD6F4,0x6908,0x7E13,0xC18D,0xAEEA,0x04CB,0xC8E6,0xB992,
    0xFF0C,0x1B66,0xF081,0x7639,0x9DCE,0x24A9,0xD636,0xE141,0xD458,0x9F89,0xCF89,0x0C1A,0x805F};


size16_t modofA[SIZE]={
    0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,
    0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,
    0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,
    0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,
    0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,
    0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000};



size16_t modofB[SIZE]={
    0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,
    0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,
    0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,
    0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,
    0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,
    0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000};*/
