#include "ft_ping.h"

int	main(int argc, char** argv) {
	t_ping*	p = t_ping_construct(argc, argv);
	if (!p) {
		dprintf(2, "Memory allocation error\n");
		return -1;
	}
	t_ping_print(p);
	if (p->err)
		return t_ping_destruct(p);

//	return t_ping_destruct(p);

	// packets....
// set sockaddr_in
    sockaddr_in  destaddr;
	bzero(&destaddr, sizeof(destaddr));
	destaddr.sin_family = AF_INET;

	if (inet_pton(AF_INET, argv[1], &destaddr.sin_addr) <= 0)
	{
		printf("Error inet_pton\n");
	//	close(sockfd);
		return (2);
	}

//	return t_ping_destruct(p);

	int sockfd = p->sockfd;

// set packet to send
	char	packet[64];
	bzero(&packet, sizeof(packet));

	icmphdr *icmp = (icmphdr *)packet;
    icmp->type = ICMP_ECHO;     // Type 8
    icmp->code = 0;             // Code 0
    icmp->checksum = 0;         // Initialisé à 0 avant le calcul
    icmp->un.echo.id = htons(getpid()); // ID du processus
    icmp->un.echo.sequence = 0;  // Numéro de séquence

    // Ajout d'une charge utile (payload) optionnelle
 //   memset(packet + sizeof(struct icmphdr), 'A', 56);

    icmp->checksum = checksum(packet, sizeof(packet));

	printf("\n\tSENT:\n");
	print_icmphdr((icmphdr*)&packet);

	printf("FT_PING\n"); /////

// send packet
    if (sendto(sockfd, packet, sizeof(packet), 0, (sockaddr *)&destaddr, sizeof(destaddr)) < 0) {
        perror("Erreur lors de l'envoi du paquet");
        close(sockfd);
        return 1;
    }

// receive reply
	char	buf[1024];
	bzero(&buf, sizeof(buf));
	socklen_t	socklen = sizeof(destaddr);
	if (recvfrom(sockfd, buf, sizeof(buf), 0, (sockaddr *)&destaddr, &socklen) < 0) {
		perror("Erreur lors de la reception de la reponse");
		close(sockfd);
		return 1;
	}

	iphdr*	ip_header = (iphdr*)buf;
	int	ip_header_len = ip_header->ihl * 4;

	if (ip_header->protocol != IPPROTO_ICMP)
	{
		printf("Error: received non ICMP packet. (%d)\n", ip_header->protocol);
		close(sockfd);
		return (1);
	}

	printf("\n\tRECEIVED (ip_header_len = %d):\n", ip_header_len);
	print_icmphdr((icmphdr*)(buf + ip_header_len));

	close(sockfd);
}