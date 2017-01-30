/*
 * Gets requested HTML.
 */

#include "get_html.h"
#include "get_links.h"

/* Define constants */
#define MAXBUFFER 2097152  // use a power of 2 for performance reasons

/*
 * Get all of the html assocated with the given address and port.
 * Example: scan_links www.openbsd.org 443 would get all the 
 * html at www.openbsd.org on port 443 (SSL).
 */
int get_html(char *address, char *port, char buff[])
{
   char *port80 = "80";
   char *port443 = "443";
   char http_request[3000];
   //char server_reply[MAXBUFFER];
   //char stripped_buff[MAXBUFFER];
   struct addrinfo hints, *res; 
   char ipstr[INET6_ADDRSTRLEN];
   int status = 0;                     // status of getaddrinfo()
   int bytes_received = 0;             // how many bytes we receive from GET request.
   int sock = 0;                       // descriptor for our socket.
   int recv_index = 0;


   // Zero out the arrays and structs.
   memset(&http_request, 0, sizeof http_request);
   //memset(&server_reply, 0, sizeof server_reply);
   //memset(&buff, 0, sizeof buff);
   memset(&hints, 0, sizeof hints);
   memset(&ipstr, 0, sizeof ipstr);


   // BUILD THE HTTP GET REQUEST WITH HOSTNAME FROM COMMAND LINE
   strncat(http_request, "GET / HTTP/1.1\r\nHost:", 1000);
   strncat(http_request, address, 1000);
   strncat(http_request, "\r\nConnection:close\r\n\r\n", 1000);

   hints.ai_family = AF_UNSPEC; // AF_INET or AF_INET6 to force version
   hints.ai_socktype = SOCK_STREAM;


   if ((status = getaddrinfo(address, port, &hints, &res)) != 0) 
   {
      fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(status));
      exit(0);
   }

   // CREATE SOCKET
   if ( (sock = socket(res->ai_family, res->ai_socktype, res->ai_protocol)) < 0)
      puts("Socket Creation Failed!");
   else
      puts("Socket Created");

   // CONNECT TO SOCKET
   if ( (connect(sock, res->ai_addr, res->ai_addrlen)) < 0)
   {
      puts("Connect Failed!");
      exit(0);
   }
   else
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
      //while ( (bytes_received = SSL_read(conn, server_reply + recv_index, MAXBUFFER-recv_index)) > 0)
      while ( (bytes_received = SSL_read(conn, buff + recv_index, MAXBUFFER-recv_index)) > 0)
      {
         recv_index += bytes_received;
         puts("Data Received");
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
      //while ( (bytes_received = recv(sock, server_reply + recv_index, MAXBUFFER-recv_index, 0)) > 0)
      while ( (bytes_received = recv(sock, buff + recv_index, MAXBUFFER-recv_index, 0)) > 0)
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

   //buff = server_reply;

   //find_links(server_reply);

/*
   int i = 0;
   for (i = 0; server_reply[i] != 0; i++)
      printf("%c", server_reply[i]);
*/
   
   return 0;
}
