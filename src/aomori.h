#include <stdint.h>
#include <stdio.h>

#define AOMORI_LABEL_MAX_LEN 128

typedef struct aomori_node_s aomori_node;

int aomori_insert(aomori_node *ao, uint8_t *addr, uint8_t mask, char *label);
