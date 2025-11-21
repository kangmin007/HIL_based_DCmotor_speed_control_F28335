################################################################################
# Automatically-generated file. Do not edit!
################################################################################

SHELL = cmd.exe

# Each subdirectory must supply rules for building sources it contributes
source_TI/%.obj: ../source_TI/%.asm $(GEN_OPTS) | $(GEN_FILES) $(GEN_MISC_FILES)
	@echo 'Building file: "$<"'
	@echo 'Invoking: C2000 Compiler'
	"C:/ti/ccs1010/ccs/tools/compiler/ti-cgt-c2000_20.2.3.LTS/bin/cl2000" -v28 -ml -mt --float_support=fpu32 --include_path="C:/Users/User/Desktop/HSK/Typhoon_DSP/IM IFOC/DSP/IM_IFOC_Typhoon/include_TI" --include_path="C:/ti/ccs1010/ccs/tools/compiler/ti-cgt-c2000_20.2.3.LTS/include" --include_path="C:/Users/User/Desktop/HSK/Typhoon_DSP/IM IFOC/DSP/IM_IFOC_Typhoon/include" -g --diag_warning=225 --diag_wrap=off --display_error_number --abi=coffabi --preproc_with_compile --preproc_dependency="source_TI/$(basename $(<F)).d_raw" --obj_directory="source_TI" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: "$<"'
	@echo ' '

source_TI/%.obj: ../source_TI/%.c $(GEN_OPTS) | $(GEN_FILES) $(GEN_MISC_FILES)
	@echo 'Building file: "$<"'
	@echo 'Invoking: C2000 Compiler'
	"C:/ti/ccs1010/ccs/tools/compiler/ti-cgt-c2000_20.2.3.LTS/bin/cl2000" -v28 -ml -mt --float_support=fpu32 --include_path="C:/Users/User/Desktop/HSK/Typhoon_DSP/IM IFOC/DSP/IM_IFOC_Typhoon/include_TI" --include_path="C:/ti/ccs1010/ccs/tools/compiler/ti-cgt-c2000_20.2.3.LTS/include" --include_path="C:/Users/User/Desktop/HSK/Typhoon_DSP/IM IFOC/DSP/IM_IFOC_Typhoon/include" -g --diag_warning=225 --diag_wrap=off --display_error_number --abi=coffabi --preproc_with_compile --preproc_dependency="source_TI/$(basename $(<F)).d_raw" --obj_directory="source_TI" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: "$<"'
	@echo ' '


