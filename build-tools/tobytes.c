#include <stdio.h>
#include <stdlib.h>

#ifdef _WIN32
#include <winsock2.h>
#else
#include <arpa/inet.h>
#endif

int main(int argc, char *argv[]) {
    if (argc != 3) {
        printf("Usage: %s <port> <IP_address>\n", argv[0]);
        return 1;
    }

    unsigned short port = atoi(argv[1]);
    const char *IpAddtess = argv[2];

#ifdef _WIN32
    unsigned long IpAddressConverted = inet_addr(IpAddtess);
    if (IpAddressConverted == INADDR_NONE) {
        printf("Invalid IP address\n");
        return 1;
    }
#else
    // Linux
    struct in_addr addr;
    if (inet_pton(AF_INET, IpAddtess, &addr) != 1) {
        printf("Invalid IP address\n");
        return 1;
    }

    unsigned long IpAddressConverted = addr.s_addr;
#endif

    unsigned short portConverted = htons(port);
#ifdef _WIN32
    printf("{\"ip\": \"%lu\",\"port\": \"%i\"}", IpAddressConverted, portConverted);
#else
    printf("{\"ip\": \"%li\",\"port\": \"%i\"}", IpAddressConverted, portConverted);
#endif

    return 0;
}

