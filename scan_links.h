/* scan_links.h v0.01 2016/11/13 */

/*
 * Copyright (c) 2016 - 2016, Foo <foo@openbsd.org>
 *
 * Permission to use, copy, modify, and distribute this software for any
 * purpose with or without fee is hereby granted, provided that the above
 * copyright notice and this permission notice appear in all copies.
 *
 * THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL WARRANTIES
 * WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES OF
 * MERCHANTABILITY AND FITNESS. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR
 * ANY SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES
 * WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN AN
 * ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF
 * OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.
 */

#ifndef _SCAN_LINKS_H
#define _SCAN_LINKS_H

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

/* Define constants */
#define MAXBUFFER 2097152  // use a power of 2 for performance reasons

/* Define functions */
//int get_html(int argc, char *argv[]);
int get_html();
int remove_white_space(char from[], char to[]);
int find_links(char buff[]);
int check_link(char *host, char *port, char *link);

const char *port80 = "80";
const char *port443 = "443";

#endif
