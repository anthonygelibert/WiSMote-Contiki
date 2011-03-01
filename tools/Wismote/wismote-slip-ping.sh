#!/bin/sh

[[ $# -ne 2 ]] && echo "usage: $0 interface @IP" && exit 1

case `uname -s` in
    "Darwin") ping -v -b $1 -c 4 -t 20 -m 64 -s 1 $2;;
    *) echo "Your system is currently not supported...";;
esac
