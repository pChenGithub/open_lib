#! /bin/bash

echo "测试所有的端口是否通"

# tcp_port.sh

#RESULT=`echo "" | telnet $1 $2 2> /dev/null`
RESULT=`echo quit | timeout --signal=9 2 telnet $1 $2`

# tcp port is opening:
# Escape character is '^]'.

# tcp port is closed:
# Trying 127.0.0.1...

# RESULT string includes "Escape"
if [[ $RESULT =~ "Escape" ]]; then
    echo -e "host $1's tcp port $2 is \033[31mopening\033[0m."
else
    # Defect: time consuming
    echo -e "host $1's tcp port $2 is \033[31mclosed\033[0m."
fi






