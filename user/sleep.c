#include "kernel/types.h"
#include "user/user.h"

int main(int argc, char *argv[]) {
    if (argc <= 1) {
        fprintf(2, "sleep: missing operand\n");
        exit(1);
    }
    int num = atoi(argv[1]);
    if (num == 0) {
        fprintf(2, "sleep: invalid time interval\n");
        exit(1);
    }
    sleep(num);
    exit(0);
}
