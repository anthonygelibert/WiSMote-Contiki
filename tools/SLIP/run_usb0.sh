#!/bin/sh
./tunslip -B 57600 -t tun0 -s /dev/ttyUSB0 192.168.1.1 255.255.255.0
