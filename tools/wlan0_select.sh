#!/bin/bash
mode=`cat /home/pi/FollowMe/tools/wlan0-mode 2> /dev/null` 
wlan="wlan0-normal"
if test "$mode" = "wlan0-hotspot"
then
	wlan=$mode
fi

echo $wlan
