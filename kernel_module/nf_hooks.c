#include "nf_hooks.h"
#include "firewall_rule.h"
#include "node.h"
#include "firewall.h"
#include "netlink.h"

unsigned int filter(struct sk_buff *skb, struct rule *rule);
unsigned int outgoing_filter(struct sk_buff *skb, struct rule *rule);
unsigned int ingoing_filter(struct sk_buff *skb, struct rule *rule);
bool check_protocol(struct iphdr *iph, struct rule *rule);
bool check_saddr(struct iphdr *iph, struct rule *rule);
bool check_daddr(struct iphdr *iph, struct rule *rule);
bool check_dst_port(struct sk_buff *skb, struct rule *rule);
bool check_src_port(struct sk_buff *skb, struct rule *rule);


/**
 * Handler for outgoing netfilter hook.
 *
 * Drops packets that do not obey the set rules.
 *
 * @param priv private data pointer
 * @param skb socket buffer containing packet data
 * @param state netfilter hook state
 *
 * @return NF_DROP if packet is dropped, NF_ACCEPT if it's accepted
 */
unsigned int ingoing_nf_hook(void *priv,
			     struct sk_buff *skb, const struct nf_hook_state *state)
{
	if (!skb) {
		return NF_ACCEPT;
	}


	unsigned int ret = NF_ACCEPT;

	struct rule_node *pos_entry, *temp_entry;

	list_for_each_entry_safe(pos_entry, temp_entry, &rules->list, list) {
		if (ingoing_filter(skb, &pos_entry->rule) == NF_DROP) { 
			send_multicast_logging(skb);
			ret = NF_DROP;
		}
	}
	
	return ret;
}

/**
 * Handler for ingoing netfilter hook.
 *
 * Drops packets that do not obey the set rules.
 *
 * @param priv private data pointer
 * @param skb socket buffer containing packet data
 * @param state netfilter hook state
 *
 * @return NF_DROP if packet is dropped, NF_ACCEPT if it's accepted
 */
unsigned int outgoing_nf_hook(void *priv,
			      struct sk_buff *skb, const struct nf_hook_state *state)
{
	if (!skb) {
		return NF_ACCEPT;
	}
	
	unsigned int ret = NF_ACCEPT;

	struct rule_node *pos_entry, *temp_entry;

	list_for_each_entry_safe(pos_entry, temp_entry, &rules->list, list) {
		if (outgoing_filter(skb, &pos_entry->rule) == NF_DROP) {
			
	     		send_multicast_logging(skb);
			ret = NF_DROP;
		}
	}

	return ret;
}

/**
 * The outgoing filter hook, checks for rule's violation.
 *
 * @param skb socket buffer containing packet data
 * @param rule firewall rule
 *
 * @return NF_DROP if packet is dropped, NF_ACCEPT if it's accepted
 */
unsigned int outgoing_filter(struct sk_buff *skb, struct rule *rule)
{
	check_dst_port(skb, rule);
	if (!rule->outgoing_packets) {
		return NF_ACCEPT;
	}

	return filter(skb, rule);

}

/**
 * The ingoing filter hook, checks for rule's violation.
 *
 * @param skb socket buffer containing packet data
 * @param rule firewall rule
 *
 * @return NF_DROP if packet is dropped, NF_ACCEPT if it's accepted
 */
unsigned int ingoing_filter(struct sk_buff *skb, struct rule *rule)
{
	if (!rule->ingoing_packets) {
		return NF_ACCEPT;
	}

	return filter(skb, rule);
}

/**
 * The main firwall handler, will actually check if a rule has been violeted.
 *
 * @param skb socket buffer containing packet data
 * @param rule firewall rule
 *
 * @return NF_DROP if packet is dropped, NF_ACCEPT if it's accepted
 */
