#include <stdint.h>
#include <stdio.h>

#define AOMORI_LABEL_MAX_LEN 128

typedef struct aomori_node_s aomori_node;

aomori_node *aomori_create();
void aomori_destroy(aomori_node *ao);
void aomori_print(aomori_node *ao);
int aomori_put(aomori_node *ao, char *network, char *label);
char *aomori_get(aomori_node *ao, char *network);
