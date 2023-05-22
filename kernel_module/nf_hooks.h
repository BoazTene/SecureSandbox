#ifndef NF_HOOKS_H
#define NF_HOOKS_H

#include <linux/netfilter.h>
#include <linux/netfilter_ipv4.h>
#include <linux/ip.h>
#include <linux/tcp.h>
#include <linux/udp.h>
#include <linux/in.h>
#include <linux/inet.h>

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
unsigned int outgoing_nf_hook(void *priv,
			      struct sk_buff *skb, const struct nf_hook_state *state);

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
unsigned int ingoing_nf_hook(void *priv,
			     struct sk_buff *skb, const struct nf_hook_state *state);

#endif
