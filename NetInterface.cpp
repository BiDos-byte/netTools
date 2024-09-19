#include "NetInterface.h"

int get_local_addr(struct sockaddr_in *addr, const char *interface)
{
	struct ifreq ifr;
	int sock = socket(AF_INET, SOCK_DGRAM, 0);
	if (sock < 0) {
		fprintf(stderr, "Could not open socket\n");
		return -1;
	}

	ifr.ifr_addr.sa_family = AF_INET;
	strcpy(ifr.ifr_name, interface);

	/* Get IP address of interface */
	if (ioctl(sock, SIOCGIFADDR, &ifr) < 0) {
//		fprintf (stderr, "Could not retrieve IP address of the device \"%s\"\n", p_dev);
		goto err;
	}
	close(sock);
	if (addr)
		addr->sin_addr = ((struct sockaddr_in *)&ifr.ifr_addr)->sin_addr;
	return 0;
err:
	close(sock);
	return -1;
}

int get_local_mask(struct sockaddr_in *addr, const char *interface)
{
	struct ifreq ifr;
	int sock = socket(AF_INET, SOCK_DGRAM, 0);
	if (sock < 0) {
		fprintf(stderr, "Could not open socket\n");
		return -1;
	}

	strcpy(ifr.ifr_name, interface);

	/* Get IP address of interface */
	if (ioctl(sock, SIOCGIFNETMASK, &ifr) < 0) {
//		fprintf (stderr, "Could not retrieve IP address of the device \"%s\"\n", p_dev);
		close(sock);
		return -1;	
	}
	close(sock);
	if (addr)
	{
		struct sockaddr_in* s = (struct sockaddr_in*)&ifr.ifr_netmask;
		addr->sin_addr = s->sin_addr;
		return 0;
	}
	
	return -1;
	
}

int get_local_mask2(char *mask, const char *interface)
{
	struct ifreq ifr;
	int sock = socket(AF_INET, SOCK_DGRAM, 0);
	if (sock < 0) {
		fprintf(stderr, "Could not open socket\n");
		return -1;
	}

	strcpy(ifr.ifr_name, interface);

	/* Get IP address of interface */
	if (ioctl(sock, SIOCGIFNETMASK, &ifr) < 0) {
//		fprintf (stderr, "Could not retrieve IP address of the device \"%s\"\n", p_dev);
		close(sock);
		return -1;	
	}
	close(sock);
	struct sockaddr_in* s = (struct sockaddr_in*)&ifr.ifr_netmask;
    char* subnetmask = inet_ntoa(s->sin_addr);
	if(mask && subnetmask)
	{
		memcpy(mask, subnetmask, strlen(subnetmask));
		return 0;
	}
	return -1;
	
	
}

int get_local_broadcast(char *ip, const char *interface)
{
	struct ifreq ifr;
	int sock = socket(AF_INET, SOCK_DGRAM, 0);
	if (sock < 0) {
		fprintf(stderr, "Could not open socket\n");
		return -1;
	}

	strcpy(ifr.ifr_name, interface);

	/* Get IP address of interface */
	if (ioctl(sock, SIOCGIFBRDADDR, &ifr) < 0) {
//		fprintf (stderr, "Could not retrieve IP address of the device \"%s\"\n", p_dev);
		close(sock);
		return -1;	
	}
	close(sock);
	if (ip)
	{
		
		inet_ntop(AF_INET, &((struct sockaddr_in*)&ifr.ifr_broadaddr)->sin_addr, ip, INET_ADDRSTRLEN);
		return 0;
	}
	
	return -1;	
}


int get_local_mac( uint8_t *mac, const char *interface)
{
	struct ifreq ifreq;
	int sock_fd;
    
	if((sock_fd = socket(AF_INET, SOCK_STREAM, 0)) < 0) 
	{
		return -1;
	}
	if (mac) {
		strcpy(ifreq.ifr_name, interface);
		if(ioctl(sock_fd, SIOCGIFHWADDR, &ifreq) < 0)
		{
			//perror("ioctl:");
			goto err;
		}
		for (int i=0; i<6; i++)
		{
			mac[i] = ifreq.ifr_hwaddr.sa_data[i];
		}
	}
	close(sock_fd);
	return 0;

err:
	close(sock_fd);
	return -1;
}

int eth_link_status(void)
{
	FILE *fp = fopen ("/sys/class/net/eth0/carrier", "r");
	if (fp) 
	{
		int st = 0;
		fscanf(fp, "%d", &st);
		fclose(fp);
		return st;
	}
	return 0;
}
