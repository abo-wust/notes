#！/usr/bin/env  bash
#
# Author     : Jack zhao
# Data         : 2017/12/08
# Description: This script is used to monitor the use of local system resources.

###The global variable###
export PATH=$PATH:/usr/kerberos/sbin:/usr/kerberos/bin:/usr/local/sbin:/usr/local/bin:/sbin:/bin:/usr/sbin:/usr/bin:/root/bin
export LANG=C
INITDIR=`pwd`
LOCAL_TIME=`date '+%F %H:%M:%S'`
BEFORE_DAY=`date -d "-1 days" +%F`
WHOLE_TIME=`date '+%H'`

###Mailbox configuration###
ACCENT_MAIL="123456@126.com"

###The alarm threshold###
LOAD_AVERAGE_ALARM=2
CPU_ALARM=70
MEMORY_ALARM=70
SWAP_ALARM=30
ROOT_PART_ALARM=95
IN_NETWORK_TRAFFIC_ALARM=167772160
OUT_NETWORK_TRAFFIC_ALARM=167772160

###Log variable###
ACCESS_LOG_PATH=localhost_access_log.${BEFORE_DAY}.txt


####Timed task###
TIMING=08

###Public function###
out_format(){
    printf "%-22s %-5s %-5s \n" "$1" ":" "$2"
}

###Send an alarm mail###
send_mail(){
    python mail.py "$ACCENT_MAIL" "[Alarm]:$1" "`cat $INITDIR/Monitoring.log`"
}

###Get load information 15 minutes###
get_load_average(){
    LOAD_AVERAGE=`uptime | awk -F "," '{print$NF}' | sed 's#[[:space:]]##g' `
    out_format "Load average"  "$LOAD_AVERAGE" >> $INITDIR/Monitoring.log
    return 0
}

###Get CPU usage###
get_cpu_usage(){
    CPU_FREE=`vmstat 1 5 |sed -n '3,$p' |awk '{x = x + $15} END {print x/5}' |awk -F. '{print $1}'`
    CPU_USAGE=$((100 - $CPU_FREE))
    out_format "CPU usage"  "${CPU_USAGE}%" >> $INITDIR/Monitoring.log
    return 0
}

###Get memory usage###
get_memory_usage(){
    MEMORY_USED=`free -m | grep -w "buffers\/cache:" | awk '{print$3}'`
    MEMORY_TOTAL=`free -m  | grep "^Mem" | awk '{print$2}'`
    MEMORY_USAGE=`echo "scale=2;${MEMORY_USED}/${MEMORY_TOTAL}*100;" | bc -l`
    out_format "Memory usage"  "${MEMORY_USAGE}%" >> $INITDIR/Monitoring.log
    return 0
}

###Get Swap usage###
get_swap_usage(){
    SWAP_USED=`free -m | grep -w "^Swap" | awk '{print$3}'`
    SWAP_TOTAL=`free -m | grep -w "^Swap" | awk '{print$2}'`
    SWAP_USAGE=`echo "scale=2;${SWAP_USED}/${SWAP_TOTAL}*100;" | bc -l`
    out_format "Swap usage"  "${SWAP_USAGE}%" >> $INITDIR/Monitoring.log
    return 0
}

###Get disk usage###
get_disk_usage(){
    MOUNT_POINT=`df -hP | egrep -wv  '^tmpfs|Filesystem|boot' | awk '{print$NF}'`
    for i in `echo $MOUNT_POINT`;do
        DISK_USAGE=`df -hP | grep -w "$i" | awk '{print$5}'`
        out_format "Disk usage $i"  "${DISK_USAGE}" >> $INITDIR/Monitoring.log
    done
    ROOT_PART=`df -hP  | grep -w "/" | awk '{print$5}' | awk -F "%" '{print$1}'`
    return  0
}

###Get network traffic###
get_network_traffic(){
    NETWORK_TRAFFIC=`sar -n DEV 1 60|grep Average|grep eth0|awk '{print "Input:",$5*1000*8,"bps","Output:",$6*1000*8,"bps"}'`
    out_format "Network traffic"  "${NETWORK_TRAFFIC}" >> $INITDIR/Monitoring.log
    return 0
}

