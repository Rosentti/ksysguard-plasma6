#!/bin/bash
if pgrep -x "ksysguard" > /dev/null
then
    killall "ksysguard"
else
    ksysguard
fi
