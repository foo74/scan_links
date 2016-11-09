all: scan_links get_https

scan_links: scan_links.c
	gcc scan_links.c -Wall -g -o scan_links

get_https: get_https.c
	gcc get_https.c -Wall -g -lssl -lcrypto -o get_https 

run:
	./get_https #./scan_links www.openbsd.org 80

clean:
	rm scan_links get_https
