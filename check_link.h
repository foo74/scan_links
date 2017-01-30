#ifndef _CHECK_LINK_H
#define _CHECK_LINK_H

/* Include headers */
#include <stdio.h>         // printf() and standard i/o functions
#include <ctype.h>         // tolower()
#include <string.h>        // strlen(), strncat(), strstr()
#include <sys/types.h>
#include <sys/socket.h>    // socket(), connect()
#include <unistd.h>        // close() to close socket, sleep() to delay
#include <netdb.h>
#include <arpa/inet.h>     // uint16_t htons()
#include <netinet/in.h>    // struct sockaddr_in
#include <stdlib.h>        // atoi() and exit()
#include <openssl/ssl.h>   // For https comms

/* Define functions */
int check_link(char *address, char *port, char *link);

#endif
