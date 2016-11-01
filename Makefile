all: scan_links

scan_links: scan_links.c
	gcc scan_links.c -Wall -g -o scan_links
run:
	./scan_links 129.128.5.194 80
clean:
	rm scan_links
