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

/*
Success: code 0
No reply: code 1
Other errors: code 2
*/

# define USAGE_DESTADDR_REQ 1
# define ERRSTR_1 "usage error: Destination address required"

# define RECRTE_OPNOTPERM 2
# define ERRSTR_2 "record route: Operation not permitted"

# define INVALID_OPT 3
# define ERRSTR_3 "invalid option -- " //  + char ''

# define GAI_ERROR 4
# define ERRSTR_4 ""

# define ERR_SOCKET 5
# define ERRSTR_5 "Error socket"

# define FLAG_VERBOSE 1 << 0
# define FLAG_HELP 1 << 1

typedef struct addrinfo	addrinfo;
typedef struct icmphdr	icmphdr;
typedef struct iphdr	iphdr;
typedef struct sockaddr	sockaddr;
typedef struct sockaddr_in	sockaddr_in;

typedef struct s_ping {
	char*	dest;
	char	ip_str[INET_ADDRSTRLEN];
	u_int16_t	flags;
	u_int16_t	err;
	char*	err_info;
	int	sockfd;
} t_ping;

void	t_ping_print(const t_ping* p);
t_ping*	t_ping_construct(int ac, char**av);
u_int16_t	t_ping_destruct(t_ping* p);

unsigned short checksum(void *b, int len);
void print_ip_address(struct sockaddr *addr, socklen_t addrlen);
void	print_icmphdr(const struct icmphdr* si);


#endif