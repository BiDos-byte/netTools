#ifndef __NETINTERFACE_H__
#define	__NETINTERFACE_H__

#include <stdio.h>
#include <string.h>
#include <strings.h>
#include <unistd.h>
#include <time.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/ioctl.h>
#include <net/if.h>



int get_local_addr(struct sockaddr_in *addr, const char *interface);
int get_local_mask(struct sockaddr_in *addr, const char *interface);
int get_local_mask2(char *mask, const char *interface);
int get_local_broadcast(char *ip, const char *interface);
int get_local_mac( uint8_t *mac, const char *interface);
int eth_link_status(void);


#endif
