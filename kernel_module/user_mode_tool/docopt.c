#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "docopt.h"

struct Command {
    const char *name;
    bool value;
};

struct Argument {
    const char *name;
    const char *value;
    const char *array[ARG_MAX];
};

struct Option {
    const char *oshort;
    const char *olong;
    bool argcount;
    bool value;
    const char *argument;
};

struct Elements {
    int n_commands;
    int n_arguments;
    int n_options;
    struct Command *commands;
    struct Argument *arguments;
    struct Option *options;
};


/*
 * Tokens object
 */

struct Tokens {
    int argc;
    char **argv;
    int i;
    char *current;
};

const char usage_pattern[] =
        "Usage:\n"
        "  firewall add --name=<n> --out --in --black --proto=<p> --src=<s> --dst=<d> [--log]\n"
        "  firewall remove <id>\n"
        "  firewall list\n"
        "  firewall dump <file>\n"
        "  firewall load <file>\n"
        "  firewall --help";

struct Tokens tokens_new(int argc, char **argv) {
    struct Tokens ts;
    ts.argc = argc;
    ts.argv = argv;
    ts.i = 0;
    ts.current = argv[0];
    return ts;
}

struct Tokens *tokens_move(struct Tokens *ts) {
    if (ts->i < ts->argc) {
        ts->current = ts->argv[++ts->i];
    }
    if (ts->i == ts->argc) {
        ts->current = NULL;
    }
    return ts;
}


/*
 * ARGV parsing functions
 */

int parse_doubledash(struct Tokens *ts, struct Elements *elements) {
    /*
    int n_commands = elements->n_commands;
    int n_arguments = elements->n_arguments;
    Command *commands = elements->commands;
    Argument *arguments = elements->arguments;

    not implemented yet
    return parsed + [Argument(None, v) for v in tokens]
    */
    return 0;
}

int parse_long(struct Tokens *ts, struct Elements *elements) {
    int i;
    int len_prefix;
    int n_options = elements->n_options;
    char *eq = strchr(ts->current, '=');
    struct Option *option;
    struct Option *options = elements->options;

    len_prefix = (eq - (ts->current)) / sizeof(char);
    for (i = 0; i < n_options; i++) {
        option = &options[i];
        if (!strncmp(ts->current, option->olong, len_prefix))
            break;
    }
    if (i == n_options) {
        /* TODO: %s is not a unique prefix */
        fprintf(stderr, "%s is not recognized\n", ts->current);
        return 1;
    }
    tokens_move(ts);
    if (option->argcount) {
        if (eq == NULL) {
            if (ts->current == NULL) {
                fprintf(stderr, "%s requires argument\n", option->olong);
                return 1;
            }
            option->argument = ts->current;
            tokens_move(ts);
        } else {
            option->argument = eq + 1;
        }
    } else {
        if (eq != NULL) {
            fprintf(stderr, "%s must not have an argument\n", option->olong);
            return 1;
        }
        option->value = true;
    }
    return 0;
}

int parse_shorts(struct Tokens *ts, struct Elements *elements) {
    char *raw;
    int i;
    int ts_num = ts->i;
    int n_options = elements->n_options;
    struct Option *option;
    struct Option *options = elements->options;

    raw = &ts->current[1];
    tokens_move(ts);
    while (raw[0] != '\0') {
        for (i = 0; i < n_options; i++) {
            option = &options[i];
            if (option->oshort != NULL && option->oshort[1] == raw[0])
                break;
        }
        if (i == n_options) {
            /* TODO -%s is specified ambiguously %d times */
            fprintf(stderr, "-%c is not recognized\n", raw[0]);
            return EXIT_FAILURE;
        }
        raw++;
        if (!option->argcount) {
            option->value = true;
        } else {
            if (raw[0] == '\0') {
                if (ts->current == NULL) {
                    fprintf(stderr, "%s requires argument\n", option->oshort);
                    return EXIT_FAILURE;
                }
                raw = ts->current;
                tokens_move(ts);
            }
            option->argument = raw;
            break;
        }
    }
    return EXIT_SUCCESS;
}

int parse_argcmd(struct Tokens *ts, struct Elements *elements) {
    int i;
    int ts_num = ts->i;
    int n_commands = elements->n_commands;
    int n_arguments = elements->n_arguments; 
    struct Command *command;
    struct Command *commands = elements->commands;
    struct Argument *argument;
    struct Argument *arguments = elements->arguments;

    for (i = 0; i < n_commands; i++) {
        command = &commands[i];
        if (strcmp(command->name, ts->current) == 0) {
            command->value = true;
            tokens_move(ts);
            return EXIT_SUCCESS;
        }
    }
    /* not implemented yet, just skip for now
       parsed.append(Argument(None, tokens.move())) */
    if (ts_num > 1) {
	char * command = ts->argv[1];
	
	if (strcmp(command, "remove") == 0 && ts_num == 2) {
		argument = &arguments[1];
		argument->value = ts->current;
		argument->name = "<id>";
		tokens_move(ts);
		return EXIT_SUCCESS;
		
	}
    }

    tokens_move(ts);
    return EXIT_SUCCESS;
}

int parse_args(struct Tokens *ts, struct Elements *elements) {
    int ret = EXIT_FAILURE;

    while (ts->current != NULL) {
        if (strcmp(ts->current, "--") == 0) {
            ret = parse_doubledash(ts, elements);
            if (ret == EXIT_FAILURE) break;
        } else if (ts->current[0] == '-' && ts->current[1] == '-') {
            ret = parse_long(ts, elements);
        } else if (ts->current[0] == '-' && ts->current[1] != '\0') {
            ret = parse_shorts(ts, elements);
        } else
            ret = parse_argcmd(ts, elements);
        if (ret) return ret;
    }
    return ret;
}

