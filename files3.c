#include <fcntl.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>

int main() {
	char buf;
	int fd, nr, ret;
        off_t l_ret;
	char source_file[100];

        printf("Enter the source file path:\n");
        scanf("%s", source_file);

        if ((fd = open(source_file, O_RDONLY)) == -1) {
                perror("Error while opening or creating the file");
                return 1;
        }
	
	if((l_ret = lseek(fd, 0, SEEK_END)) == -1){
		perror("Lseek error");
		close(fd);
		return 1;
	}

	printf("File size: %ld\n", l_ret);
	
	for (off_t i = l_ret - 1; i >= 0; --i) {
		if((l_ret = lseek(fd, i, SEEK_SET)) == -1){
                        perror("Lseek error");
                        close(fd);
                        return 1;
                }
                if(ret = read(fd, &buf, 1) == -1){
			perror("read error");
                        close(fd);
                        return 1;
		}
		if((nr = write(STDOUT_FILENO, &buf, 1)) == -1){
                	perror("write error");
			close(fd);
			return 1;
		}

        }

	if((nr = write(STDOUT_FILENO, "\n", 1)) == -1){
		perror("write error");
		close(fd);
		return 1;
	}
	
	close(fd);

        return 0;
}
