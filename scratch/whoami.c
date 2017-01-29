#include <stdio.h>
#include <stdlib.h>
#include <unistd.h> // sleep()

int main(int argc, char **argv, char** envp)
{
  char** env;

  puts("Status: 200 OK\r");
  puts("Set-Cookie:fuckingid=fuckyou;\r");
  puts("Content-Type: text/html\r");
  puts("\r");

  puts("<!DOCTYPE html>");
  puts("<html><body>");
  puts("<h1>FUCK YOU</h1> <p>You fucking, fuck.</p>");
  puts("<a href=\"/index.html\">Back</a>");

  for (env = envp; *env != 0; env++)
  {
    puts("<p>");
    char* thisEnv = *env;
    printf("%s", thisEnv);    
    puts("</p>");
  }

  puts("</body></html>");

  return(EXIT_SUCCESS);

//puts("<form action=\"whoami\" method=\"get\">IP:<input type=\"text\" name=\"foo\"><input type=\"submit\" value=\"Go\"></form>");
//sleep(5);
}
