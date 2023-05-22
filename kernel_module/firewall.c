/*
 * 
 * This Kernel module is a basic firewall implmention.
 * The module will handle packets according to a configuration file. 
 */
#define pr_fmt(fmt) "%s:%s(): " fmt, KBUILD_MODNAME, __func__

#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include "netlink.h"
#include "node.h"
#include "firewall_rule.h"
#include "nf_hooks.h"

#define MODNAME   "firewall"

MODULE_AUTHOR("Boaz Tene");
MODULE_DESCRIPTION("Kernel based firewall");
MODULE_LICENSE("Dual MIT/GPL");
MODULE_VERSION("0.1");

struct rule_node *rules;

static struct nf_hook_ops *nf_outgoing_ops = NULL;
static struct nf_hook_ops *nf_ingoing_ops = NULL;

void configure_nf_outgoing_ops(void)
{
	nf_outgoing_ops->hook = (nf_hookfn *) outgoing_nf_hook;
	nf_outgoing_ops->hooknum = NF_INET_PRE_ROUTING;
	nf_outgoing_ops->pf = PF_INET;
	nf_outgoing_ops->priority = NF_IP_PRI_FIRST;
}

void configure_nf_ingoing_ops(void)
{
	nf_ingoing_ops->hook = (nf_hookfn *) ingoing_nf_hook;
	nf_ingoing_ops->hooknum = NF_INET_POST_ROUTING;
	nf_ingoing_ops->pf = PF_INET;
	nf_ingoing_ops->priority = NF_IP_PRI_FIRST;
}

void configure_nf_ops(void)
{
	configure_nf_outgoing_ops();
	configure_nf_ingoing_ops();
}

static int __init packet_saver_init(void)
{
	printk("inserted\n");
	
	rules = kmalloc(sizeof(struct rule_node), GFP_KERNEL);
	if (!rules) {
		return -1;
	}

	INIT_LIST_HEAD(&rules->list);

	nf_outgoing_ops =
	    (struct nf_hook_ops *)kcalloc(1, sizeof(struct nf_hook_ops), GFP_KERNEL);
	nf_ingoing_ops =
	    (struct nf_hook_ops *)kcalloc(1, sizeof(struct nf_hook_ops), GFP_KERNEL);

	configure_nf_ops();

	nf_register_net_hook(&init_net, nf_outgoing_ops);
	nf_register_net_hook(&init_net, nf_ingoing_ops);


	
    if (register_netlink() != 0) {
        printk("Failed to register netlink family.\n");
        return -1;
    }

    
	return 0;
}

static void __exit packet_saver_exit(void)
{
	if (rules)
		free_list(rules);

	nf_unregister_net_hook(&init_net, nf_outgoing_ops);
	nf_unregister_net_hook(&init_net, nf_ingoing_ops);
	kfree(nf_outgoing_ops);
	kfree(nf_ingoing_ops);

	
    if (unregister_netlink() != 0) {
        printk("Failed to unregister netlink family.\n");
    }

    
	pr_info("removed\n");
}

module_init(packet_saver_init);
module_exit(packet_saver_exit);
