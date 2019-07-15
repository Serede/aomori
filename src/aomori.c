#include <aomori.h>
#include <inttypes.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define _NET_FMT_ "%" SCNu8 ".%" SCNu8 ".%" SCNu8 ".%" SCNu8 "/%" SCNu8

struct aomori_node_s {
    bool active;
    uint8_t label[AOMORI_LABEL_MAX_LEN];
    struct aomori_node_s *children[2];
};

typedef struct net_s {
    bool bits[32];
    int mask;
} net;

net *_parse_net_(char *network) {
    net *n;
    uint8_t byt[4];
    uint8_t mask;
    int i, j, k;

    i = sscanf(network, _NET_FMT_, &byt[0], &byt[1], &byt[2], &byt[3], &mask);

    if (i == 4) {
        mask = 32;
    } else if (i != 5) {
        return NULL;
    }

    n = (net *)malloc(sizeof(net));
    if (n == NULL) {
        return NULL;
    }

    for (i = 0; i < 4; i++) {
        for (j = 0; j < 8; j++) {
            n->bits[8 * i + j] = (byt[i] >> j) & 0x01;
        }
    }

    n->mask = mask;

    return n;
}

void _aomori_print_(aomori_node *ao, unsigned level) {
    unsigned i, j;
    for (i = 0; i < 2; i++) {
        if (ao->children[i] != NULL) {
            for (j = 0; j < level; j++) {
                printf("  ");
            }
            printf("- [%01u] %01u: %s\n", level, i, ao->children[i]->label);
            _aomori_print_(ao->children[i], level + 1);
        }
    }
}

aomori_node *aomori_create() {
    aomori_node *ao;

    ao = (aomori_node *)malloc(sizeof(aomori_node));
    if (ao == NULL) {
        return NULL;
    }

    ao->active = false;
    ao->children[0] = NULL;
    ao->children[1] = NULL;

    return ao;
}

void aomori_destroy(aomori_node *ao) {
    if (ao == NULL) {
        return;
    }
    free(ao);
}

void aomori_print(aomori_node *ao) {
    if (ao == NULL) {
        return;
    }
    _aomori_print_(ao, 0);
}

int aomori_put(aomori_node *ao, char *network, char *label) {
    aomori_node **node;
    net *n;
    int i;

    n = _parse_net_(network);
    if (n == NULL) {
        return -1;
    }

    node = &ao;
    for (i = 0; i < n->mask - 1; i++) {
        node = &(*node)->children[n->bits[i]];
        if ((*node) == NULL) {
            (*node) = aomori_create();
            if ((*node) == NULL) {
                return -1;
            }
        }
    }
    free(n);

    if (strcpy((*node)->label, label) == NULL) {
        return -1;
    };
    (*node)->active = true;

    return 0;
}

char *aomori_get(aomori_node *ao, char *network) {
    char *label;
    aomori_node *node;
    net *n;
    int i;

    n = _parse_net_(network);
    if (n == NULL) {
        return NULL;
    }

    label = NULL;
    node = ao;
    for (i = 0; (i < n->mask) && (node != NULL); i++) {
        if (node->active) {
            label = node->label;
        }
        node = node->children[n->bits[i]];
    }
    free(n);

    return label;
}