/*
 * Scan for broken links. Supports HTTP (port 80) and HTTPS (port 443).
 */

#include "scan_links.h"

int main(int argc, char *argv[])
{
   // If hostname and port not specified then show usage.
   if (argc != 3) 
   {
       fprintf(stderr,"usage: scan_links hostname port\n");
       return 1;
   }

   //get_html(argc, argv);

   char *address = "www.openbsd.org";
   char *port = "443";

   get_html(address, port);

   //check_link(argv[1], argv[2], "/60.html");
/*
   if ( (check_link(argv[1], argv[2], "/")) == 1 )
      puts("LINK OK");
   else
      puts("INVALID LINK!");
*/

   return 0;
}

/*
 * Get all of the html assocated with the given address and port.
 * Example: scan_links www.openbsd.org 443 would get all the 
 * html at www.openbsd.org on port 443 (SSL).
 */
int get_html(char *address, char *port)
{
   char http_request[3000];
   char server_reply[MAXBUFFER];
   char stripped_buff[MAXBUFFER];
   struct addrinfo hints, *res; 
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


   // BUILD THE HTTP GET REQUEST WITH HOSTNAME FROM COMMAND LINE
   strncat(http_request, "GET / HTTP/1.1\r\nHost:", 1000);
   strncat(http_request, address, 1000);
   strncat(http_request, "\r\nConnection:close\r\n\r\n", 1000);

   hints.ai_family = AF_UNSPEC; // AF_INET or AF_INET6 to force version
   hints.ai_socktype = SOCK_STREAM;


   if ((status = getaddrinfo(address, port, &hints, &res)) != 0) 
   {
      fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(status));
      exit(0);  // TODO: Need to make the status codes meaningful.
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
      while ( (bytes_received = SSL_read(conn, server_reply + recv_index, MAXBUFFER-recv_index)) > 0)
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

   // REMOVE WHITE SPACE
   puts("Removing White Space");
   if ( (remove_white_space(server_reply, stripped_buff) < 0))
      puts("Remove White Space Failed!");
   puts("White Space Removed");

   // LOOK FOR LINKS
   puts("Looking for links...");
   if ( (find_links(stripped_buff)) < 0)
      puts("Find Links Failed!");
   puts("Find Links Done");

   return 0;
}

// Find links in the HTTP.
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

// Remove white space so results are easier to parse.
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
