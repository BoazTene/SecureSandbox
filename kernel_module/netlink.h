#ifndef NETLINK_H_MOEW
#define NETLINK_H_MOEW

#include <net/genetlink.h>
#include "node.h"
#include "parser.h"
#include "firewall.h"


#define FAMILY_NAME "FIREWALL_LOGER"

struct logging_data {
	int protocol;
	int len;
	char data[4096];
};

/**
 * Defines the fnetlink family possible commands.
 */
enum FAMILY_COMMANDS {
	BLOCKED_PACKET,
	LIST_RULES,
	ADD_RULE,
	REMOVE_RULE,
	__MAX_COMMAND
};

#define MAX_COMMAND (__MAX_COMMAND - 1)

/*
 * Defines the netlink family possible attributes
 */
enum FAMILY_ATTRIBUTES {
	PACKET_BUFFER,
	RULES,
	RULE,
	RULE_ID,
	DUMMY2,
	DUMMY3,
	DUMMY4,
	DUMMY5,
	DUMMY6,
	DUMMY7,
	RULE_NAME,
	RULE_INGOING_PACKETS,
	RULE_OUTGOING_PACKETS,
	RULE_BLACK,
	RULE_PROTOCOL,
	RULE_SRC_IP,
	RULE_DST_IP,
	RULE_LOG,
	RULE_SRC_PORT,
	RULE_DST_PORT,
	__MAX_ATTRIBUTE
};

#define MAX_ATTRIBUTE (__MAX_ATTRIBUTE - 1)


enum FAMILY_MULTICAST_GROUPS {
    NONE,
    LOGGING,
    __MAX_MULTICAST_GROUP,
};

#define MAX_MULTICAST_GROUP (__MAX_MULTICAST_GROUP - 1)

#define VERSION 1


extern struct genl_ops family_gnl_ops[];

extern struct genl_family family;

extern struct genl_multicast_group mc[1];

/**
 * Registers the netlink family to the generic netlink router.
 *
 * @return return code, zero on success.
 */
int register_netlink(void);

/**
 * Unregisters the netlink family from the generic netlink router.
 *
 * @return the return code zero on success.
 */
int unregister_netlink(void);

/**
 * Sends a multicast message over the LOGGING multicast group.
 *
 * The message containes the BLOCKED_PACKET operation and has a attribute of a PACKET_BUFFER (blocked_packet).
 *
 * @param logging_packet packet to log.
 * @return the return code.
 */
int send_multicast_logging(struct sk_buff *logging_packet);

#endif
