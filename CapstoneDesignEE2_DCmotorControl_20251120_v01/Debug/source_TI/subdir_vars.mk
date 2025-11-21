################################################################################
# Automatically-generated file. Do not edit!
################################################################################

SHELL = cmd.exe

# Add inputs and outputs from these tool invocations to the build variables 
ASM_SRCS += \
../source_TI/DSP2833x_ADC_cal.asm \
../source_TI/DSP2833x_CSMPasswords.asm \
../source_TI/DSP2833x_CodeStartBranch.asm \
../source_TI/DSP2833x_DBGIER.asm \
../source_TI/DSP2833x_DisInt.asm \
../source_TI/DSP2833x_usDelay.asm 

C_SRCS += \
../source_TI/DSP2833x_Adc.c \
../source_TI/DSP2833x_CpuTimers.c \
../source_TI/DSP2833x_DMA.c \
../source_TI/DSP2833x_DefaultIsr.c \
../source_TI/DSP2833x_ECan.c \
../source_TI/DSP2833x_ECap.c \
../source_TI/DSP2833x_EPwm.c \
../source_TI/DSP2833x_EQep.c \
../source_TI/DSP2833x_GlobalVariableDefs.c \
../source_TI/DSP2833x_Gpio.c \
../source_TI/DSP2833x_I2C.c \
../source_TI/DSP2833x_Mcbsp.c \
../source_TI/DSP2833x_MemCopy.c \
../source_TI/DSP2833x_PieCtrl.c \
../source_TI/DSP2833x_PieVect.c \
../source_TI/DSP2833x_Sci.c \
../source_TI/DSP2833x_Spi.c \
../source_TI/DSP2833x_SysCtrl.c \
../source_TI/DSP2833x_Xintf.c 

C_DEPS += \
./source_TI/DSP2833x_Adc.d \
./source_TI/DSP2833x_CpuTimers.d \
./source_TI/DSP2833x_DMA.d \
./source_TI/DSP2833x_DefaultIsr.d \
./source_TI/DSP2833x_ECan.d \
./source_TI/DSP2833x_ECap.d \
./source_TI/DSP2833x_EPwm.d \
./source_TI/DSP2833x_EQep.d \
./source_TI/DSP2833x_GlobalVariableDefs.d \
./source_TI/DSP2833x_Gpio.d \
./source_TI/DSP2833x_I2C.d \
./source_TI/DSP2833x_Mcbsp.d \
./source_TI/DSP2833x_MemCopy.d \
./source_TI/DSP2833x_PieCtrl.d \
./source_TI/DSP2833x_PieVect.d \
./source_TI/DSP2833x_Sci.d \
./source_TI/DSP2833x_Spi.d \
./source_TI/DSP2833x_SysCtrl.d \
./source_TI/DSP2833x_Xintf.d 

OBJS += \
./source_TI/DSP2833x_ADC_cal.obj \
./source_TI/DSP2833x_Adc.obj \
./source_TI/DSP2833x_CSMPasswords.obj \
./source_TI/DSP2833x_CodeStartBranch.obj \
./source_TI/DSP2833x_CpuTimers.obj \
./source_TI/DSP2833x_DBGIER.obj \
./source_TI/DSP2833x_DMA.obj \
./source_TI/DSP2833x_DefaultIsr.obj \
./source_TI/DSP2833x_DisInt.obj \
./source_TI/DSP2833x_ECan.obj \
./source_TI/DSP2833x_ECap.obj \
./source_TI/DSP2833x_EPwm.obj \
./source_TI/DSP2833x_EQep.obj \
./source_TI/DSP2833x_GlobalVariableDefs.obj \
./source_TI/DSP2833x_Gpio.obj \
./source_TI/DSP2833x_I2C.obj \
./source_TI/DSP2833x_Mcbsp.obj \
./source_TI/DSP2833x_MemCopy.obj \
./source_TI/DSP2833x_PieCtrl.obj \
./source_TI/DSP2833x_PieVect.obj \
./source_TI/DSP2833x_Sci.obj \
./source_TI/DSP2833x_Spi.obj \
./source_TI/DSP2833x_SysCtrl.obj \
./source_TI/DSP2833x_Xintf.obj \
./source_TI/DSP2833x_usDelay.obj 

ASM_DEPS += \
./source_TI/DSP2833x_ADC_cal.d \
./source_TI/DSP2833x_CSMPasswords.d \
./source_TI/DSP2833x_CodeStartBranch.d \
./source_TI/DSP2833x_DBGIER.d \
./source_TI/DSP2833x_DisInt.d \
./source_TI/DSP2833x_usDelay.d 

