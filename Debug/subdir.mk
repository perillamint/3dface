################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../faceprocess.cpp \
../libfreenect_cv.cpp \
../main.cpp 

OBJS += \
./faceprocess.o \
./libfreenect_cv.o \
./main.o 

CPP_DEPS += \
./faceprocess.d \
./libfreenect_cv.d \
./main.d 


# Each subdirectory must supply rules for building sources it contributes
%.o: ../%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: Cross G++ Compiler'
	g++ -I/usr/include/opencv -I/usr/local/include/libfreenect -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


