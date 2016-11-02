#include <stdio.h>         // printf() and standard i/o functions.
#include <sys/socket.h>    // socket()
#include <arpa/inet.h>     // uint16_t htons()
#include <stdlib.h>        // atoi()
#include <netinet/in.h>    // struct soccaddr_in
#include <unistd.h>        // close() to close socket.
#include <string.h>        // strlen(), strstr()
#include <stddef.h>        // size_t type

#define MAXBUFFER 200000

/*
 * TODO
 * - create with host lookup.
*/

int remove_white_space(char from[], char to[]);

int main(int argc, char *argv[])
{
   char *get_msg;
   //char *foo;
   int sock;
   int result;
   //int i;
   unsigned int port;
   struct sockaddr_in name;

   char server_reply[MAXBUFFER] = {0};
   char stripped_buff[MAXBUFFER] = {0};
   //char link_marker[2] = "<a";

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

   puts("stripped buff is:");
   printf("%s\n\n", stripped_buff);
   

/*
   printf("\nFirst 1000 bytes\n\n");
   for (i=0; i<1000; i++)
      printf("%c", server_reply[i]);

   printf("\n\nTotal bytes received: %lu\n\n", strlen(server_reply));


   foo = strstr(server_reply, link_marker);

   printf("found: ");
   for (i=0;foo[i] != '>';i++)
      printf("%c", foo[i]);
   printf("%c", foo[i]);

   remove_white_space(server_reply, stripped_buff);

   printf("\n\n\n------END------\n\n");
*/

   return 0;
}


int remove_white_space(char from[], char to[])
{

   return 0;
}
