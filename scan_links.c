/*
 * Scan for broken links. 
 */

#include <ctype.h>         // tolower()
#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>        // close() to close socket.
#include <netdb.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <stdlib.h>        // atoi()

/* Include headers */
/*
#include <stdio.h>         // printf() and standard i/o functions.
#include <sys/socket.h>    // socket()
#include <arpa/inet.h>     // uint16_t htons()
#include <netinet/in.h>    // struct soccaddr_in
#include <unistd.h>        // close() to close socket.
#include <string.h>        // strlen(), strstr()
#include <stddef.h>        // size_t type
*/


/* Define functions */
int remove_white_space(char from[], char to[]);
int find_links(char buff[]);

int main(int argc, char *argv[])
{
   struct addrinfo hints, *res, *p;
   int status;
   int sock = 0; // descriptor for our socket.
   char ipstr[INET6_ADDRSTRLEN];
   //unsigned int port; // hold the port number likely 80 or 443.

   if (argc != 3) {
       fprintf(stderr,"usage: scan_links hostname port\n");
       return 1;
   }

   // get the port.
   //port = atoi(argv[2]);

   memset(&hints, 0, sizeof hints);
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

   // CREATE TO SOCKET
   if ( (sock = socket(res->ai_family, res->ai_socktype, res->ai_protocol)) < 0)
      puts("Socket Creation Failed!");
   puts("Socket Created");


   // CONNECT TO SOCKET
   if ( (connect(sock, res->ai_addr, res->ai_addrlen)) < 0)
      puts("Connect Failed!");
   puts("Connected");



   // CLOSE SOCKET
   if ( (close(sock)) < 0)
      puts("Close Socket Failed!");
   puts("Socket Closed");

   freeaddrinfo(res); // free the linked list

   return 0;
}






/* Define constants */
#define MAXBUFFER 200000


/*
int main(int argc, char *argv[])
{
   char *get_msg;
   int sock;
   int result;
   unsigned int port;
   struct sockaddr_in name;

   char server_reply[MAXBUFFER] = {0};
   char stripped_buff[MAXBUFFER] = {0};


   //get_head = "HEAD / HTTP/1.1\r\nHost: www.example.com\r\n\r\n";
   get_msg = "GET / HTTP/1.1\r\nHost:www.openbsd.org\r\nConnection:close\r\n\r\n";
   

   sleep(1);


   if ( (send(sock, get_msg, strlen(get_msg), 0)) < 0)
      puts("Send Failed!");
   puts("Data Sent, ready to recv.");

   sleep(1);

   // Keep getting recv until we get all the bytes.
   while ( (result = recv(sock, server_reply, 200000, 0)) > 0)
   {
      puts("Data Received");
      printf("Bytes Received = %d\n", result);
   }

   sleep(1);

   puts("Removing White Space");
   if ( (remove_white_space(server_reply, stripped_buff) < 0))
      puts("Remove White Space Failed!");
   puts("White Space Removed");

   
   puts("Looking for links...");
   if ( (find_links(stripped_buff)) < 0)
      puts("Find Links Failed!");
   puts("\nFind Links Done");


   return 0;
}

*/

int find_links(char buff[])
{
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

      // start at 7 to get to start of link.
      //for (i=7;pos[i] != '\"';i++)
         //printf("%c", pos[i]);
   }

/*
   for ( i = 7; buff[i] != '\0' && i < MAXBUFFER; i++)
*/

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
         //printf("char is: %c and code is %d\n", from[from_pos], from[from_pos]);
         to[to_pos++] = tolower(from[from_pos++]);
      }

   return 0;
}
