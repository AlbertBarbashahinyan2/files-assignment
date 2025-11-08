#include <fcntl.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>

int main() {
        const char *filename = "log.txt";
        int fd;

	fd = open(filename, O_WRONLY | O_CREAT | O_APPEND, 0644);
	if (fd == -1) {
                perror("Error while opening or creating the file");
                return 1;
        }

        ssize_t ret;
        char buf[1024];
        int bytes_read = 0;
	char c;
	pid_t pid = getpid();

	printf("Enter a line:\n");

        while ((ret = read(0, &c, 1)) > 0) {
                if (bytes_read < sizeof(buf) - 1){
			buf[bytes_read++] = c;
		}

		if (c == '\n') break;
        }

        if (ret == -1) {
                perror("error while reading file");
                close(fd);
                return 1;
        }
	buf[bytes_read] = '\0';

	char line[1200];
    	int len = snprintf(line, sizeof(line), "PID=%d: %s", pid, buf);
        
	ssize_t bytes_written = write(fd, line, len);
        if (bytes_written < 0) {
                perror("write error");
                close(fd);
                return 1;
        }
	off_t size = lseek(fd, 0, SEEK_CUR);
	if (size == -1){
	       	perror ("lseek error");
		close(fd);
		return 1;
	}
        printf("file offset: %ld\n", size);
	
        close(fd);
        return 0;

	// Even with O_APPEND, the system keeps an internal 
	// offset that moves forward after each write, so 
	// lseek with SEEK_CUR shows the growing end position.
}
