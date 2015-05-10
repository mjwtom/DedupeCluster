################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../src/proxy-server/chunk.c \
../src/proxy-server/proxy-server.c 

OBJS += \
./src/proxy-server/chunk.o \
./src/proxy-server/proxy-server.o 

C_DEPS += \
./src/proxy-server/chunk.d \
./src/proxy-server/proxy-server.d 


# Each subdirectory must supply rules for building sources it contributes
src/proxy-server/%.o: ../src/proxy-server/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C Compiler'
	gcc -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


