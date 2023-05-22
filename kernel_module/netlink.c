#include "netlink.h"
#include <linux/ip.h>


char * get_raw_packet_data(struct sk_buff * skb, int len);

int list_rules_dumpit(struct sk_buff *skb, struct netlink_callback *cb);
int add_rule_doit(struct sk_buff *skb, struct genl_info *info);
int remove_rule_doit(struct sk_buff *skb, struct genl_info *info);

struct nla_policy list_rules_policy[] = {
	[RULES] = { .type = RULES, .len=8642 },
};

struct nla_policy add_rule_policy[] = {
	[RULE]  = { .type = NLA_NESTED},
};


struct nla_policy remove_rule_policy[] = {
	[RULE_ID]  = { .type = NLA_NESTED },
};


struct genl_ops family_gnl_ops[] = { 
	{
		.cmd = LIST_RULES,
		.policy = list_rules_policy,
		.dumpit = list_rules_dumpit,
		.doit = NULL,
		.flags = GENL_ADMIN_PERM,
	},
	{
		.cmd = ADD_RULE,
		.policy = add_rule_policy,
		.dumpit = NULL,
		.doit = add_rule_doit,
		.flags = GENL_ADMIN_PERM,
	},
 	{
		.cmd = REMOVE_RULE,
		.policy = remove_rule_policy,
		.dumpit = NULL,
		.doit = remove_rule_doit,
		.flags = GENL_ADMIN_PERM,
		.validate = NLA_VALIDATE_NONE,
	}
};


struct genl_multicast_group mc[1] ={{
	.name = "logger",
	.flags = 0,
} }; 


struct genl_family family = {
    .id = 0, // let the generic netlink choose an id.
    .hdrsize = 0, // no special header.
    .name = FAMILY_NAME,
    .version = VERSION,
    .maxattr = MAX_ATTRIBUTE,
    .n_ops = 3,
    .ops = family_gnl_ops,
    .mcgrps = mc,
    .mcgrp_offset = 1,
    .n_mcgrps = 1,
};

int add_rule_doit(struct sk_buff *skb, struct genl_info *info)
{
	if(info->attrs[RULE]) {
		struct rule rule = parse_nested_rule(info->attrs[RULE]);
		add_rule(rules, rule);
	}

	return 0;
}

int remove_rule_doit(struct sk_buff *skb, struct genl_info *info)
{
	if(info->attrs[RULE_ID]) {
		int rule_id = parse_rule_id(info->attrs[RULE_ID]);

		printk("Removing %d\n", rule_id);
		remove_rule(rules, rule_id);
	}

	return 0;
}

/**
 * The list_rules dumpit operation.
 * Dumps a serializeable of the current firewall rules.
 */
int list_rules_dumpit(struct sk_buff *skb, struct netlink_callback *cb)
{
	int err = 0;
	void *msg_head;
	int size;

	char * serialized_rules = serialize_list(rules);
	size = sizeof(struct rule) * count_list(rules);	

	if (size == 0) size = 1;
	
	msg_head = genlmsg_put(skb, cb->nlh->nlmsg_pid, cb->nlh->nlmsg_seq, &family, NLM_F_DUMP, LIST_RULES);
	
	if (count_list(rules) != 0) {
		err = nla_put(skb, RULES, size, serialized_rules);
	}
	if (err < 0) {
		printk("Failed putting string\n");
	}

	genlmsg_end(skb, msg_head);

	return 0;
}

/**
 * Registers the netlink family to the generic netlink router.
 *
 * @return return code, zero on success.
 */
int register_netlink(void)
{
    return genl_register_family(&family);
}

/**
 * Unregisters the netlink family from the generic netlink router.
 *
 * @return the return code zero on success.
 */
int unregister_netlink(void)
{
    return genl_unregister_family(&family);
}

int buff_to_logging(struct sk_buff *packet , struct logging_data* data)
{
	struct iphdr *iph = ip_hdr(packet);
	int len = sizeof(data->data);
	if (packet->len < len) len = packet->len;

	data->protocol = iph->protocol;
	data->len = packet->len;
	char * raw_packet_data = get_raw_packet_data(packet, len);
	
	memset(data->data, 0x00, sizeof(data->data));
	memcpy(data->data, raw_packet_data, len);

	kfree(raw_packet_data);

	return len + 2*sizeof(int);
}

char* get_raw_packet_data(struct sk_buff* skb, int len) {
    char* packet_data;

    // Allocate memory for the packet data
    packet_data = kmalloc(len, GFP_KERNEL);
    if (!packet_data) {
        return NULL;
    }

    // Copy the packet data into the buffer
    skb_copy_bits(skb, 0, packet_data, len);

    return packet_data;
}


/**
 * Sends a multicast message over the LOGGING multicast group.
 *
 * The message containes the BLOCKED_PACKET operation and has a attribute of a PACKET_BUFFER (blocked_packet).
 *
 * @param logging_packet packet to log.
 * @return the return code.
 */
int send_multicast_logging(struct sk_buff *logging_packet)
{
    struct logging_data *data = kmalloc(sizeof(struct logging_data), GFP_KERNEL);
    if (data == NULL) return -1;
    int mcgrp_id;
    struct sk_buff *skb;
    void *msg_head;
    int len;
    
    skb = genlmsg_new(NLMSG_GOODSIZE, GFP_KERNEL);
    if (skb == NULL) {
        return -1;
    }

    int id = 9; 

    msg_head = genlmsg_put(skb, 0, 0, &family, 0, BLOCKED_PACKET);

    len = buff_to_logging(logging_packet, data);

    nla_put(skb, PACKET_BUFFER, sizeof(struct logging_data) - (4096 - len), (void *) data);

    genlmsg_end(skb, msg_head);

    genlmsg_multicast_allns(&family, skb, id, 0, NETLINK_CB(skb).portid);
    kfree(data);


    return 0;
}
