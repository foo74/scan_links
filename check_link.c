/*
 * Scan for broken links. Supports HTTP (port 80) and HTTPS (port 443).
 */

#include "check_link.h"

/* Define constants */
#define MAXBUFFER 2097152  // use a power of 2 for performance reasons

int check_link(char *host, char *port, char *link)
{
   char *port80 = "80";
   char *port443 = "443";
   char http_request[5000];
   char server_reply[MAXBUFFER];
   struct addrinfo hints, *res, *p;
   char ipstr[INET6_ADDRSTRLEN];
   int status = 0;                     // status of getaddrinfo()
   int bytes_received = 0;             // how many bytes we receive from GET request.
   int sock = 0;                       // descriptor for our socket.
   int recv_index = 0;

   // Zero out the arrays and structs.
   memset(&http_request, 0, sizeof http_request);
   memset(&server_reply, 0, sizeof server_reply);
   memset(&hints, 0, sizeof hints);

   // BUILD THE HTTP GET REQUEST WITH HOSTNAME FROM COMMAND LINE
   strncat(http_request, "HEAD ", 1000);
   strncat(http_request, link, 1000);
   strncat(http_request, " HTTP/1.1\r\nHost:", 1000);
   strncat(http_request, host, 1000);
   strncat(http_request, "\r\nConnection:close\r\n\r\n", 1000);

   hints.ai_family = AF_UNSPEC; // AF_INET or AF_INET6 to force version
   hints.ai_socktype = SOCK_STREAM;

   if ((status = getaddrinfo(host, port, &hints, &res)) != 0) 
   {
      fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(status));
      return 2;
   }

   printf("IP addresses for %s:\n\n", host);

   for(p = res;p != NULL; p = p->ai_next) 
   {
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

   // CREATE SOCKET
   if ( (sock = socket(res->ai_family, res->ai_socktype, res->ai_protocol)) < 0)
      puts("Socket Creation Failed!");
   puts("Socket Created");

   // CONNECT TO SOCKET
   if ( (connect(sock, res->ai_addr, res->ai_addrlen)) < 0)
      puts("Connect Failed!");
   puts("Connected");

   if ( (strcmp(port, port443)) == 0 )
   {
      // INIT SSL
      SSL_load_error_strings();
      SSL_library_init();
      SSL_CTX *ssl_ctx = SSL_CTX_new (SSLv23_client_method());

      // CREATE SSL AND ATTACH TO SOCKET
      SSL *conn = SSL_new(ssl_ctx);
      if ( (SSL_set_fd(conn, sock)) != 1)
         puts("SSL Set File Descriptor Failed!");
      puts("SSL Set File Descriptor");

      // CONNECT SSL
      if ( (SSL_connect(conn)) != 1)
         puts("SSL Connect Failed!");
      puts("SSL Connected");

      // SEND HTTP MESSAGE
      if ( (SSL_write(conn, http_request, strlen(http_request))) < 0)
         puts("Send Failed!");
      puts("Data Sent, ready to recv.");

      // RECEIVE HTTP MESSAGE
      while ( (bytes_received = SSL_read(conn, server_reply + recv_index, MAXBUFFER-recv_index)) > 0)
      {
         recv_index += bytes_received;
         printf("Bytes Received = %d\n", bytes_received);
      }
   }
   else if ( (strcmp(port, port80)) == 0 )
   {
      // SEND HTTP MESSAGE
      if ( (send(sock, http_request, strlen(http_request), 0)) < 0)
         puts("Send Failed!");
      puts("Data Sent, ready to recv.");

      // RECEIVE HTTP MESSAGE
      while ( (bytes_received = recv(sock, server_reply + recv_index, MAXBUFFER-recv_index, 0)) > 0)
      {
         recv_index += bytes_received;
         puts("Data Received");
         printf("Bytes Received = %d\n", bytes_received);
      }
   }

   // CLOSE SOCKET
   if ( (close(sock)) < 0)
      puts("Close Socket Failed!");
   puts("Socket Closed");

   freeaddrinfo(res); // free the linked list

   printf("results is: \n\n%s\n\n", server_reply);
   if ( strstr(server_reply, "200 OK") )
      return 1;

/*
   check_link("www.openbsd.org", "80", "/60.html");
   if ( (check_link(argv[1], argv[2], "/")) == 1 )
      puts("LINK OK");
   else
      puts("INVALID LINK!");
*/

   return 0;
}
