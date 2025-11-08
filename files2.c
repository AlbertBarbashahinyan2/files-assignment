#include <fcntl.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>

int main() {
	const char *buf = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";
	int fd, nr, t_ret;
        off_t l_ret;
	char rem[11];

	fd = open("data.txt", O_WRONLY | O_CREAT | O_TRUNC, 0644);

        if (fd == -1) {
                perror("Error while opening or creating the file");
                return 1;
        }
	
	nr = write(fd, buf, strlen(buf));
	if (nr == -1) {
		perror("Error while writing to the file");
		close(fd);
		return 1;
	}
        close(fd);

	fd = open("data.txt", O_RDWR);
	if (fd == -1) {
                perror("Error while opening the file");
                return 1;
        }

	if((l_ret = lseek(fd, 0, SEEK_END)) == -1){
		perror("Lseek error");
		close(fd);
		return 1;
	}

	printf("Current size: %ld\n", l_ret);

	t_ret = truncate("data.txt", 10);
    	
	if (t_ret == -1){
        	perror("truncate error");
		close(fd);
        	return -1;
    	}

	if((l_ret = lseek(fd, 0, SEEK_END)) == -1){
                perror("Lseek error");
                close(fd);
                return 1;
        }

	printf("Truncated size: %ld\n", l_ret);

	if((l_ret = lseek(fd, 0, SEEK_SET)) == -1){
                perror("Lseek error");
                close(fd);
                return 1;
        }
	
	if((nr = read(fd, rem, sizeof(rem))) == -1){
		perror("Error while reading the file");
		close(fd);
		return 1;
	}
	rem[nr] = '\0';
	printf("%s\n", rem);
	close(fd);

        return 0;
}
