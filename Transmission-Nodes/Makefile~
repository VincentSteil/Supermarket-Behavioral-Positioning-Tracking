BOARD := BOARD_RESPECK

C_SOURCE_FILES += main.c

C_SOURCE_FILES += app_timer.c
C_SOURCE_FILES += simple_uart.c

OUTPUT_FILENAME		:= gpio-test
SDK_PATH			:= ../nrf51_sdk/
JLINK_PATH			:= /afs/inf.ed.ac.uk/user/s10/s1008380/Desktop/SLIP/JLink_Linux_V474

GNU_INSTALL_ROOT	:= /afs/inf.ed.ac.uk/user/s10/s1008380/Desktop/SLIP/arm-2013.05
GNU_VERSION			:= 4.7.2
GNU_PREFIX			:= arm-none-eabi


#Uncomment correct line if you have softdevice programmed on the chip.
DEVICE_VARIANT := xxaa
#DEVICE_VARIANT := xxab

#USE_SOFTDEVICE := S110
#USE_SOFTDEVICE := S210

include $(SDK_PATH)Source/templates/gcc/Makefile.common
