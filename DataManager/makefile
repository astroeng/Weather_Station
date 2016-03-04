

#PATH=/home/dschacht/extraDrive/toolchains/toolchain-mipsel_4.1.1_BRCM24/bin:/usr/bin/

default: target

target:
	mipsel-linux-uclibc-g++ -o weather-target main.cpp station_interface.cpp tcp_server.cpp client_interface.cpp tcp_client.cpp system_utilities.cpp system_error.cpp -lrt

host:
	g++ -o weather-host main.cpp station_interface.cpp tcp_server.cpp client_interface.cpp tcp_client.cpp system_utilities.cpp system_error.cpp -lrt
