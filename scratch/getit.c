#include <stdio.h>
#include <stdlib.h>

int main(int argc, char **argv, char** envp)
{
   char** env;

   puts("Status: 200 OK\r");
   puts("Content-Type: text/html\r");
   puts("\r");

   puts("<!DOCTYPE html><html><body><h1>FUCK YOU</h1> <p>You fucking, fuck.</p>");
   puts("<form action=\"whoami\" method=\"get\">IP:<input type=\"text\" name=\"foo\"><input type=\"submit\" value=\"Go\"></form>");

   for (env = envp; *env != 0; env++)
   {
      char* thisEnv = *env;
      printf("<p>%s</p>", thisEnv);    
   }

   puts("</body></html>");

   return(EXIT_SUCCESS);
}
