################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../src/Connection.cpp \
../src/Sample.cpp 

OBJS += \
./src/Connection.o \
./src/Sample.o 

CPP_DEPS += \
./src/Connection.d \
./src/Sample.d 


# Each subdirectory must supply rules for building sources it contributes
src/%.o: ../src/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -I"/home/arsenic/AnonyChat/AnonyChatLib/include" -I"/home/arsenic/AnonyChat/AnonyChatLib/include/ContactList" -I"/home/arsenic/AnonyChat/AnonyChatLib/include/Routing" -O0 -g3 -Wall -c -fmessage-length=0 -std=gnu++0x -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o"$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