int elems_to_args(struct Elements *elements, struct DocoptArgs *args,
                     const bool help, const char *version) {
    struct Command *command;
    struct Argument *argument;
    struct Option *option;
    int i, j;

    /* fix gcc-related compiler warnings (unused) */
    (void) command;
    (void) argument;

    /* options */
    for (i = 0; i < elements->n_options; i++) {
        option = &elements->options[i];
        if (help && option->value && strcmp(option->olong, "--help") == 0) {
            for (j = 0; j < 19; j++)
                puts(args->help_message[j]);
            return EXIT_FAILURE;
        } else if (version && option->value &&
                   strcmp(option->olong, "--version") == 0) {
            puts(version);
            return EXIT_FAILURE;
        } else if (strcmp(option->olong, "--black") == 0) {
            args->black = option->value;
        } else if (strcmp(option->olong, "--help") == 0) {
            args->help = option->value;
        } else if (strcmp(option->olong, "--in") == 0) {
            args->in = option->value;
        } else if (strcmp(option->olong, "--log") == 0) {
            args->log = option->value;
        } else if (strcmp(option->olong, "--out") == 0) {
            args->out = option->value;
        } else if (strcmp(option->olong, "--dst") == 0) {
            if (option->argument) {
                args->dst = (char *) option->argument;
            }
        } else if (strcmp(option->olong, "--name") == 0) {
            if (option->argument) {
                args->name = (char *) option->argument;
            }
        } else if (strcmp(option->olong, "--proto") == 0) {
            if (option->argument) {
                args->proto = (char *) option->argument;
            }
        } else if (strcmp(option->olong, "--src") == 0) {
            if (option->argument) {
                args->src = (char *) option->argument;
            }
        }
    }
    /* commands */
    for (i = 0; i < elements->n_commands; i++) {
        command = &elements->commands[i];
        
        if (strcmp(command->name, "add") == 0) {
    args->add = command->value;
}
 else if (strcmp(command->name, "dump") == 0) {
    args->dump = command->value;
}
 else if (strcmp(command->name, "list") == 0) {
    args->list = command->value;
}
 else if (strcmp(command->name, "load") == 0) {
    args->load = command->value;
}
 else if (strcmp(command->name, "remove") == 0) {
    args->remove = command->value;
}

    }
    /* arguments */
    for (i = 0; i < elements->n_arguments; i++) {
        argument = &elements->arguments[i];

	if (strcmp(argument->name, "<file>") == 0) {
            args->file = (char *) argument->value;
        }
         else if (strcmp(argument->name, "<id>") == 0) {
            args->id = (char *) argument->value;
        }
    }
    return EXIT_SUCCESS;
}


/*
 * Main docopt function
 */

struct DocoptArgs docopt(int argc, char *argv[], const bool help, const char *version) {
    struct DocoptArgs args = {
        0, 0, 0, 0, 0, NULL, NULL, 0, 0, 0, 0, 0, NULL, NULL, NULL, NULL,
            usage_pattern,
            { "Usage:",
              "  firewall add --name=<n> --out --in --black --proto=<p> --src=<s> --dst=<d> [--log]",
              "  firewall remove <id>",
              "  firewall list",
              "  firewall dump <file>",
              "  firewall load <file>",
              "  firewall --help",
              "",
              "Options:",
              "  -h --help     Show this screen.",
              "  --name=<n>    Rule's name.",
              "  --out         Allow outgoing packets.",
              "  --in          Allow incoming packets.",
              "  --black       Block matching packets.",
              "  --proto=<p>   Protocol number (e.g. 6 for TCP).",
              "  --src=<s>     Source IP address or network.",
              "  --dst=<d>     Destination IP address or network.",
              "  --log         Log matching packets.",
              ""}
    };
    struct Command commands[] = {
        {"add", 0},
        {"dump", 0},
        {"list", 0},
        {"load", 0},
        {"remove", 0}
    };
    struct Argument arguments[] = {
        {"<file>", NULL, NULL},
        {"<id>", NULL, NULL}
    };
    struct Option options[] = {
        {NULL, "--black", 0, 0, NULL},
        {"-h", "--help", 0, 0, NULL},
        {NULL, "--in", 0, 0, NULL},
        {NULL, "--log", 0, 0, NULL},
        {NULL, "--out", 0, 0, NULL},
        {NULL, "--dst", 1, 0, NULL},
        {NULL, "--name", 1, 0, NULL},
        {NULL, "--proto", 1, 0, NULL},
        {NULL, "--src", 1, 0, NULL}
    };
    struct Elements elements;
    int return_code = EXIT_SUCCESS;

    elements.n_commands = 5;
    elements.n_arguments = 2;
    elements.n_options = 9;
    elements.commands = commands;
    elements.arguments = arguments;
    elements.options = options;

    if (argc == 1) {
        argv[argc++] = "--help";
        argv[argc++] = NULL;
        return_code = EXIT_FAILURE;
    }

    {
        struct Tokens ts = tokens_new(argc, argv);
        if (parse_args(&ts, &elements))
            exit(EXIT_FAILURE);
    }
    if (elems_to_args(&elements, &args, help, version))
        exit(return_code);
    return args;
}
