/*
 * Scan for broken links. Supports HTTP (port 80) and HTTPS (port 443).
 */

#include "get_html.h"
#include "get_links.h"

/* Define constants */
#define MAXBUFFER 2097152  // use a power of 2 for performance reasons

int main(int argc, char *argv[])
{
/*
   // If hostname and port not specified then show usage.
   if (argc != 3) 
   {
       fprintf(stderr,"usage: scan_links hostname port\n");
       return 1;
   }
*/

   char *domain = "www.openbsd.org";
   char *port = "443";
   char buff[MAXBUFFER] = { 0 };

   // Get the HTML
   get_html(domain, port, buff);

   int i = 0;
   for (i = 0; buff[i] != 0; i++)
      printf("%c", buff[i]);

   // Check for links.
   find_links(buff);
   check_link("www.openbsd.org", "443", "/60.html");

   return 0;
}
