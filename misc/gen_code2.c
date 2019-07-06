#include <stdio.h>
#include <stdint.h>
#include <string.h>

char *strs[] = {
    "HALT",
    "RET",
    0
};

char buf[8];
int main() {
    for (char** i = strs; *i; i++) {
        memset(buf, 0, 8);
        for (char j = 0, *k = buf, *l = *i; j < 8 && *l; j++,
                                                         *(k++) = *(l++));
        printf("case 0x%lx: //%s\n"
               "    *out = %s;\n"
               "    return 1;\n",
               *(uint64_t*)buf, buf, buf);
    }
}
