/*
 * Scan for broken links. Supports HTTP (port 80) and HTTPS (port 443).
 */

#include "get_links.h"

/* Define constants */
#define MAXBUFFER 2097152  // use a power of 2 for performance reasons
//const char *port80 = "80";
//const char *port443 = "443";

// Find links in the HTTP.
int find_links(char buff[])
{
   char trimmed_buff[MAXBUFFER] = { 0 };

   remove_white_space(buff, trimmed_buff);

printf("trimmed_buff=%s\n\n", trimmed_buff);
//printf("buff=%s\n\n", buff);
   char *pos = trimmed_buff;
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
