################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../sources/adc.c \
../sources/aht10.c \
../sources/app.c \
../sources/bluetooth.c \
../sources/can_app.c \
../sources/lcd.c \
../sources/led.c \
../sources/main.c \
../sources/uart.c 

OBJS += \
./sources/adc.o \
./sources/aht10.o \
./sources/app.o \
./sources/bluetooth.o \
./sources/can_app.o \
./sources/lcd.o \
./sources/led.o \
./sources/main.o \
./sources/uart.o 

C_DEPS += \
./sources/adc.d \
./sources/aht10.d \
./sources/app.d \
./sources/bluetooth.d \
./sources/can_app.d \
./sources/lcd.d \
./sources/led.d \
./sources/main.d \
./sources/uart.d 


# Each subdirectory must supply rules for building sources it contributes
sources/%.o: ../sources/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: ARM-GCC C Compiler'
	"$(TOOLCHAIN_ROOT)/bin/arm-none-eabi-gcc" -MMD -MT "$@" -DXMC4200_F64x256 -I"$(PROJECT_LOC)/Libraries/XMCLib/inc" -I"$(PROJECT_LOC)/headers" -I"$(PROJECT_LOC)/Libraries/CMSIS/Include" -I"$(PROJECT_LOC)/Libraries/CMSIS/Infineon/XMC4200_series/Include" -I"$(PROJECT_LOC)" -I"$(PROJECT_LOC)/Dave/Generated" -I"$(PROJECT_LOC)/Libraries" -Os -ffunction-sections -fdata-sections -Wall -std=gnu99 -mfloat-abi=softfp -Wa,-adhlns="$@.lst" -pipe -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d) $@" -mcpu=cortex-m4 -mfpu=fpv4-sp-d16 -mthumb -o "$@" "$<" 
	@echo 'Finished building: $<'
	@echo.

