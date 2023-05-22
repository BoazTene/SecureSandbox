#include "parser.h"

/**
 * Parses a firewall rule from a nested netlink attribute.
 *
 * @param nested_rule the nested attribute containing the rule.
 * @return rule the firewall rule.
 */
struct rule parse_nested_rule(struct nlattr* nested_rule)
{
	struct rule rule;
	struct nlattr *pos;
	int rem;

	nla_for_each_nested(pos, nested_rule, rem) {
		int type = pos->nla_type;

		if (type == RULE_NAME && pos->nla_len > 50) {
			memcpy(rule.name, nla_data(pos), 50);
		} else if (type == RULE_DST_IP && pos->nla_len > 15) {
			memcpy(rule.dst_ip, nla_data(pos), 15);
		} else if (type == RULE_SRC_IP && pos->nla_len > 15) {
			memcpy(rule.src_ip, nla_data(pos), 15);
		} else if (type == RULE_LOG) {
			rule.log = nla_get_u8(pos);
		} else if (type == RULE_PROTOCOL) {
			rule.protocol = nla_get_u8(pos);
		} else if (type == RULE_BLACK) {
			rule.black_list = nla_get_u8(pos);
		} else if (type == RULE_INGOING_PACKETS) {
			rule.ingoing_packets = nla_get_u8(pos);
		} else if (type == RULE_OUTGOING_PACKETS) {
			rule.outgoing_packets = nla_get_u8(pos);
		} else if (type == RULE_SRC_PORT) {
			rule.src_port = (int) nla_get_s32(pos);
		} else if (type == RULE_DST_PORT) {
			rule.dst_port = (int) nla_get_s32(pos);
		} 
	}

	return rule;
}

/**
 * Parses a rule id from a nested netlink attribute.
 *
 * @param nested_rule_id a nested netlink attribute containing a rule_id.
 * @return the rule id or -1 on error.
 */
int parse_rule_id(struct nlattr* nested_rule_id)
{
	int rule_id = -1;
	struct nlattr *pos;
	int rem;

	nla_for_each_nested(pos, nested_rule_id, rem) {
		int type = pos->nla_type;

		if (type == NLA_U8) {
			rule_id = nla_get_u8(pos);
		}
	}

	return rule_id;
}

