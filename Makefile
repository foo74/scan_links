all: scan_links

scan_links: scan_links.c
	gcc scan_links.c -Wall -g -lssl -lcrypto -o scan_links

run:
	#./scan_links www.openbsd.org 443
	./scan_links www.example.com 80

debug:
	gdb scan_links www.example.com 80

clean:
	rm scan_links
