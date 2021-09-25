#!/bin/sh

value=$(mosquitto_sub -u broker2 -P 123456 -t Temp -C 1)
echo $value
