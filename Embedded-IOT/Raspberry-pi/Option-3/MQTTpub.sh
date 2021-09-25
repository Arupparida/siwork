#!/bin/sh

while :
do
	output=$(python3 BrokerInfo.py)
	echo $output
	mosquitto_pub -d -h niaiot.azure-devices.net -p 8883 -i mypi -u 'niaiot.azure-devices.net/mypi/?api-version=2018-06-30' -P 'SharedAccessSignature sr=niaiot.azure-devices.net%2Fdevices%2Fmypi&sig=tDY%2B8yhTIbq99mMIqtxSYw6Sc4tqBLBVk6loO5EWLdA%3D&se=1630131290' -t devices/mypi/messages/events/ -V mqttv311 --cafile /home/pi/Documents/ssl/BaltimoreCyberTrustRoot.crt.pem -q 1 -m $output --tls-version tlsv1.2
done
