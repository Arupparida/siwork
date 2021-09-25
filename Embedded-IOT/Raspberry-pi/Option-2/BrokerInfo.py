import subprocess
from datetime import datetime

val= subprocess.check_output('./MQTTsub.sh')
output= str(val)
output=output[2:-2]
now= datetime.now()
current_time = now.strftime("%A,%m/%d/%Y,%I:%M:%S%p")
msg= output[:-2]+',"BrokerID":"broker01","BrokerTime":"%s"}'%current_time
print(msg)
 