#include "sysfs_utils.h"

/**
 * Lists the active rules on the system.
 */
void list_rules() 
{
	FILE *file;
	char buffer[1024];
	memset(buffer, 0x00, 1024);
	int len;

	file = fopen(SYSFS_LIST_RULES, "r");

	if (file == NULL) {
        printf("Make sure that the module is loaded and that you have the write permissions to access it.\n");
		return;
	}

	len = fread(buffer, 1, sizeof(buffer), file);

	if (len < 0) {
		return;
	}


	printf("Rules Listed: \n%s", buffer);

	fclose(file);
}

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
void add_rule(char *name, int outgoing_packets, int ingoing_packets, int black_list, int protocol, char *src_ip, char *dst_ip, int log)
{
	FILE *file;
	char buffer[256];
	int len;

	file = fopen(SYSFS_ADD_RULE, "w");

	if (file == NULL) {
        printf("Make sure that the module is loaded and that you have the write permissions to access it.\n");
		return;
	}
	
	len = snprintf(buffer, sizeof(buffer), "name: %s\noutgoing_packets:%d\ningoing_packets:%d\nblack_list:%d\nprotocol:%d\nsrc_ip:%s\ndst_ip:%s\nlog:%d\n", name, outgoing_packets, ingoing_packets, black_list, protocol, src_ip, dst_ip, log);

	if (fwrite(buffer, 1, len, file) != len) {
		return;
	}

	fclose(file);
}

/**
 * Removes a firewall rule.
 *
 * @param id rule's id
 */
void remove_rule(int id) 
{
	FILE *file;
	char buffer[256];
	int len;

	file = fopen(SYSFS_REMOVE_RULE, "w");

	if (file == NULL) {
        printf("Make sure that the module is loaded and that you have the write permissions to access it.\n");
		return;
	}
	
	len = snprintf(buffer, sizeof(buffer), "%d", id);

	if (fwrite(buffer, 1, len, file) != len) {
		return;
	}

	fclose(file);
}


