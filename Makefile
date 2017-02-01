all: get_html scan_links get_links check_link
	gcc scan_links.o get_html.o get_links.o check_link.o -Wall -g -lssl -lcrypto -o scan_links

scan_links: scan_links.c get_html.h
	gcc scan_links.c -c -Wall -g

get_html: get_html.c get_html.h
	gcc get_html.c -c -Wall -g

get_links: get_links.c get_links.h
	gcc get_links.c -c -Wall -g

check_link: check_link.c
	gcc check_link.c -c -Wall -g

run:
	./scan_links www.openbsd.org 443

debug:
	gdb scan_links www.example.com 80

clean:
	rm scan_links scan_links.o get_html.o get_links.o check_link.o
