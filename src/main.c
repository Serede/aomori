#include <aomori.h>
#include <argp.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <version.h>

#define BUFFER_LEN 1024

const char *argp_program_version = PROJECT_NAME " " PROJECT_VERSION;

static char doc[] = PROJECT_NAME;
static char args_doc[] = "DB_FILE IN_FILE";

typedef struct arguments_s {
    char *args[2]; /* DB_FILE and IN_FILE */
    char *outfile; /* Argument for -o */
    bool verbose;  /* The -v flag */
} arguments;

static struct argp_option options[] = {
    {"output", 'o', "FILE", 0, "Output to FILE instead of to standard output"},
    {"verbose", 'v', 0, 0, "Produce verbose output"},
    {0}};

static error_t parse_opt(int key, char *arg, struct argp_state *state) {
    arguments *args = state->input;

    switch (key) {
    case 'v':
        args->verbose = 1;
        break;
    case 'o':
        args->outfile = arg;
        break;
    case ARGP_KEY_ARG:
        if (state->arg_num >= 2) {
            argp_usage(state);
        }
        args->args[state->arg_num] = arg;
        break;
    case ARGP_KEY_END:
        if (state->arg_num < 2) {
            argp_usage(state);
        }
        break;
    default:
        return ARGP_ERR_UNKNOWN;
    }

    return 0;
}

static struct argp argp = {options, parse_opt, args_doc, doc};

int main(int argc, char **argv) {
    arguments args;
    FILE *db, *in, *out;
    aomori_node *ao;
    char buffer[BUFFER_LEN];
    char *label, *network;
    size_t len;

    /* Set argument defaults */
    args.outfile = NULL;
    args.verbose = 0;

    /* Where the magic happens */
    argp_parse(&argp, argc, argv, 0, 0, &args);

    ao = aomori_create();

    db = fopen(args.args[0], "r");
    while (!feof(db)) {
        if (fgets(buffer, BUFFER_LEN - 1, db) == NULL) {
            aomori_destroy(ao);
            return -1;
        }
        if (strlen(buffer) > 0) {
            label = strtok(buffer, ",");
            network = strtok(NULL, ",");
            aomori_put(ao, network, label);
        }
    }
    fclose(db);

    // if (args.outfile)
    //     out = fopen(args.outfile, "w");
    // else
    //     out = stdout;

    if (args.verbose) {
        aomori_print(ao);
    }

    aomori_destroy(ao);

    return 0;
}
