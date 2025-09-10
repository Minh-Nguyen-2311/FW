/*
 * network.c
 *
 *  Created on: Aug 9, 2025
 *      Author: Nguyen Trieu
 */

#include "network.h"
#include "lwip/sockets.h"
#include "string.h"
#include "lwip/tcpip.h"
#include "lwip/ip4_addr.h"
#include "lwip/inet.h"
#include "lwip.h"

#define ETH_SERVER_IP		"192.168.1.100"
#define ETH_SERVER_PORT		503

//int NetworkConnect(Network* n, char* host, int port)
//{
//    struct sockaddr_in server_addr;
//    n->my_socket = lwip_socket(AF_INET, SOCK_STREAM, 0);
//    if (n->my_socket < 0) {
//        return -1;
//    }
//
//    memset(&server_addr, 0, sizeof(server_addr));
//    server_addr.sin_family = AF_INET;
//    server_addr.sin_port = htons(port);
//
//    // Nếu host là dạng IP "192.168.1.100"
//    server_addr.sin_addr.s_addr = inet_addr(host);
//
//    if (lwip_connect(n->my_socket, (struct sockaddr*)&server_addr, sizeof(server_addr)) < 0) {
//        lwip_close(n->my_socket);
//        return -1;
//    }
//    return 0;
//}

int NetworkConnect(Network* n)
{
   // struct sockaddr_in server_addr;
    struct sockaddr_in server_addr = {
    		.sin_len = sizeof(server_addr),
    		.sin_family = AF_INET,
    		.sin_port = PP_HTONS(ETH_SERVER_PORT),
    		.sin_addr.s_addr = inet_addr(ETH_SERVER_IP),
    	};

    // Tạo socket TCP
    n->my_socket = lwip_socket(PF_INET, SOCK_STREAM, 0); // thay 0 -> IPPROTO_TCP để rõ nghĩa
    if (n->my_socket < 0) {
        return -1;
    }


    // Kết nối đến server
    if (lwip_connect(n->my_socket, (const struct sockaddr*)&server_addr, sizeof(server_addr)) < 0) {
        lwip_close(n->my_socket);
        return -1;
    }

    return 0;
}
int NetworkInit(Network* n)
{
    n->my_socket = -1;
    n->mqttread = NetworkRead;
    n->mqttwrite = NetworkWrite;
    n->disconnect = NetworkDisconnect;
    return 0;
}

int NetworkRead(Network* n, unsigned char* buffer, int len, int timeout_ms)
{
    if (n->my_socket < 0) return -1;

    struct timeval tv;
    tv.tv_sec = timeout_ms / 1000;
    tv.tv_usec = (timeout_ms % 1000) * 1000;

    /* set socket recv timeout */
    lwip_setsockopt(n->my_socket, SOL_SOCKET, SO_RCVTIMEO, &tv, sizeof(tv));

    int received = 0;
    int rc = lwip_recv(n->my_socket, buffer, len, 0);
    if (rc < 0) {
        /* EWOULDBLOCK or timeout returns -1 and errno=EWOULDBLOCK or EAGAIN depending on LwIP port */
        return rc;
    }
    received = rc;
    return received;
}

int NetworkWrite(Network* n, unsigned char* buffer, int len, int timeout_ms)
{
    if (n->my_socket < 0) return -1;

    struct timeval tv;
    tv.tv_sec = timeout_ms / 1000;
    tv.tv_usec = (timeout_ms % 1000) * 1000;

    lwip_setsockopt(n->my_socket, SOL_SOCKET, SO_SNDTIMEO, &tv, sizeof(tv));

    int sent = 0;
    int rc = lwip_send(n->my_socket, buffer, len, 0);
    if (rc < 0) {
        return rc;
    }
    sent = rc;
    return sent;
}

void NetworkDisconnect(Network* n)
{
    if (n->my_socket >= 0) {
        lwip_close(n->my_socket);
        n->my_socket = -1;
    }
}
