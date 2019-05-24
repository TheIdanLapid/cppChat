################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../src/Client.cpp \
../src/ClientHandler.cpp \
../src/UDPListener.cpp 

OBJS += \
./src/Client.o \
./src/ClientHandler.o \
./src/UDPListener.o 

CPP_DEPS += \
./src/Client.d \
./src/ClientHandler.d \
./src/UDPListener.d 


# Each subdirectory must supply rules for building sources it contributes
src/%.o: ../src/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -I"Chat/SocketUtilities/src" -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


