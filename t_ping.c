#include "ft_ping.h"

const char*	error_strs[] = {
	"No error.",
	ERRSTR_1,
	ERRSTR_2,
	ERRSTR_3,
	ERRSTR_4,
	ERRSTR_5
};

void	t_ping_print(const t_ping* p)
{
	uint16_t	flags = p->flags;
	printf("T_PING STRUCTURE\n\tdest = %s\n\tip_str = %s\n\tflags:\n\t\tverbose = %u\n\t\thelp = %u\n\t\tsockfd = %d\n",
		p->dest,
		p->ip_str,
		!!(flags & FLAG_VERBOSE),
		!!(flags & FLAG_HELP),
		p->sockfd);
}

static void	t_ping_resolve_dns(t_ping* p) {
	addrinfo	hints, *result;
	bzero(&hints, sizeof(hints));
	hints.ai_family = AF_INET;
	hints.ai_socktype = SOCK_RAW;

	int gai = getaddrinfo(p->dest, NULL, &hints, &result);
	if (gai != 0) { ////////////
		p->err = GAI_ERROR; ////
		p->err_info = strdup(
			gai_strerror(gai));
	//	freeaddrinfo(result);
		return;
	}
	for (addrinfo* rp = result; rp != NULL; rp = rp->ai_next) ////////////
		inet_ntop(
			AF_INET,
			&((sockaddr_in*)(rp->ai_addr))->sin_addr,
			p->ip_str,
			INET_ADDRSTRLEN);

	freeaddrinfo(result);
}

static void	t_ping_set_socket(t_ping* p) {
	int	fd = socket(AF_INET, SOCK_RAW, IPPROTO_ICMP);
	if (fd < 0)
		p->err = ERR_SOCKET;
	else
		p->sockfd = fd;
}

t_ping*	t_ping_construct(int ac, char** av) {
	t_ping*	p = calloc(1, sizeof(t_ping));
	if (!p)
		return NULL;

	if (ac < 2)
		p->err = USAGE_DESTADDR_REQ;

	for (int i = 1; i < ac; i++) {
		if (av[i][0] == '-') {
			switch (av[i][1]) {
				case 'v':
					p->flags |= FLAG_VERBOSE;
					break;
				case '?':
					p->flags |= FLAG_HELP;
					break;
				case 'h':
					p->flags |= FLAG_HELP;
					break;
				default:
					p->err = INVALID_OPT;
					break;
			}
		} else if (!p->dest) {
			p->dest = strdup(av[i]);
			if (!p->dest)
				return NULL;
		} else {
			p->err = RECRTE_OPNOTPERM;
			p->err_info = strdup("record route");
			break;
		}
	}
	if (!p->err)
		t_ping_resolve_dns(p);
	if (!p->err)
		t_ping_set_socket(p);
	return p;
}

uint16_t	t_ping_destruct(t_ping* p) {
	uint16_t	err = p->err;
	printf("err = %u\n", err);

	if (err == GAI_ERROR) //////////////////////////////
		dprintf(2, "ft_ping: %s\n", p->err_info);
	else if (err)
		dprintf(2, "ft_ping: %s: %s\n", p->err_info, error_strs[err]);

	if (p->dest)
		free(p->dest);
	if (p->sockfd > 2)
		close(p->sockfd);

	free(p);
/*
Success: code 0
No reply: code 1
Other errors: code 2
*/
	return err; ////
}