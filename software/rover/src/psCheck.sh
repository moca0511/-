#!/bin/bash

PS_DATA=$(ps ax | grep -v grep)

echo "crontab"
echo "${PS_DATA}" | grep reboot_crontab.sh
echo "${PS_DATA}" | grep crontab_start.sh
echo ""

echo "mjpg"
echo "${PS_DATA}" | grep mjpg
echo ""

echo "str2str"
echo "${PS_DATA}" | grep str2str
echo ""

echo "*.out"
echo "${PS_DATA}" | grep "\.out"
echo ""

echo "tcp_clent"
echo "${PS_DATA}" | grep tcp_client.py
echo ""


echo "icm"
echo "${PS_DATA}" | grep icm.py
echo ""


echo "bot"
echo "${PS_DATA}" | grep run.py 
echo ""


