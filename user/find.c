#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
#include "kernel/fs.h"

int check(char *path, char *name) {
    int index = 0;
    for (int i = strlen(path) - 1; i >= 0 ; --i) {
        if (path[i] == '/') {
            index = i + 1;
            break;
        }
    }
    return strcmp(path + index, name);
}

void find(char *path, char *name) {
    char buf[512], *p;
    int fd;
    struct stat st;
    struct dirent de;

    fd = open(path, 0);

    if (fd < 0) {
        fprintf(2, "find: cannot open %s\n", path);
        exit(1);
    }

    if(fstat(fd, &st) < 0){
        fprintf(2, "find: cannot stat %s\n", path);
        close(fd);
        exit(1);
    }

    switch(st.type) {
        case T_DEVICE:
        case T_FILE:
            if (check(path, name) == 0) {
                printf("%s\n", path);
            }
            break;
        
        case T_DIR:
            if(strlen(path) + 1 + DIRSIZ + 1 > sizeof buf){
                printf("find: path too long\n");
                break;
            }
            strcpy(buf, path);
            p = buf+strlen(buf);
            *p++ = '/';
            while(read(fd, &de, sizeof(de)) == sizeof(de)){
                if(de.inum == 0)
                    continue;
                if (strcmp(de.name, ".") == 0 || strcmp(de.name, "..") == 0) {
                    continue;
                }
                memmove(p, de.name, DIRSIZ);
                p[DIRSIZ] = 0;
                find(buf, name);
            }
            break;
    }
    close(fd);
}

int main(int argc, char *argv[]) {
    if (argc != 3) {
        printf("find: miss path or name");
        exit(1);
    }
    find(argv[1], argv[2]);
    exit(0);
}
