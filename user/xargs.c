#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
#include "kernel/fs.h"
#include "kernel/param.h"

int main(int argc, char *argv[]) {
    if (argc == 1) {
        fprintf(2, "xargs: missing arguments\n");
        exit(1);
    }
    char **buf = malloc(MAXARG * sizeof(char *));

    char *command = argv[1];
    int cnt = 0;
    int l;
    for (int i = 1; i < argc; ++i) {
        buf[cnt] = malloc(1024);
        l = strlen(argv[i]);
        memcpy(buf[cnt], argv[i], l);
        buf[cnt][l] = 0;
        ++cnt;
    }
    char ch;
    int j = 0;
    buf[cnt] = malloc(1024);
    buf[cnt + 1] = 0;
    while (read(0, &ch, 1) == 1) {
        if (ch == '\n') {
            buf[cnt][j] = 0;
            if (fork() == 0) {
                exec(command, buf);
            } else {
                wait(0);
            }
            j = 0;
        } else {
            buf[cnt][j++] = ch;
        }
    }
    for (int i = 0; i <= cnt; ++i) {
        free(buf[i]);
    }
    free(buf);
    return 0;
}
