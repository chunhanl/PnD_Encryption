################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Each subdirectory must supply rules for building sources it contributes
src_audio/audioSample_io.obj: ../src_audio/audioSample_io.c $(GEN_OPTS) $(GEN_SRCS)
	@echo 'Building file: $<'
	@echo 'Invoking: C6000 Compiler'
	"C:/ti/ccsv5/tools/compiler/c6000/bin/cl6x" -mv6740 -O2 --include_path="C:/ti/ccsv5/tools/compiler/c6000/include" --include_path="C:/Users/r0603366/Desktop/Secure_Codec_SpeechGroup6_CryptoGroup6_realtime/2013_realtime_audio" --include_path="C:/Users/r0603366/Desktop/Secure_Codec_SpeechGroup6_CryptoGroup6_realtime/2013_realtime_audio/include" --include_path="C:/Users/r0603366/Desktop/Secure_Codec_SpeechGroup6_CryptoGroup6_realtime/2013_realtime_audio/src_audio" --include_path="C:/Users/r0603366/Desktop/Secure_Codec_SpeechGroup6_CryptoGroup6_realtime/2013_realtime_audio/Release" --include_path="C:/ti/bios_5_41_11_38/packages/ti/bios/include" --include_path="C:/ti/bios_5_41_11_38/packages/ti/rtdx/include/c6000" --display_error_number --diag_warning=225 --abi=coffabi --preproc_with_compile --preproc_dependency="src_audio/audioSample_io.pp" --obj_directory="src_audio" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: $<'
	@echo ' '

src_audio/audioSample_main.obj: ../src_audio/audioSample_main.c $(GEN_OPTS) $(GEN_SRCS)
	@echo 'Building file: $<'
	@echo 'Invoking: C6000 Compiler'
	"C:/ti/ccsv5/tools/compiler/c6000/bin/cl6x" -mv6740 -O2 --include_path="C:/ti/ccsv5/tools/compiler/c6000/include" --include_path="C:/Users/r0603366/Desktop/Secure_Codec_SpeechGroup6_CryptoGroup6_realtime/2013_realtime_audio" --include_path="C:/Users/r0603366/Desktop/Secure_Codec_SpeechGroup6_CryptoGroup6_realtime/2013_realtime_audio/include" --include_path="C:/Users/r0603366/Desktop/Secure_Codec_SpeechGroup6_CryptoGroup6_realtime/2013_realtime_audio/src_audio" --include_path="C:/Users/r0603366/Desktop/Secure_Codec_SpeechGroup6_CryptoGroup6_realtime/2013_realtime_audio/Release" --include_path="C:/ti/bios_5_41_11_38/packages/ti/bios/include" --include_path="C:/ti/bios_5_41_11_38/packages/ti/rtdx/include/c6000" --display_error_number --diag_warning=225 --abi=coffabi --preproc_with_compile --preproc_dependency="src_audio/audioSample_main.pp" --obj_directory="src_audio" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: $<'
	@echo ' '

src_audio/process_audio.obj: ../src_audio/process_audio.c $(GEN_OPTS) $(GEN_SRCS)
	@echo 'Building file: $<'
	@echo 'Invoking: C6000 Compiler'
	"C:/ti/ccsv5/tools/compiler/c6000/bin/cl6x" -mv6740 -O2 --include_path="C:/ti/ccsv5/tools/compiler/c6000/include" --include_path="C:/Users/r0603366/Desktop/Secure_Codec_SpeechGroup6_CryptoGroup6_realtime/2013_realtime_audio" --include_path="C:/Users/r0603366/Desktop/Secure_Codec_SpeechGroup6_CryptoGroup6_realtime/2013_realtime_audio/include" --include_path="C:/Users/r0603366/Desktop/Secure_Codec_SpeechGroup6_CryptoGroup6_realtime/2013_realtime_audio/src_audio" --include_path="C:/Users/r0603366/Desktop/Secure_Codec_SpeechGroup6_CryptoGroup6_realtime/2013_realtime_audio/Release" --include_path="C:/ti/bios_5_41_11_38/packages/ti/bios/include" --include_path="C:/ti/bios_5_41_11_38/packages/ti/rtdx/include/c6000" --display_error_number --diag_warning=225 --abi=coffabi --preproc_with_compile --preproc_dependency="src_audio/process_audio.pp" --obj_directory="src_audio" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: $<'
	@echo ' '


