#ifndef PAGES_H
#define PAGES_H

#define MAX_BUF 1024

struct page
{
   char name[MAX_BUF];
   struct page *next;
   struct page *next;
};

#endif
