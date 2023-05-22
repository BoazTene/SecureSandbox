#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define SYSFS_ADD_RULE "/sys/kernel/firewall/fw_rules/add_rule"
#define SYSFS_REMOVE_RULE "/sys/kernel/firewall/fw_rules/remove_rule"
#define SYSFS_LIST_RULES "/sys/kernel/firewall/fw_rules/list_rules"

/**
 * Lists the active rules on the system.
 */
void list_rules();

/**
 * Adds a new firewall rule.
 *
 * @param outgoing_packets does the rule should affect outgoing_packets
 * @param ingoing_packets  does the rule should affect ingoing_packets
 * @param black_list if true a packet matching this rule will be dropped otherwise any other packet will be dropped
 * @param protocol the protocol to use
 * @param src_ip the src ip use * for all
 * @param dst_ip the dst ip use * for all
 * @param log does the rule should log blocked packets
 */
void add_rule(char *name, int outgoing_packets, int ingoing_packets, int black_list, int protocol, char *src_ip, char *dst_ip, int log);

/**
 * Removes a firewall rule.
 *
 * @param id rule's id
 */
void remove_rule(int id);

