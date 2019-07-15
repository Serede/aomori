#include <aomori.h>
#include <string.h>

int main(int argc, char const *argv[]) {
    aomori_node *ao;
    char *label;

    ao = aomori_create();
    if (ao == NULL) {
        return -1;
    }

    if (aomori_put(ao, "192.168.0.0/16", "Home") != 0) {
        return -1;
    }

    if (aomori_put(ao, "192.168.1.0/24", "Hall") != 0) {
        return -1;
    }

    if (aomori_put(ao, "192.168.2.0/24", "John's Room") != 0) {
        return -1;
    }

    if (aomori_put(ao, "192.168.3.0/24", "Lisa's Room") != 0) {
        return -1;
    }

    if (aomori_put(ao, "192.168.2.1", "John's Laptop") != 0) {
        return -1;
    }

    if (aomori_put(ao, "192.168.3.1", "Lisa's Laptop") != 0) {
        return -1;
    }

    if (aomori_put(ao, "192.168.3.2", "Lisa's Phone") != 0) {
        return -1;
    }

    if (strcmp(aomori_get(ao, "192.168.2.1"), "John's Laptop") != 0) {
        return -1;
    }

    if (strcmp(aomori_get(ao, "192.168.3.2"), "Lisa's Phone") != 0) {
        return -1;
    }

    if (strcmp(aomori_get(ao, "192.168.1.1"), "Hall") != 0) {
        return -1;
    }

    if (strcmp(aomori_get(ao, "192.168.4.0/24"), "Home") != 0) {
        return -1;
    }

    if (aomori_get(ao, "192.169.1.1") != NULL) {
        return -1;
    }

    if (aomori_get(ao, "192.0.0.0/8") != NULL) {
        return -1;
    }

    aomori_destroy(ao);

    return 0;
}
