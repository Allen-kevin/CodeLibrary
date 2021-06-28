com=$1

echo $com
for i in `seq 150 200`;
	do
		if [ "$com" == "up" ]; then
		    echo "up "
#			ifconfig enp1s0f1:$i 192.168.3.$i up;
      		ifconfig eno2:$i 192.168.100.$i up;
		elif [ "$com" == "down" ]; then
#			ifconfig enp1s0f1:$i down;
     		ifconfig eno2:$i down;
			echo "down"
		fi
	done;
systemctl stop firewalld.service
#arp -s 192.168.5.4 aa:bb:cc:dd:55:04
