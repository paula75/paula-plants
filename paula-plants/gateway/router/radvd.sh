#!/bin/sh

IFACE=${IFACE:-"lowpan0"}
IP=${IP:-"2001:dead:beef::1/64"}

# Enable forwarding for host
sysctl net.ipv6.conf.all.forwarding=1

# Set IP address
if ! ip address show $IFACE | grep $IP 
then
    ip address add $IP dev $IFACE
fi
radvd -d 5 -m stderr -n
