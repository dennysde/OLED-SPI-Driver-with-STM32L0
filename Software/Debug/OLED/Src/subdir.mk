################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (14.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../OLED/Src/GLCD_driver.c \
../OLED/Src/Icons.c \
../OLED/Src/picture1.c \
../OLED/Src/sans_serif.c 

OBJS += \
./OLED/Src/GLCD_driver.o \
./OLED/Src/Icons.o \
./OLED/Src/picture1.o \
./OLED/Src/sans_serif.o 

C_DEPS += \
./OLED/Src/GLCD_driver.d \
./OLED/Src/Icons.d \
./OLED/Src/picture1.d \
./OLED/Src/sans_serif.d 


# Each subdirectory must supply rules for building sources it contributes
OLED/Src/%.o OLED/Src/%.su OLED/Src/%.cyclo: ../OLED/Src/%.c OLED/Src/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m0plus -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32L011xx -c -I../Core/Inc -I../Drivers/STM32L0xx_HAL_Driver/Inc -I../Drivers/STM32L0xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32L0xx/Include -I../Drivers/CMSIS/Include -I"C:/Users/karimex/Downloads/NUCLEO_L011K4-OLED64x48/Software/OLED/Inc" -Oz -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfloat-abi=soft -mthumb -o "$@"

clean: clean-OLED-2f-Src

clean-OLED-2f-Src:
	-$(RM) ./OLED/Src/GLCD_driver.cyclo ./OLED/Src/GLCD_driver.d ./OLED/Src/GLCD_driver.o ./OLED/Src/GLCD_driver.su ./OLED/Src/Icons.cyclo ./OLED/Src/Icons.d ./OLED/Src/Icons.o ./OLED/Src/Icons.su ./OLED/Src/picture1.cyclo ./OLED/Src/picture1.d ./OLED/Src/picture1.o ./OLED/Src/picture1.su ./OLED/Src/sans_serif.cyclo ./OLED/Src/sans_serif.d ./OLED/Src/sans_serif.o ./OLED/Src/sans_serif.su

.PHONY: clean-OLED-2f-Src

