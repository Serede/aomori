#include "aomori.h"
#include <inttypes.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct aomori_node_s {
    uint8_t label[AOMORI_LABEL_MAX_LEN];
    struct aomori_node_s *children[2];
};

typedef struct bit_s {
    unsigned x : 1;
} bit;

bit **parse_net(uint8_t *addr, uint8_t mask) {
    bit **net, *current;
    uint8_t octets[4];
    int i, j, k;

    if (sscanf(addr, "%" PRIu8 ".%" PRIu8 ".%" PRIu8 ".%" PRIu8, &octets[0],
               &octets[1], &octets[2], &octets[3]) != 4) {
        return NULL;
    };

    net = (bit **)malloc(32 * sizeof(bit *));
    if (net == NULL) {
        return NULL;
    }

    for (i = 0; (i < 4) && (4 * i < mask); i++) {
        for (j = 0; (j < 8) && (4 * i + j < mask); j++) {
            current = net[4 * i + j];
            current = (bit *)malloc(32 * sizeof(bit));
            if (current = NULL) {
                for (k = 0; k < 4 * i + j; k++) {
                    free(net[4 * i + j]);
                }
                free(net);
                return NULL;
            }
            current->x = (octets[i] >> j) & 0x1;
        }
    }

    if (mask < 32) {
        net[mask] = NULL;
    }

    return net;
}

int aomori_insert(aomori_node *ao, uint8_t *addr, uint8_t mask, char *label) {
    aomori_node *node;
    bit **net, *current;
    int i;

    net = parse_net(addr, mask);
    if (net == NULL) {
        return -1;
    }

    node = ao;
    for (i = 0; (i < 31) && (net[i + 1] != NULL); i++) {
        node = node->children[net[i]->x];
        if (node == NULL) {
            node = (aomori_node *)malloc(sizeof(aomori_node));
        }
    }
    strcpy(node->label, label);
}