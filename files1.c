#include <fcntl.h>
#include <stdio.h>
#include <unistd.h>

int main() {
        char source_file[100], dest_file[100];

        printf("Enter the source file path:\n");
        scanf("%s", source_file);

        printf("enter the destination path:\n");
        scanf("%s", dest_file);

        int fd_1;
        fd_1 = open(source_file, O_RDONLY);

        int fd_2;
        fd_2 = open(dest_file, O_WRONLY | O_CREAT | O_TRUNC, 0644);

        if (fd_1 == -1 || fd_2 == -1) {
                perror("Error while opening or creating the file");
                return 1;
        }

        ssize_t ret;
        char buf[1024];
        int bytes_read = 0;

        while ((ret = read(fd_1, buf, sizeof(buf))) > 0) {
                write(fd_2, buf, ret);
                bytes_read += ret;
        }

        if (ret == -1) {
                perror("error while reading file");
                close(fd_1);
                close(fd_2);
                return 1;
        }

        printf("Total bytes: %d", bytes_read);
        printf("\n");

        close(fd_1);
        close(fd_2);
        return 0;
}
