/*
 * test.cpp
 *
 *  Created on: Dec 2, 2014
 *      Author: emin
 */
#include <net/if.h>
#include <linux/if_tun.h>

#include <stdio.h>
#include <fcntl.h>
#include <errno.h>
#include <string.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <algorithm>
#include <algorithm>


int tuntap_open(char *dev, char* HWaddress, char* IPaddress, char* netmask, int mtu)
{
	struct ifreq ifr;
	int fd, err;
	char *tuntap_device = "/dev/net/tun";
#define N2N_LINUX_SYSTEMCMD_SIZE 128
	char buf[N2N_LINUX_SYSTEMCMD_SIZE];
	int rc;


	fd = open(tuntap_device, O_RDWR);
	if(fd < 0) {
		printf("ERROR: ioctl() [%s][%d]\n", strerror(errno), errno);
		return -1;
	}

	memset(&ifr, 0, sizeof(ifr));
	ifr.ifr_flags = IFF_TAP|IFF_NO_PI; /* Want a TAP device for layer 2 frames. */

	if (dev && dev[0] == '\0')
	{
		printf("devName is empty.");
	}
	else
	{
		strncpy(ifr.ifr_name, dev, IFNAMSIZ);
		rc = ioctl(fd, TUNSETIFF, (void *)&ifr);
	}


	if(rc < 0) {
		close(fd);
		return -1;
	}

	if (HWaddress && HWaddress[0] == '\0')
	{
		printf("HWaddress is empty.");
	}
	else
	{
		/* Set the hw address before bringing the if up. */
		snprintf(buf, sizeof(buf), "/sbin/ifconfig %s hw ether %s",
				ifr.ifr_name, HWaddress );
		system(buf);
	}

	if (IPaddress && IPaddress[0] == '\0')
	{
		printf("IPaddress is empty.");
	}
	else
	{
		if (netmask && netmask[0] == '\0')
		{
			printf("netmask is empty.");
		}
		else
		{

			snprintf(buf, sizeof(buf), "/sbin/ifconfig %s %s netmask %s mtu %d up",
					ifr.ifr_name, IPaddress, netmask, mtu);
			system(buf);
		}
	}

	return(fd);
}

int main()
{

	tuntap_open("edge0","80:70:60:50:40:30","10.1.2.3", "255.255.255.0", 1500);
	while(1){}
}



