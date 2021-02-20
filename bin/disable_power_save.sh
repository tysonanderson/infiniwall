#!/bin/bash

# This script disables powersave on some x11 based linux computers.
# During very long climbing sessions, we want the monitors to continue to 
# display current status.  Powersave interfears with this.

xset s off
xset -dpms

xset -q | grep DPMS
