

#PATH=/home/dschacht/extraDrive/toolchains/toolchain-mipsel_4.1.1_BRCM24/bin:/usr/bin/

CPP_FILES=main.cpp \
station_interface.cpp \
tcp_server.cpp \
client_interface.cpp \
tcp_client.cpp \
system_utilities.cpp \
system_error.cpp

LIBRARY_INC=-lrt

default: target

target:
	mipsel-linux-uclibc-g++ -o weather-target -I../Common $(CPP_FILES) $(LIBRARY_INC)

host:
	g++ -o weather-host -I../Common $(CPP_FILES) $(LIBRARY_INC)
