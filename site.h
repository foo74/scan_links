#ifndef SITE_H
#define SITE_H

#define MAX_BUF 1024

struct site
{
   struct page *pages;
   struct link *links;
};

struct page
{
   char page_name[MAX_BUF] = {0};
   struct page *head;
   struct page *tail;
   struct page *next;
   struct page *prev;
};

struct link
{
   char url[MAX_BUF] = {0};
   struct link *head;
   struct link *tail;
   struct link *next;
   struct link *prev;
};

#endif
