#include <fcntl.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>

int main() {
        const char *filename = "numbers.txt";
        const char *numbers = "1\n2\n3\n4\n5\n6\n7\n8\n9\n10\n";
	const char *rep = "100\n";
	int fd;
	ssize_t nr;
	char c;
    	off_t offset = 0;
    	int newlines = 0;
	char rem[1024];

	fd = open(filename, O_WRONLY | O_CREAT | O_TRUNC, 0644);
	if (fd == -1) {
                perror("Error while opening or creating the file");
                return 1;
        }
	
	if ((nr = write(fd, numbers, strlen(numbers))) == -1){
		perror("write error");
		close(fd);
		return 1;
	}

	close(fd);

	fd = open(filename, O_RDWR);
        if (fd == -1) {
                perror("Error while opening the file");
                return 1;
        }

	while (read(fd, &c, 1) == 1) {
        	if (newlines == 3) break;
        	if (c == '\n') newlines++;
        	offset++;
    	}

	off_t size = lseek(fd, offset, SEEK_SET);
	if (size == -1){
	       	perror ("lseek error");
		close(fd);
		return 1;
	}

	while(read(fd, &c, 1) == 1)
		if (c == '\n') break;

	ssize_t rem_size = read(fd, rem, sizeof(rem));
	if (rem_size == -1){
                perror ("read error");
                close(fd);
                return 1;
        }

	size = lseek(fd, offset, SEEK_SET);
        if (size == -1){
                perror ("lseek error");
                close(fd);
                return 1;
        }

	if ((nr = write(fd, rep, strlen(rep))) == -1){
                perror("write error");
                close(fd);
                return 1;
        }

	if ((nr = write(fd, rem, rem_size)) == -1){
                perror("write error");
                close(fd);
                return 1;
        }

	close(fd);

	fd = open(filename, O_RDONLY);
    	if (fd == -1) { 
		perror("open error"); 
		return 1; 
	}

    	printf("Final file content:\n");
    	while (read(fd, &c, 1) == 1) {
        	putchar(c);
    	}

        close(fd);

        return 0;
}
