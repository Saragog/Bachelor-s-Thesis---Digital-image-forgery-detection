################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../src/DrawingOperations.cpp \
../src/ExivOperations.cpp \
../src/HistogramOperations.cpp \
../src/Inzynierka.cpp \
../src/SignOperations.cpp \
../src/WindowAllExivData.cpp \
../src/WindowExivData.cpp \
../src/WindowMain.cpp \
../src/WindowMenu.cpp \
../src/WindowSecureCheck.cpp \
../src/WindowSecureImage.cpp \
../src/moc_WindowMain.cpp \
../src/moc_WindowMenu.cpp \
../src/moc_WindowSecureCheck.cpp \
../src/moc_WindowSecureImage.cpp 

OBJS += \
./src/DrawingOperations.o \
./src/ExivOperations.o \
./src/HistogramOperations.o \
./src/Inzynierka.o \
./src/SignOperations.o \
./src/WindowAllExivData.o \
./src/WindowExivData.o \
./src/WindowMain.o \
./src/WindowMenu.o \
./src/WindowSecureCheck.o \
./src/WindowSecureImage.o \
./src/moc_WindowMain.o \
./src/moc_WindowMenu.o \
./src/moc_WindowSecureCheck.o \
./src/moc_WindowSecureImage.o 

CPP_DEPS += \
./src/DrawingOperations.d \
./src/ExivOperations.d \
./src/HistogramOperations.d \
./src/Inzynierka.d \
./src/SignOperations.d \
./src/WindowAllExivData.d \
./src/WindowExivData.d \
./src/WindowMain.d \
./src/WindowMenu.d \
./src/WindowSecureCheck.d \
./src/WindowSecureImage.d \
./src/moc_WindowMain.d \
./src/moc_WindowMenu.d \
./src/moc_WindowSecureCheck.d \
./src/moc_WindowSecureImage.d 


# Each subdirectory must supply rules for building sources it contributes
src/%.o: ../src/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -I/usr/include/qt4 -I/usr/local/include/exiv2 -I/usr/include/qt4/Qt -I/usr/include/qt4/QtCore -I/usr/include/qt4/QtGui -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


