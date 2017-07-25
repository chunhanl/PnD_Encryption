################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Each subdirectory must supply rules for building sources it contributes
src_crypto/aes.obj: ../src_crypto/aes.c $(GEN_OPTS) $(GEN_SRCS)
	@echo 'Building file: $<'
	@echo 'Invoking: C6000 Compiler'
	"C:/ti/ccsv5/tools/compiler/c6000/bin/cl6x" -mv6740 -O2 --include_path="C:/ti/ccsv5/tools/compiler/c6000/include" --include_path="C:/Users/r0603366/Desktop/DSP_dev/Crypto_src" --include_path="C:/Users/r0603366/Desktop/DSP_dev/DSP_src" --include_path="C:/Users/r0603366/Desktop/DSP_dev/C_src" --include_path="C:/Users/r0603501/Dropbox/PnD/DSP_dev/DSP_src" --include_path="C:/Users/r0603501/Dropbox/PnD/DSP_dev/C_src" --include_path="C:/Users/r0603366/Desktop/DSP_dev/realtime_audio/include" --include_path="C:/Users/r0603366/Desktop/DSP_dev/realtime_audio/src_audio" --include_path="" --include_path="C:/Users/r0603366/Desktop/DSP_dev/realtime_audio/Release" --include_path="C:/ti/bios_5_41_11_38/packages/ti/bios/include" --include_path="C:/ti/bios_5_41_11_38/packages/ti/rtdx/include/c6000" --display_error_number --diag_warning=225 --abi=coffabi --preproc_with_compile --preproc_dependency="src_crypto/aes.pp" --obj_directory="src_crypto" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: $<'
	@echo ' '


