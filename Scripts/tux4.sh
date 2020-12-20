#!/bin/bash

echo -n "Restarting Network service..."
service networking restart
echo "Done!"

ifconfig eth0 up 172.16.30.254/24
ifconfig eth0 up 172.16.31.253/24
echo "IP Adresses set!"

route add default gw 172.16.31.254
echo "Default gateway route set!"

echo 1 > /proc/sys/net/ipv4/ip_forward
echo 0 > /proc/sys/net/ipv4/icmp_echo_ignore_broadcasts

printf "search netlab1.fe.up.pt\nnameserver 172.16.1.1\nnameserver 172.16.2.1\n" > /etc/resolv.conf
echo "DNS set!"

echo "Performing ping test..."
ping -q -c 1 google.pt > /dev/null

if [ "$?" -ne "$zero" ]; then
	echo -e "\033[0;31mERROR\033[0m: Could NOT ping a foreign host. Please review! Error "
else
	echo -e "\033[0;32mSUCCESS\033[0m: Could ping a foreign host, by hostname. Network and DNS are OK."
fi
