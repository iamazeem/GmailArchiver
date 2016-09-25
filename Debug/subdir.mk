################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../gmail_oauth2.cpp \
../gmail_service.cpp \
../gmail_utils.cpp \
../main.cpp 

OBJS += \
./gmail_oauth2.o \
./gmail_service.o \
./gmail_utils.o \
./main.o 

CPP_DEPS += \
./gmail_oauth2.d \
./gmail_service.d \
./gmail_utils.d \
./main.d 


# Each subdirectory must supply rules for building sources it contributes
%.o: ../%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: Cross G++ Compiler'
	g++ -std=c++11 -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


