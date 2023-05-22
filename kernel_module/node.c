#include "node.h"

/**
 * Gets the head of a linked list (linear/circular),
 * and frees the entire linked list.
 *
 * @param head the head of the list
 */
void free_list(struct rule_node *list)
{
	if (list == NULL) 
		return;

	struct rule_node *pos_entry, *temp_entry;

	list_for_each_entry_safe(pos_entry, temp_entry, &list->list, list) {
	    list_del(&pos_entry->list);
	    kfree(pos_entry);
	}	
}

/**
 * Countes the number of elements in the list.
 *
 * @param list the head of the list.
 * @return the number of elements in the list.
 */
int count_list(struct rule_node *list)
{
	if (list == NULL)
		return 0; 

	int rules_number = 0;
	struct rule_node *pos_entry, *temp_entry;

	list_for_each_entry_safe(pos_entry, temp_entry, &list->list, list) {
		rules_number++;
	}
	printk("rules_number (func): %d\n", rules_number);

	return rules_number;
}

/**
 * Serializes a list of a rule_nodes.
 * New allocated seriazable is returned (need to be freed).
 *
 * @param list the list to serialize.
 * @return the serialized list.
 */
char * serialize_list(struct rule_node *list) 
{
	struct rule_node *pos_entry, *temp_entry;
	int rules_number = count_list(list);
	char *current_ptr;
	char *serialized_list;

	if (list == NULL) 
		return NULL;

	serialized_list = (char *) kmalloc(sizeof(struct rule) * rules_number, GFP_KERNEL);
	if (serialized_list == NULL) {
		return NULL;
	}

	current_ptr = serialized_list;

	if (!list_empty(&list->list)) {
		list_for_each_entry_safe(pos_entry, temp_entry, &list->list, list) {
			printk("meow 1\n");
			printk("Remaining memory: %d\n", sizeof(struct rule) * rules_number);
			printk("rules memory: %d\n", sizeof(struct rule));

			memcpy(current_ptr, &pos_entry->rule, sizeof(struct rule));

			current_ptr += sizeof(struct rule);
		}
	}

	return serialized_list;
}

/**
 * Adds a new rule to the list (id will be generated automatically).
 *
 * @param head the head of the list
 * @param rule the rule to add
 * @return 0 on success and -1 on failure
 */
int add_rule(struct rule_node *head, struct rule rule)
{
	struct rule_node *new_rule = kmalloc(sizeof(struct rule_node), GFP_KERNEL);

	if (!new_rule) {
		return -1;
	}

	new_rule->rule = rule;
	if (list_empty(&head->list)) {
		new_rule->id = 0;
	} else {
		struct rule_node *rule = list_last_entry(&head->list, struct rule_node, list);

		new_rule->id = rule->id+1;
	}
	list_add_tail(&new_rule->list, &head->list);

	return 0;
}

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
int remove_rule(struct rule_node *head, int id)
{
	if (head == NULL)
		return -2;

	struct rule_node *pos_entry, *temp_entry;
	int ret = -1;
	bool id_found = 0;

	list_for_each_entry_safe(pos_entry, temp_entry, &head->list, list) {
		if (pos_entry->id == id && !id_found) {
			list_del(&pos_entry->list);
			ret = 0;
			id_found = 1;
		}

		if (id_found && pos_entry->id != id) {
			pos_entry->id = pos_entry->id -1;
		}
	}

	return ret;
}
