/*
 * Scan for broken links. 
 */

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
#include <stdlib.h>        // atoi()
#include <openssl/ssl.h>   // For https comms

/* Define constants */
#define MAXBUFFER 2097152  // use a power of 2 for performance reasons

/* Define functions */
int remove_white_space(char from[], char to[]);
int find_links(char buff[]);

int main(int argc, char *argv[])
{
   char http_request[3000];
   char server_reply[MAXBUFFER];
   char stripped_buff[MAXBUFFER];
   struct addrinfo hints, *res, *p;
   char ipstr[INET6_ADDRSTRLEN];
   int status = 0;                     // status of getaddrinfo()
   int bytes_received = 0;             // how many bytes we receive from GET request.
   int sock = 0;                       // descriptor for our socket.
   int recv_index = 0;

   // Zero out the arrays and structs.
   memset(&http_request, 0, sizeof http_request);
   memset(&server_reply, 0, sizeof server_reply);
   memset(&stripped_buff, 0, sizeof stripped_buff);
   memset(&hints, 0, sizeof hints);
   memset(&ipstr, 0, sizeof ipstr);

   // If hostname and port not specified then show usage.
   if (argc != 3) {
       fprintf(stderr,"usage: scan_links hostname port\n");
       return 1;
   }

   // BUILD THE HTTP GET REQUEST WITH HOSTNAME FROM COMMAND LINE
   strncat(http_request, "GET / HTTP/1.1\r\nHost:", 1000);
   strncat(http_request, argv[1], 1000);
   strncat(http_request, "\r\nConnection:close\r\n\r\n", 1000);

   hints.ai_family = AF_UNSPEC; // AF_INET or AF_INET6 to force version
   hints.ai_socktype = SOCK_STREAM;

   if ((status = getaddrinfo(argv[1], argv[2], &hints, &res)) != 0) {
      fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(status));
      return 2;
   }

   printf("IP addresses for %s:\n\n", argv[1]);

   for(p = res;p != NULL; p = p->ai_next) {
      void *addr;
      char *ipver;

      // get the pointer to the address itself,
      // different fields in IPv4 and IPv6:
      if (p->ai_family == AF_INET) { // IPv4
         struct sockaddr_in *ipv4 = (struct sockaddr_in *)p->ai_addr;
         addr = &(ipv4->sin_addr);
         ipver = "IPv4";
      } else { // IPv6
         struct sockaddr_in6 *ipv6 = (struct sockaddr_in6 *)p->ai_addr;
         addr = &(ipv6->sin6_addr);
         ipver = "IPv6";
      }

      // convert the IP to a string and print it:
      inet_ntop(p->ai_family, addr, ipstr, sizeof ipstr);
      printf("  %s: %s\n", ipver, ipstr);
   }

   //sleep(1);

   // CREATE TO SOCKET
   if ( (sock = socket(res->ai_family, res->ai_socktype, res->ai_protocol)) < 0)
      puts("Socket Creation Failed!");
   puts("Socket Created");


   //sleep(1);

   // CONNECT TO SOCKET
   if ( (connect(sock, res->ai_addr, res->ai_addrlen)) < 0)
      puts("Connect Failed!");
   puts("Connected");

   //sleep(1);

// INIT SSL
SSL_load_error_strings();
SSL_library_init();
SSL_CTX *ssl_ctx = SSL_CTX_new (SSLv23_client_method());

   // SEND HTTP MESSAGE
   if ( (send(sock, http_request, strlen(http_request), 0)) < 0)
      puts("Send Failed!");
   puts("Data Sent, ready to recv.");

   //sleep(1);

   // RECEIVE HTTP MESSAGE
   while ( (bytes_received = recv(sock, server_reply + recv_index, MAXBUFFER-recv_index, 0)) > 0)
   {
      recv_index += bytes_received;
      puts("Data Received");
      printf("Bytes Received = %d\n", bytes_received);
   }

   //sleep(1);

   // CLOSE SOCKET
   if ( (close(sock)) < 0)
      puts("Close Socket Failed!");
   puts("Socket Closed");

   freeaddrinfo(res); // free the linked list

   //sleep(1);

   // REMOVE WHITE SPACE
   puts("Removing White Space");
   if ( (remove_white_space(server_reply, stripped_buff) < 0))
      puts("Remove White Space Failed!");
   puts("White Space Removed");

   //sleep(1);
   
   // LOOK FOR LINKS
   puts("Looking for links...");
   if ( (find_links(stripped_buff)) < 0)
      puts("Find Links Failed!");
   puts("Find Links Done");

   return 0;
}

int find_links(char buff[])
{
//printf("buff=%s\n\n", buff);
   char *pos = buff;
   char link_marker[] = "ahref=\"";
   int i = 0;

   // pos is pointer to start of ahref=" string. Returns NULL if not found.
   while ( (pos = strstr(pos, link_marker)) )
   {
      // Increase size of marker so we don't loop forever.
      pos += 7;
      i = 0;      // start at 0 since this is the start of pos.

      printf("found link:  ");
      while ( pos[i] != '\"' && pos[i] != '\0' )
      {
         printf("%c", pos[i]);
         i++;
      }

      printf("\n");

   }

   return 0;
}

int remove_white_space(char from[], char to[])
{
   int from_pos = 0;
   int to_pos = 0;

   while (from[from_pos] != '\0')
      if (from[from_pos] == ' ' || from[from_pos] == '\r' || from[from_pos] == '\n' || from[from_pos] == '\t' || from[from_pos] == '\f')
         from_pos++;
      else
      {
         to[to_pos++] = tolower(from[from_pos++]);
      }

   return 0;
}