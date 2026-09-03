#ifndef FT_PING_H
# define FT_PING_H

# include <sys/socket.h>
# include <netinet/ip_icmp.h>
# include <netinet/ip.h>
# include <arpa/inet.h>

# include <stdio.h>
# include <stdlib.h>
# include <string.h>
# include <unistd.h>
# include <netdb.h>

# define USAGE_DESTADDR_REQ 1
// const = "usage error: Destination address required"

# define RECRTE_OPNOTPERM 2
// const = "record route: Operation not permitted"

typedef struct addrinfo	addrinfo;
typedef struct icmphdr	icmphdr;
typedef struct iphdr	iphdr;
typedef struct sockaddr	sockaddr;
typedef struct sockaddr_in	sockaddr_in;

typedef struct s_ping {
	const char*	av;
	const char*	ip_addr;
	u_int16_t	flags;
	// .... Flags...
} t_ping;

unsigned short checksum(void *b, int len);
void print_ip_address(struct sockaddr *addr, socklen_t addrlen);
void	print_icmphdr(const struct icmphdr* si);


#endif