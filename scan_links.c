#include <stdio.h>         // printf() and standard i/o functions.
#include <sys/socket.h>    // socket()
#include <arpa/inet.h>     // uint16_t htons()
//#include <stdlib.h>
#include <string.h>
#include <netinet/in.h>    // struct soccaddr_in
#include <unistd.h>        // close() to close socket.
#include <string.h>        // strlen()

#define PORT 80
#define IP "129.128.5.194"

int main()
{
   //uint16_t port = 80;
   char *get_msg;
   char *foo;
   int sock;
   int result;
   int i;
   //int total;
   struct sockaddr_in name;

   char server_reply[200000] = {0};
   char link_marker[2] = "<a";

   //get_msg = "HEAD / HTTP/1.1\r\nHost: www.example.com\r\n\r\n";
   get_msg = "GET / HTTP/1.1\r\nHost:www.openbsd.org\r\nConnection:close\r\n\r\n";
   name.sin_family = AF_INET;
   name.sin_port = htons(PORT);
   name.sin_addr.s_addr = inet_addr(IP);
   
   if ( (sock = socket(AF_INET, SOCK_STREAM, 0)) < 0)
      puts("Socket Creation Failed!");
   puts("Socket Created");

   sleep(2);

   if ( (connect(sock, (struct sockaddr *)&name, sizeof(name))) < 0)
      puts("Connect Failed!");
   puts("Connected");

   if ( (send(sock, get_msg, strlen(get_msg), 0)) < 0)
      puts("Send Failed!");
   puts("Data Sent, ready to recv.");

   sleep(2);
   //while ( (recv(sock, server_reply, 2000, MSG_WAITALL)) > 0)
   while ( (result = recv(sock, server_reply, 200000, 0)) > 0)
   {
      //if ( (recv(sock, server_reply, 2000, 0)) < 0)
      puts("Data Received");
      printf("Bytes Received = %d\n", result);
      //printf("data = %s\n\n", server_reply);
   }

   sleep(2);
   if ( (close(sock)) < 0)
      puts("Close Socket Failed!");
   puts("Socket Closed");

   //puts("Message =");
   //printf("%s \n\nDONE\n\n\n", server_reply);
   //printf("%s", server_reply);

   printf("\nFirst 1000 bytes\n\n");
   for (i=0; i<1000; i++)
      printf("%c", server_reply[i]);

   //for (total=0; server_reply[total] != 0; total++)
      //; //printf(".");
   printf("\n\nTotal bytes received: %lu\n\n", strlen(server_reply));


   foo = strstr(server_reply, link_marker);

   printf("found: ");
   for (i=0;foo[i] != '>';i++)
      printf("%c", foo[i]);
   printf("%c", foo[i]);

   printf("\n\n\n------END------\n\n");


   return 0;
}
