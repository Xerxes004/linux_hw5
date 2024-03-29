//'Hello World' netfilter hooks example based a three-part blog by Paul Kiddie
//   www.paulkiddie.com/2009/10/creating-a-simple-hello-world-netfilter-module/
// and modified by K Shomper for linux 4.4 assignment in CS3320.  Dec 1, 2016.
// 
// Authors : Wesley Kelly & Jimmy Von Eiff
// Modified: 5 December, 2016

#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/netfilter.h>
#include <linux/ip.h>
#include <linux/skbuff.h>
#include <linux/string.h>
#include <linux/netfilter_ipv4.h>
#include "netfilter_main.h"
#include "dns_query.h"

#define NF_IP_LOCAL_IN    1
#define NF_IP_LOCAL_OUT   3

// Define constants that are broken when linking
#ifndef NF_IP_PRE_ROUTING
#define NF_IP_PRE_ROUTING 0
#endif

#ifndef NF_IP_POST_ROUTING
#define NF_IP_POST_ROUTING 3
#endif

#define PROTOCOL_ICMP     1

struct iphdr *ip_header;

// Moved to dns_query.h by Wes Kelly
/* Convinience union to convert __be32 to individual octets */
//   union ip_address {
//       u8     a[4];
//       __be32 addr;
//   };

//function to be called by nfho hook operations
unsigned int hook_funco(void *priv, struct sk_buff *skb, 
							  const struct nf_hook_state *state) {

	//grab network header using accessor
	ip_header = (struct iphdr *)skb_network_header(skb);
       
	if (ip_header->protocol == PROTOCOL_ICMP) {
   		//log to dmesg queue indicating an outbound ICMP packet was discovered 
    	printk(KERN_INFO "netfilter: Received an outbound ICMP packet.");
	}

   //allows the packet to proceed
   return NF_ACCEPT;
}

//function to be called by nfhi hook operations
unsigned int hook_funci(void *priv, struct sk_buff *skb, 
							  const struct nf_hook_state *state) {

	//grab network header using accessor
	ip_header = (struct iphdr *)skb_network_header(skb); 
       
 	//grab the incoming ip address
	char buf[20];
	union ip_address ip;
  	ip.addr = ip_header->saddr;

	// convert the __be32 address to a typical dotted-notation IP address
	snprintf(buf, 20, "%d.%d.%d.%d", ip.a[0], ip.a[1], ip.a[2], ip.a[3]);

  get_cname(ip, NULL);

	// If the packet is ICMP
  	if (ip_header->protocol==PROTOCOL_ICMP) {
  		
  		// if the ICMP packet is from telehack
		if (strncmp(buf, "64.13.139.230", 20) == 0) {
			//log to dmesg queue 
   			printk(KERN_WARNING "incoming ICMP packet allowed from telehack.com\n");
   		}
		else {
	   		//log to dmesg queue 
	   		printk(KERN_WARNING "incoming ICMP packet allowed from %s", buf);
		}
	}
	// else allow the packet without mentioning it in the dmesg queue

   //allows the packet to proceed
   return NF_ACCEPT;
}

//struct holding set of hook function options for outbound packets
static struct nf_hook_ops nfho = {
  .hook     = hook_funco,           //function to call when conditions below met
  .hooknum  = NF_IP_LOCAL_OUT,      //call before packet sent
  .pf       = PF_INET,              //IPV4 packets
  .priority = NF_IP_PRI_FIRST       //set highest priority over other hook funcs
};

//struct holding set of hook function options for inbound packets
static struct nf_hook_ops nfhi = {
  .hook     = hook_funci,           //function to call when conditions below met
  .hooknum  = NF_IP_LOCAL_IN,       //call before packet recieved
  .pf       = PF_INET,              //IPV4 packets
  .priority = NF_IP_PRI_FIRST       //set highest priority over other hook funcs
};

//Called when module loaded using 'insmod'
int init_module() {
   printk(KERN_WARNING "registering net filter\n");

   // Hook up output filter
   nfho.hook     = hook_funco;
   nfho.pf       = PF_INET;
   nfho.hooknum  = NF_IP_POST_ROUTING;
   nfho.priority = NF_IP_PRI_FIRST;

   // Hook up input filter
   nfhi.hook     = hook_funci;
   nfhi.pf       = PF_INET;
   nfhi.hooknum  = NF_IP_PRE_ROUTING;
   nfhi.priority = NF_IP_PRI_FIRST;

   nf_register_hook(&nfho);
   nf_register_hook(&nfhi);

   printk(KERN_WARNING "registered net filter\n");

   //return 0 for success
   return 0;
}

//Called when module unloaded using 'rmmod'
void cleanup_module() {
   printk(KERN_WARNING "unregistering net filter\n");
   
   nf_unregister_hook(&nfho);
   nf_unregister_hook(&nfhi);

   printk(KERN_WARNING "unregistered net filter\n");
}

MODULE_AUTHOR("K. Shomper based on work by P. Kiddie");
MODULE_LICENSE("GPL");
