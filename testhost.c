#include <stdio.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netdb.h>

void main(int argc, char *argv[])
{
   char *hostname;
   hostname = argv[1];

   struct hostent *hostinfo;
   hostinfo = gethostbyname(hostname);

   printf("official name of host = %s\n", hostinfo->h_name);
   printf("alias list = %s\n", hostinfo->h_aliases);
}
