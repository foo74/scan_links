all: scan_links

scan_links: scan_links.c
	gcc scan_links.c -Wall -g -o scan_links
run:
	./scan_links www.example.com
clean:
	rm scan_links