###Get website PV,UV###
get_website_pu(){
    WEBSITE_PV=`grep -v "^127.0.0.1" ${ACCESS_LOG_PATH}  | wc -l`
    out_format "Before Day Website PV" "$WEBSITE_PV" >> $INITDIR/Monitoring.log
    WEBSITE_UV=`grep -v "^127.0.0.1" ${ACCESS_LOG_PATH} | awk '{print$1}' | sort | uniq | wc -l`
    out_format "Before Day Website UV" "$WEBSITE_UV" >> $INITDIR/Monitoring.log
}

###Server information###
IP=`ip addr show eth0 | grep -w "inet" | awk -F "[ /]+" '{print$3}'`
HOSTNAME=`hostname`
SN=`sudo -b /usr/sbin/dmidecode | grep -A5 "System Information" | grep "Serial Number"| awk -F ":" '{print$2}' | sed 's#^[[:space:]]##g'`
OS=`cat /etc/redhat-release`

###Program entrance###
echo "############### Monitoring information Date:$LOCAL_TIME ###############" > $INITDIR/Monitoring.log
out_format "IP"  "$IP" >> $INITDIR/Monitoring.log
out_format "Hostname"  "$HOSTNAME" >> $INITDIR/Monitoring.log
out_format "SN"  "$SN" >> $INITDIR/Monitoring.log
out_format "Os"  "$OS" >> $INITDIR/Monitoring.log

###Call function###
get_load_average
get_cpu_usage
get_memory_usage
get_swap_usage
get_disk_usage
get_network_traffic
get_website_pu

###variable format###
LOAD_AVERAGE_FORMAT=`echo $LOAD_AVERAGE | awk -F "." '{print$1}'`
MEMORY_USAGE_FORMAT=`echo $MEMORY_USAGE | awk -F "." '{print$1}'`
SWAP_USAGE_FORMAT=`echo $SWAP_USAGE | awk -F "." '{print$1}'`
IN_NETWORK_TRAFFIC=`echo $NETWORK_TRAFFIC | awk '{print$2}'`
OUT_NETWORK_TRAFFIC=`echo $NETWORK_TRAFFIC | awk '{print$5}'`

##Monitoring threshold judgment###
if [ ${LOAD_AVERAGE_FORMAT} -ge ${LOAD_AVERAGE_ALARM} ];then
    send_mail "$IP Load:High load average."
elif [ $CPU_USAGE -ge $CPU_ALARM ]; then
    send_mail "$IP CPU:usage greater than ${CPU_ALARM}%."
elif [ $MEMORY_USAGE_FORMAT -ge $MEMORY_ALARM ];then
    send_mail "$IP Memory:usage greater than ${MEMORY_ALARM}%."
elif [ $SWAP_USAGE_FORMAT -ge $SWAP_ALARM ];then
    send_mail "$IP Swap:usage greater than ${SWAP_ALARM}%."
elif [ $ROOT_PART -ge $ROOT_PART_ALARM ]; then
    send_mail "$IP Root Partition:usage greater than ${ROOT_PART_ALARM}%."
elif [ $IN_NETWORK_TRAFFIC -ge $IN_NETWORK_TRAFFIC_ALARM ]; then
    send_mail "$IP Network Traffic:Input Greater than ${IN_NETWORK_TRAFFIC_ALARM}bps."
elif [ $OUT_NETWORK_TRAFFIC -ge $OUT_NETWORK_TRAFFIC_ALARM ]; then
    send_mail "$IP Network Traffic:Output Greater than ${OUT_NETWORK_TRAFFIC_ALARM}bps."
fi

###Record history log###
cat $INITDIR/Monitoring.log >> $INITDIR/Monitor_history.log

###Send a notification email at a time###
if [ "$WHOLE_TIME" == "$TIMING" ];then
    python mail.py "$ACCENT_MAIL" "[Notice]:$IP Monitoring information" "`cat $INITDIR/Monitoring.log`"
fi

###Delete temporary files###
INITDIR_CONFIRM=${INITDIR:-/tmp}
rm -f $INITDIR_CONFIRM/Monitoring.log