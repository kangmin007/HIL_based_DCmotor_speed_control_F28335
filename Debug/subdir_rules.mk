################################################################################
# Automatically-generated file. Do not edit!
################################################################################

SHELL = cmd.exe

# Each subdirectory must supply rules for building sources it contributes
%.obj: ../%.c $(GEN_OPTS) | $(GEN_FILES) $(GEN_MISC_FILES)
	@echo 'Building file: "$<"'
	@echo 'Invoking: C2000 Compiler'
	"C:/ti/ccs1240/ccs/tools/compiler/bin/cl2000" -v28 -ml -mt --float_support=fpu32 --include_path="C:/Users/User/Desktop/Project/KangminKim/DSP/CapstoneDesignEE2_DCmotorControl_20251120_v2/TI_include" --include_path="C:/ti/ccs1240/ccs/tools/compiler/include" --include_path="C:/Users/User/Desktop/Project/KangminKim/DSP/CapstoneDesignEE2_DCmotorControl_20251120_v2/include" -g --diag_warning=225 --diag_wrap=off --display_error_number --abi=coffabi --preproc_with_compile --preproc_dependency="$(basename $(<F)).d_raw" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: "$<"'
	@echo ' '


