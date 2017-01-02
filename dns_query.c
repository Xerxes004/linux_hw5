#include "dns_query.h"

int get_cname(union ip_address ip, char *c_name)
{
	printk(KERN_INFO "get_cname: Translating ip: %d.%d.%d.%d", ip.a[0], ip.a[1], ip.a[2], ip.a[3]);
	// Connect to DNS
	// Send DNS query
	// Read DNS response
	
	return 0;
}