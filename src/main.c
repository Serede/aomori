#include <aomori.h>
#include <argp.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <version.h>

#define BUFFER_LEN 65536 /* Maximum line length for files */
#define TOKENS_MAX 1024  /* Maximum number of columns for files */

/* Argp stuff */

const char *argp_program_version = PROJECT_NAME " " PROJECT_VERSION;

static char doc[] =
    "\n"
    "  DB_FILE                    Database file with label definitions\n"
    "  IN_FILE                    Input file to be labelled\n"
    "  COLUMNS                    Indices of input file columns to label\n"
    "                             (comma-separated, begins at 0)\n";

static char args_doc[] = "DB_FILE IN_FILE COLUMNS";

typedef struct arguments_s {
    char *args[3]; /* DB_FILE IN_FILE COLUMNS */
    char *dfs;     /* Field separator for DB_FILE */
    char *ifs;     /* Field separator for IN_FILE */
    char *ofs;     /* Field separator for OUT_FILE */
    char *outfile; /* Output file */
} arguments;

static struct argp_option options[] = {
    {"db-separator", 'd', "STRING", 0,
     "Field separator for DB_FILE (defaults to comma)"},
    {"in-separator", 'i', "STRING", 0,
     "Field separator for IN_FILE (defaults to comma)"},
    {"out-separator", 'o', "STRING", 0,
     "Field separator for OUT_FILE (defaults to comma)"},
    {"output", 'O', "OUT_FILE", 0, "Output file (defaults to stdout)"},
    {0}};

static error_t parse_opt(int key, char *arg, struct argp_state *state) {
    arguments *args = (arguments *)state->input;

    switch (key) {
        break;
    case 'd':
        args->dfs = arg;
        break;
    case 'i':
        args->ifs = arg;
        break;
    case 'o':
        args->ofs = arg;
        break;
    case 'O':
        args->outfile = arg;
        break;
    case ARGP_KEY_ARG:
        if (state->arg_num >= 3) {
            argp_usage(state);
        }
        args->args[state->arg_num] = arg;
        break;
    case ARGP_KEY_END:
        if (state->arg_num < 3) {
            argp_usage(state);
        }
        break;
    default:
        return ARGP_ERR_UNKNOWN;
    }

    return 0;
}

static struct argp argp = {options, parse_opt, args_doc, doc};

/* Main function */

int main(int argc, char **argv) {
    arguments args;
    FILE *db = NULL, *in = NULL, *out = NULL;
    aomori_node *ao = NULL;
    char buffer[BUFFER_LEN];
    char *tokens[TOKENS_MAX] = {NULL};
    char *ptr = NULL, *endptr = NULL, *label = NULL;
    long cols[TOKENS_MAX];
    int ncols, i, j;

    /* Set argument defaults */
    args.outfile = NULL;
    args.dfs = ",";
    args.ifs = ",";
    args.ofs = ",";

    /* Parse arguments */
    argp_parse(&argp, argc, argv, 0, 0, &args);

    /* Parse indices of columns to label */
    ncols = 0;
    ptr = strtok(args.args[2], ",");
    while (ptr != NULL) {
        cols[ncols++] = strtol(ptr, &endptr, 10);
        if (endptr == ptr) {
            return -1;
        }
        ptr = strtok(NULL, ",");
    }

    /* Create aomori */
    ao = aomori_create();
    if (ao == NULL) {
        return -1;
    }

    /* Populate aomori from DB_FILE */
    db = fopen(args.args[0], "r");
    if (db == NULL) {
        aomori_destroy(ao);
        return -1;
    }
    while (fgets(buffer, BUFFER_LEN - 1, db) != NULL) {
        buffer[strcspn(buffer, "\n")] = 0;
        if (strlen(buffer) > 0) {
            i = 0;
            tokens[0] = strtok(buffer, args.dfs);
            while (tokens[i] != NULL) {
                tokens[++i] = strtok(NULL, args.dfs);
            }
#ifndef NDEBUG
            fprintf(stderr, "[aomori] PUT '%s' '%s'\n", tokens[1], tokens[0]);
#endif
            if (aomori_put(ao, tokens[1], tokens[0]) == -1) {
                fclose(db);
                aomori_destroy(ao);
                return -1;
            };
        }
    }
    fclose(db);

    /* Prepare output file */
    out = args.outfile == NULL ? stdout : fopen(args.outfile, "w");
    if (out == NULL) {
        aomori_destroy(ao);
        return -1;
    }

    /* Process input file */
    in = fopen(args.args[1], "r");
    if (in == NULL) {
        fclose(out);
        aomori_destroy(ao);
        return -1;
    }
    while (fgets(buffer, BUFFER_LEN - 1, in) != NULL) {
        buffer[strcspn(buffer, "\n")] = 0;
        if (strlen(buffer) > 0) {
            i = 0;
            tokens[0] = strtok(buffer, args.ifs);
            while (tokens[i] != NULL) {
                if (i == 0) {
                    fprintf(out, "%s", tokens[i]);
                } else {
                    fprintf(out, "%s%s", args.ofs, tokens[i]);
                }
                tokens[++i] = strtok(NULL, args.ifs);
            }
            for (j = 0; j < ncols; j++) {
#ifndef NDEBUG
                fprintf(stderr, "[aomori] GET '%s' \n", tokens[cols[j]]);
#endif
                label = aomori_get(ao, tokens[cols[j]]);
                fprintf(out, "%s%s", args.ofs, label);
            }
            fprintf(out, "\n", buffer);
        }
    }

    /* Clean up */
    fclose(in);
    fclose(out);
    aomori_destroy(ao);

    return 0;
}
