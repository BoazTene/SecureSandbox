#ifndef FIREWALL_H
#define FIREWALL_H

/**
 * This sturct descirbes a firewall rule.
 * Each rule is defined as blocking rule or allowing rule (white/black list).
 */
struct rule {
        char name[50];          // The name of the rule.
	int outgoing_packets;	// boolean that describes if the rules affect on outgoing packets. 
	int ingoing_packets;	// boolean that describes if the rules affect on ingoing packets.
	int black_list;		// boolean that describes if the rule is a black list or a white list.
	int protocol;		// The affected protocol.
	char src_ip[15];	// The src ip to be blocked/allowed, * means all.
	char dst_ip[15];	// The dst ip to be blocked/allowed, * means all.
	int dst_port;           // the src port to be blocked/allowed, -1 means all.
	int src_port;           // the dst port to be blocked/allowed, -1 means all.
	int log;		// boolean that describes if the rule should be blocked. 
};

#endif
