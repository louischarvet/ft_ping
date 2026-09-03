#include "ft_ping.h"

unsigned short checksum(void *b, int len) {
    unsigned short *buf = b;
    unsigned int sum = 0;
    unsigned short result;

    for (sum = 0; len > 1; len -= 2)
        sum += *buf++;
    if (len == 1)
        sum += *(unsigned char *)buf;
    sum = (sum >> 16) + (sum & 0xFFFF);
    sum += (sum >> 16);
    result = ~sum;
    return result;
}

void print_ip_address(struct sockaddr *addr, socklen_t addrlen) {
    char ipstr[INET6_ADDRSTRLEN];
    void *addr_ptr;

    if (addr->sa_family == AF_INET) {
        struct sockaddr_in *ipv4 = (struct sockaddr_in *)addr;
        addr_ptr = &(ipv4->sin_addr);
    } else if (addr->sa_family == AF_INET6) {
        struct sockaddr_in6 *ipv6 = (struct sockaddr_in6 *)addr;
        addr_ptr = &(ipv6->sin6_addr);
    } else {
        printf("Unsupported address family\n");
        return;
    }

    inet_ntop(addr->sa_family, addr_ptr, ipstr, sizeof(ipstr));
    printf("IP Address: %s\n", ipstr);
}

void	print_icmphdr(const struct icmphdr* si) {
	printf("type = %u\ncode = %u\nchecksum = %u\necho.id = %u\necho.sequence = %u\n",
		si->type, si->code, si->checksum, si->un.echo.id, si->un.echo.sequence);
}