// An addition to the 'Hello World' netfilter hooks example based a three-part 
// blog by Paul Kiddie
//   www.paulkiddie.com/2009/10/creating-a-simple-hello-world-netfilter-module/
// and modified by K Shomper for linux 4.4 assignment in CS3320.  Dec 1, 2016.
// 
// Author  : Wesley Kelly
// Modified: 2 January, 2017
//
// This module addition is used to resolve an IP address to a given Canonical
// Name via a DNS query and response.


/* Translates an IP address union into a Canonical Name via a DNS query.
 *
 * @param an ip_address union
 * @param c_name the buffer in which to store the canonical name, if any
 * @return 0 if the lookup failed, otherwise the length of the c_name string
 *				 not including the null char at the end
 */
int get_cname(const ip_address ip, char *c_name);