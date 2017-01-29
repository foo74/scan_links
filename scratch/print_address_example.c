/* THIS CODE BELOW IS MORE FOR TESTING AND PRINTING OUT
 * WHAT IS GOING ON WITH THE LOOKUPS.
 */

// an addrinfo struct named p to loop through.
struct addrinfo *p; 

printf("IP addresses for %s:\n\n", address);

for(p = res;p != NULL; p = p->ai_next) 
{
void *addr;
char *ipver;

// get the pointer to the address itself,
// different fields in IPv4 and IPv6:
if (p->ai_family == AF_INET) { // IPv4
 struct sockaddr_in *ipv4 = (struct sockaddr_in *)p->ai_addr;
 addr = &(ipv4->sin_addr);
 ipver = "IPv4";
} else { // IPv6
 struct sockaddr_in6 *ipv6 = (struct sockaddr_in6 *)p->ai_addr;
 addr = &(ipv6->sin6_addr);
 ipver = "IPv6";
}

// convert the IP to a string and print it:
inet_ntop(p->ai_family, addr, ipstr, sizeof ipstr);
printf("  %s: %s\n", ipver, ipstr);
}
