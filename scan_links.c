/*
 * TODO
 * - create with host lookup.
*/

/* Include headers */
#include <ctype.h>         // tolower()
#include <stdio.h>         // printf() and standard i/o functions.
#include <sys/socket.h>    // socket()
#include <arpa/inet.h>     // uint16_t htons()
#include <stdlib.h>        // atoi()
#include <netinet/in.h>    // struct soccaddr_in
#include <unistd.h>        // close() to close socket.
#include <string.h>        // strlen(), strstr()
#include <stddef.h>        // size_t type

/* Define constants */
#define MAXBUFFER 200000

/* Define functions */
int remove_white_space(char from[], char to[]);
int find_links(char buff[]);

int main(int argc, char *argv[])
{
   char *get_msg;
   int sock;
   int result;
   //int i;
   unsigned int port;
   struct sockaddr_in name;

   char server_reply[MAXBUFFER] = {0};
   char stripped_buff[MAXBUFFER] = {0};

   if ( argc != 3 )
   {
      printf("Usage: scan_links <server_ip> <port>\n");
      return -1;
   }

   // get the port.
   port = atoi(argv[2]);

   // fill the sockaddr_in struct.
   name.sin_family = AF_INET;
   name.sin_port = htons(port);
   inet_aton(argv[1], &name.sin_addr);

   //get_head = "HEAD / HTTP/1.1\r\nHost: www.example.com\r\n\r\n";
   get_msg = "GET / HTTP/1.1\r\nHost:www.openbsd.org\r\nConnection:close\r\n\r\n";
   
   if ( (sock = socket(AF_INET, SOCK_STREAM, 0)) < 0)
      puts("Socket Creation Failed!");
   puts("Socket Created");

   sleep(1);

   if ( (connect(sock, (struct sockaddr *)&name, sizeof(name))) < 0)
      puts("Connect Failed!");
   puts("Connected");

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
   if ( (close(sock)) < 0)
      puts("Close Socket Failed!");
   puts("Socket Closed");

   puts("Removing White Space");
   if ( (remove_white_space(server_reply, stripped_buff) < 0))
      puts("Remove White Space Failed!");
   puts("White Space Removed");

   
   puts("find links");
   if ( (find_links(stripped_buff)) < 0)
      puts("Find Links Failed!");
   puts("\nFind Links Done");

/*
   printf("\nFirst 1000 bytes\n\n");
   for (i=0; i<1000; i++)
      printf("%c", server_reply[i]);

   printf("\n\nTotal bytes received: %lu\n\n", strlen(server_reply));




   remove_white_space(server_reply, stripped_buff);

   printf("\n\n\n------END------\n\n");
*/

   return 0;
}


int find_links(char buff[])
{
   char *pos = buff;
   char link_marker[7] = "ahref=\"";
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
