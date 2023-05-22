#include <stdio.h>
#include <stdlib.h>
#include "docopt.h"
#include "sysfs_utils.h"

int main(int argc, char *argv[]) {
    struct DocoptArgs args = docopt(argc, argv, 1, NULL);

    if (args.add) {
        if (args.src == NULL ||  args.dst == NULL || args.proto == NULL || args.name == NULL) {
            printf("You are missing arguments.\n");
            exit(0);
        }
	
        add_rule(args.name, args.out, args.in, args.black, atoi(args.proto), args.src, args.dst, args.log);
    } else if (args.remove) {
	remove_rule(atoi(args.id));
    } else if (args.list) {
        list_rules();
    } else if (args.dump) {
        printf("Dumping \n");
    } else if (args.load) {
        printf("Loading \n");
    }

    return 0;
}

