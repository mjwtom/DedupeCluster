################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../src/chunk.c \
../src/debug.c \
../src/network.c \
../src/proxy-server.c 

OBJS += \
./src/chunk.o \
./src/debug.o \
./src/network.o \
./src/proxy-server.o 

C_DEPS += \
./src/chunk.d \
./src/debug.d \
./src/network.d \
./src/proxy-server.d 


# Each subdirectory must supply rules for building sources it contributes
src/%.o: ../src/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C Compiler'
	gcc -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


