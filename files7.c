#include <fcntl.h>
#include <stdio.h>
#include <unistd.h>

#define BUFFER_SIZE 1024

int main() {
        char file1[100], file2[100];
	ssize_t n1, n2;

        printf("Enter the first file path:\n");
        scanf("%s", file1);

        printf("enter the second file path:\n");
        scanf("%s", file2);

        int fd_1;
        fd_1 = open(file1, O_RDONLY);

        int fd_2;
        fd_2 = open(file2, O_RDONLY);

        if (fd_1 == -1 || fd_2 == -1) {
                perror("Error while opening the file");
                return 1;
        }

	unsigned char buf1[BUFFER_SIZE], buf2[BUFFER_SIZE];
	off_t byte_index = 0;

        while (1) {
        	n1 = read(fd_1, buf1, BUFFER_SIZE);
        	if (n1 < 0) { 
			perror("read first file"); 
			close(fd_1); 
			close(fd_2); 
			return 1; 
		}

        	n2 = read(fd_2, buf2, BUFFER_SIZE);
        	if (n2 < 0) { 
			perror("read second file"); 
			close(fd_1); 
			close(fd_2); 
			return 1; 
		}

        	if (n1 == 0 && n2 == 0) break;

        	ssize_t min_read = (n1 < n2) ? n1 : n2;
        	for (ssize_t i = 0; i < min_read; i++) {
            		if (buf1[i] != buf2[i]) {
                		printf("Files differ at byte %ld\n", (long)(byte_index + i));
                		close(fd_1);
                		close(fd_2);
                		return 1;
            		}
        	}

        	if (n1 != n2) {
            		printf("Files differ at byte %ld\n", (long)(byte_index + min_read));
            		close(fd_1);
            		close(fd_2);
            		return 1;
        	}

        	byte_index += min_read;
    	}

	printf("Files are identical\n");
        close(fd_1);
        close(fd_2);
        return 0;
}
