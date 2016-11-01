#include <stdio.h>         // printf() and standard i/o functions.
#include <sys/socket.h>    // socket()
#include <arpa/inet.h>     // uint16_t htons()
//#include <stdlib.h>
#include <netinet/in.h>    // struct soccaddr_in
#include <unistd.h>        // close() to close socket.
#include <string.h>        // strlen()

#define PORT 80
#define IP "93.184.216.34"

int main()
{
   //uint16_t port = 80;
   char *get_msg;
   int sock;
   int result;
   struct sockaddr_in name;

   char server_reply[2000] = {0};

   //get_msg = "HEAD / HTTP/1.1\r\nHost: www.example.com\r\n\r\n";
   get_msg = "GET / HTTP/1.1\r\nHost:www.example.com\r\nConnection:close\r\n\r\n";
   name.sin_family = AF_INET;
   name.sin_port = htons(PORT);
   name.sin_addr.s_addr = inet_addr(IP);
   
   if ( (sock = socket(AF_INET, SOCK_STREAM, 0)) < 0)
      puts("Socket Creation Failed!");
   puts("Socket Created");

   sleep(5);

   if ( (connect(sock, (struct sockaddr *)&name, sizeof(name))) < 0)
      puts("Connect Failed!");
   puts("Connected");

   if ( (send(sock, get_msg, strlen(get_msg), 0)) < 0)
      puts("Send Failed!");
   puts("Data Sent, ready to recv.");

   sleep(5);
   //while ( (recv(sock, server_reply, 2000, MSG_WAITALL)) > 0)
   while ( (result = recv(sock, server_reply, 2000, 0)) > 0)
   {
      //if ( (recv(sock, server_reply, 2000, 0)) < 0)
      puts("Data Received");
      printf("result = %d\n", result);
      printf("data = %s\n\n", server_reply);
   }

   if ( (close(sock)) < 0)
      puts("Close Socket Failed!");
   //puts("Socket Closed");

   //puts("Message =");
   //printf("%s \n\nDONE\n\n\n", server_reply);
   //printf("%s", server_reply);

   return 0;
}
