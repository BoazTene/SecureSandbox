#ifndef PARSER_H
#define PARSER_H

#include <net/genetlink.h>
#include "netlink.h"
#include "node.h"

/**
 * Parses a firewall rule from a nested netlink attribute.
 *
 * @param nested_rule the nested attribute containing the rule.
 * @return rule the firewall rule.
 */
struct rule parse_nested_rule(struct nlattr* nested_rule);

/**
 * Parses a rule id from a nested netlink attribute.
 *
 * @param nested_rule_id a nested netlink attribute containing a rule_id.
 * @return the rule id or -1 on error.
 */
int parse_rule_id(struct nlattr* nested_rule_id);

#endif