unsigned int filter(struct sk_buff *skb, struct rule *rule)
{
	struct iphdr *iph = ip_hdr(skb);
	printk("New packet\n");
	printk("protocol: %d\n", check_protocol(iph, rule));
	printk("saddr: %d\n", check_saddr(iph, rule));
	printk("daddr: %d\n", check_daddr(iph, rule));
	printk("check_src_port: %d\n", check_src_port(skb, rule));	
	printk("check_dsr_post: %d\n", check_dst_port(skb, rule));

	if (rule->black_list) {
		if (!check_protocol(iph, rule)
		    && !check_saddr(iph, rule)
		    && !check_daddr(iph, rule)
		    && !check_src_port(skb, rule)
		    && !check_dst_port(skb, rule)) {
			return NF_DROP;
		}
	} else {
		if (!(check_protocol(iph, rule)
		    && check_saddr(iph, rule)
		    && check_daddr(iph, rule)
		    && check_src_port(skb, rule)
		    && check_dst_port(skb, rule))) {
			return NF_DROP;
		}
	}

	return NF_ACCEPT;
}

/**
 * Checks if the ip header is obbeying src address.
 *
 * @return true if the ip header is obbeying (allowed), false if the ip header isn't obbeying (rejected).
 */
bool check_saddr(struct iphdr *iph, struct rule *rule)
{
	
	if (!strcmp(rule->src_ip, "*")) {
		return !rule->black_list;
	}

	char saddr_str[INET_ADDRSTRLEN];
	snprintf(saddr_str, sizeof(saddr_str), "%pI4", &iph->saddr);
	

	if (rule->
	    black_list ? !strcmp(rule->src_ip, saddr_str) : strcmp(rule->src_ip, saddr_str)) {
		return 0;
	}

	return 1;
}

/**
 * Checks if the ip header is obbeying destination address.
 *
 * @return true if the ip header is obbeying (allowed), false if the ip header isn't obbeying (rejected).
 */
bool check_daddr(struct iphdr *iph, struct rule *rule)
{
	if (!strcmp(rule->dst_ip, "*")) {
		return !rule->black_list;
	}

	char daddr_str[INET_ADDRSTRLEN];
	snprintf(daddr_str, sizeof(daddr_str), "%pI4", &iph->daddr);
		
	if (rule->
	    black_list ? !strcmp(rule->dst_ip, daddr_str) : strcmp(rule->dst_ip, daddr_str)) {
		return 0;
	}

	return 1;
}

/**
 * Checks if the ip header is obbeying protocol.
 *
 * @return true if the ip header is obbeying (allowed), false if the ip header isn't obbeying (rejected).
 */
bool check_protocol(struct iphdr *iph, struct rule *rule)
{
	if (rule->protocol == -1) {
		return !rule->black_list;
	}

	return (iph->protocol != rule->protocol) && rule->black_list;
}


bool check_src_port(struct sk_buff *skb, struct rule *rule)
{
	if (rule->src_port == 0) {
		return !rule->black_list;
	}

	struct iphdr *iph = ip_hdr(skb);

	if (iph->protocol == 17) { //udp
		struct udphdr * udphdr = udp_hdr(skb);
		unsigned short port = ntohs(udphdr->source);

		return !rule->black_list ? port == rule->src_port : port != rule->src_port;
	} else if (iph->protocol == 6) { //tcp
		struct tcphdr *tcphdr = tcp_hdr(skb);
		unsigned short port = ntohs(tcphdr->source);

		return !rule->black_list ? port == rule->src_port : port != rule->src_port;
	}

	return true; // unknown protocol, or a protocol without ports as icmp, etc.
}

bool check_dst_port(struct sk_buff *skb, struct rule *rule)
{
	if (rule->dst_port == 0) {
		return !rule->black_list;
	}

	struct iphdr *iph = ip_hdr(skb);

	if (iph->protocol == 17) { //udp
		struct udphdr * udphdr = udp_hdr(skb);
		unsigned short port = ntohs(udphdr->dest);
		printk("Dst_port: %d\n", port);
		printk("Dst2_port: %d\n", rule->dst_port);

		return !rule->black_list ? port == rule->dst_port : port != rule->dst_port;
	} else if (iph->protocol == 6) { //tcp
		struct tcphdr *tcphdr = tcp_hdr(skb);
		unsigned short port = ntohs(tcphdr->dest);

		return !rule->black_list ? port == rule->dst_port : port != rule->dst_port;
	}

	return true; // unknown protocol, or a protocol without ports as icmp, etc.
}
