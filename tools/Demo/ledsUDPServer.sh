#!/bin/sh

[[ $# -ne 1 ]] && echo "usage: $0 USB_Interface" && exit 1

echo "!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!"
echo "!!! WARNING!!! THIS SCRIPT MUST BE STARTED FROM THE ROOT OF CONTIKI !!!"
echo "!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!"

echo "\n"

echo "I test the presence of the required apps"
[[ -z "`which make`" ]]  && "MAKE:  [FAILED]" && exit 2
echo "MAKE:  [DONE]"
[[ -z "`which xterm`" ]] && "XTERM: [FAILED]" && exit 2
echo "XTERM: [DONE]"
[[ -z "`which ant`" ]]   && "ANT:   [FAILED]" && exit 2
echo "ANT:   [DONE]"
[[ -z "`which java`" ]]  && "JAVA:  [FAILED]" && exit 2
echo "JAVA:  [DONE]"

echo "\n"

echo "This script will help you to run the ledsUDPServer application with SLIP interface"
echo "=================================================================================="


echo "\n/--------------------\\"
echo "|Reminder:           |"
echo "| - USB Interface: $1|"
echo "\\--------------------/"

echo "\nFirst step: create a SLIP tunnel between $1 and /dev/tun0"
echo "---------------------------------------------------------"
echo "I go to tools/SLIP"
cd $PWD/tools/SLIP
echo "I compile"
make tunslip
echo "I start tunslip in a new terminal"
xterm -T tunslip -e sudo ./tunslip -s $1 -B 57600 192.168.1.1 255.255.0.0 &
echo "NOW YOU HAVE A TERMINAL WITH THE TUNNEL RUNNING"

cd - > /dev/null

echo "\nSecond step: compile, program and run the wismote."
echo "-------------------------------------------------"
echo "I go to examples/ledsUDPServer" 
cd $PWD/examples/ledsUDPServer
echo "I compile the code"
make TARGET=wismote
echo "I program and run the wismote" 
xterm -T MSPDebug -e mspdebug -j olimex "prog ledsUDPServer-example.wismote" "run" &
echo "NOW YOUR WISMOTE MUST BE RUNNING WITH THE GOOD PROGRAM"

cd - > /dev/null

echo "\nThird and last step: start the UDP java client"
echo "----------------------------------------------"
echo "I go to tools/UDP"
cd $PWD/tools/UDP
echo "I compile"
ant ledsUDPClient
echo "I run the client"
java ledsUDPClient 192.168.1.2 1234

cd - > /dev/null

echo "\nNOW, I WILL SHUTDOWN THE TUNNEL AND THE MSPDEBUG"

kill -s QUIT `ps | grep "xterm -T" | cut -f1 -d " " | xargs` 2> /dev/null