OBJS__QUOTED += \
"source_TI\DSP2833x_ADC_cal.obj" \
"source_TI\DSP2833x_Adc.obj" \
"source_TI\DSP2833x_CSMPasswords.obj" \
"source_TI\DSP2833x_CodeStartBranch.obj" \
"source_TI\DSP2833x_CpuTimers.obj" \
"source_TI\DSP2833x_DBGIER.obj" \
"source_TI\DSP2833x_DMA.obj" \
"source_TI\DSP2833x_DefaultIsr.obj" \
"source_TI\DSP2833x_DisInt.obj" \
"source_TI\DSP2833x_ECan.obj" \
"source_TI\DSP2833x_ECap.obj" \
"source_TI\DSP2833x_EPwm.obj" \
"source_TI\DSP2833x_EQep.obj" \
"source_TI\DSP2833x_GlobalVariableDefs.obj" \
"source_TI\DSP2833x_Gpio.obj" \
"source_TI\DSP2833x_I2C.obj" \
"source_TI\DSP2833x_Mcbsp.obj" \
"source_TI\DSP2833x_MemCopy.obj" \
"source_TI\DSP2833x_PieCtrl.obj" \
"source_TI\DSP2833x_PieVect.obj" \
"source_TI\DSP2833x_Sci.obj" \
"source_TI\DSP2833x_Spi.obj" \
"source_TI\DSP2833x_SysCtrl.obj" \
"source_TI\DSP2833x_Xintf.obj" \
"source_TI\DSP2833x_usDelay.obj" 

C_DEPS__QUOTED += \
"source_TI\DSP2833x_Adc.d" \
"source_TI\DSP2833x_CpuTimers.d" \
"source_TI\DSP2833x_DMA.d" \
"source_TI\DSP2833x_DefaultIsr.d" \
"source_TI\DSP2833x_ECan.d" \
"source_TI\DSP2833x_ECap.d" \
"source_TI\DSP2833x_EPwm.d" \
"source_TI\DSP2833x_EQep.d" \
"source_TI\DSP2833x_GlobalVariableDefs.d" \
"source_TI\DSP2833x_Gpio.d" \
"source_TI\DSP2833x_I2C.d" \
"source_TI\DSP2833x_Mcbsp.d" \
"source_TI\DSP2833x_MemCopy.d" \
"source_TI\DSP2833x_PieCtrl.d" \
"source_TI\DSP2833x_PieVect.d" \
"source_TI\DSP2833x_Sci.d" \
"source_TI\DSP2833x_Spi.d" \
"source_TI\DSP2833x_SysCtrl.d" \
"source_TI\DSP2833x_Xintf.d" 

ASM_DEPS__QUOTED += \
"source_TI\DSP2833x_ADC_cal.d" \
"source_TI\DSP2833x_CSMPasswords.d" \
"source_TI\DSP2833x_CodeStartBranch.d" \
"source_TI\DSP2833x_DBGIER.d" \
"source_TI\DSP2833x_DisInt.d" \
"source_TI\DSP2833x_usDelay.d" 

ASM_SRCS__QUOTED += \
"../source_TI/DSP2833x_ADC_cal.asm" \
"../source_TI/DSP2833x_CSMPasswords.asm" \
"../source_TI/DSP2833x_CodeStartBranch.asm" \
"../source_TI/DSP2833x_DBGIER.asm" \
"../source_TI/DSP2833x_DisInt.asm" \
"../source_TI/DSP2833x_usDelay.asm" 

C_SRCS__QUOTED += \
"../source_TI/DSP2833x_Adc.c" \
"../source_TI/DSP2833x_CpuTimers.c" \
"../source_TI/DSP2833x_DMA.c" \
"../source_TI/DSP2833x_DefaultIsr.c" \
"../source_TI/DSP2833x_ECan.c" \
"../source_TI/DSP2833x_ECap.c" \
"../source_TI/DSP2833x_EPwm.c" \
"../source_TI/DSP2833x_EQep.c" \
"../source_TI/DSP2833x_GlobalVariableDefs.c" \
"../source_TI/DSP2833x_Gpio.c" \
"../source_TI/DSP2833x_I2C.c" \
"../source_TI/DSP2833x_Mcbsp.c" \
"../source_TI/DSP2833x_MemCopy.c" \
"../source_TI/DSP2833x_PieCtrl.c" \
"../source_TI/DSP2833x_PieVect.c" \
"../source_TI/DSP2833x_Sci.c" \
"../source_TI/DSP2833x_Spi.c" \
"../source_TI/DSP2833x_SysCtrl.c" \
"../source_TI/DSP2833x_Xintf.c" 


