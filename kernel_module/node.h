#ifndef NODE_H
#define NODE_H

#include <linux/list.h>
#include <linux/string.h>
#include <linux/slab.h>
#include "firewall_rule.h"


/**
 * A linked list of firewall rules.
 */
struct rule_node {
	int id;
	struct rule rule;
	struct list_head list;
};


char * serialize_list(struct rule_node *list); 

int count_list(struct rule_node *list);




/**
 * Gets the head of a linked list (linear/circular),
 * and frees the entire linked list.
 *
 * @param head the head of the list
 */
void free_list(struct rule_node *head); 

/**
 * Adds a new rule to the list (id will be generated automatically).
 *
 * @param head the head of the list
 * @param rule the rule to add
 * @return 0 on success and -1 on failure
 */
int add_rule(struct rule_node *head, struct rule rule);

/**
 * Removes a rule based on id.
 * Note that it will update other element's id,
 * in order to keep order.
 * For example:
 * [1,2,3] 
 * 2 gets deleted
 * [1,2] // three was replaced with 2 and renamed 3
 *
 * @param head the head of the list
 * @param id the id of the element to remove
 * @return 0 on success otherwise negative numbers.
 */
int remove_rule(struct rule_node *head, int id);

#endif
