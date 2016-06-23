

#PATH=/home/dschacht/extraDrive/toolchains/toolchain-mipsel_4.1.1_BRCM24/bin:/usr/bin/

CPP_FILES=main.cpp \
station_interface.cpp \
tcp_server.cpp \
client_interface.cpp \
tcp_client.cpp \
system_utilities.cpp \
system_error.cpp \
numerical_statistics.cpp 

LIBRARY_INC=-lrt

#VPATH=numerical_statistics.cpp:../../libraries/Numerical_Statistics/

#SEARCH=-I../../libraries/Numerical_Statistics/ \
#-I../Common

default: host

target:
	mipsel-linux-uclibc-g++ -o weather-target -I../Common $(CPP_FILES) $(LIBRARY_INC)

host:
	g++ -o weather-host $(CPP_FILES) $(LIBRARY_INC)
