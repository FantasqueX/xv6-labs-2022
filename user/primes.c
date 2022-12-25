#include "kernel/types.h"
#include "user/user.h"

void f(int left_pipe[]) {
    int p, num;
    int r = read(left_pipe[0], &p, 4);
    if (r == 0) {
        close(left_pipe[0]);
        exit(0);
    }

    fprintf(1, "prime %d\n", p);

    int right_pipe[2];
    pipe(right_pipe);


    if (fork() == 0) {
        close(right_pipe[1]);
        f(right_pipe);
    } else {
        close(right_pipe[0]);
        while (read(left_pipe[0], &num, 4)) {
            if (num % p != 0) {
                write(right_pipe[1], &num, 4);
            }
        }
        close(left_pipe[0]);
        close(right_pipe[1]);
    }
}

int main(void) {
    int left_pipe[2];
    int r;

    r = pipe(left_pipe);
    if (r == -1) {
        printf("Pipe failed!\n");
        exit(1);
    }
    if (fork() == 0) {
        close(left_pipe[1]);
        f(left_pipe);
    } else {
        close(left_pipe[0]);
        for (int i = 2; i <= 35; ++i) {
            write(left_pipe[1], &i, 4);
        }
        close(left_pipe[1]);
    }
    wait(0);
    exit(0);
}
