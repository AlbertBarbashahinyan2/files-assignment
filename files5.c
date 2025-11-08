#include <fcntl.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>

int main() {
        const char *filename = "sparse.bin";
	const char *start = "START";
	const char *end = "END";
        int fd;
	ssize_t nr;

	fd = open(filename, O_WRONLY | O_CREAT | O_TRUNC, 0644);
	if (fd == -1) {
                perror("Error while opening or creating the file");
                return 1;
        }
	
	if ((nr = write(fd, start, strlen(start))) == -1){
		perror("write error");
		return 1;
	}

	off_t size = lseek(fd, 1024*1024, SEEK_CUR);
	if (size == -1){
	       	perror ("lseek error");
		close(fd);
		return 1;
	}
        
	if ((nr = write(fd, end, strlen(end))) == -1){
                perror("write error");
                return 1;
        }
	
        close(fd);

	fd = open(filename, O_RDONLY);
        if (fd == -1) {
                perror("Error while opening the file");
                return 1;
        }

	size = lseek(fd, 0, SEEK_END);
        if (size == -1){
                perror ("lseek error");
                close(fd);
                return 1;
        }

	printf("Apparent file size: %ld\n", size);

	close(fd);

        return 0;

	// The file’s size appears large because we skipped 
	// ahead and wrote at a far offset, but the empty region 
	// wasn’t actually written to disk.
	// Filesystems that support sparse files don’t allocate 
	// real space for these unwritten gaps, so the apparent 
	// size is big while the actual disk usage stays small.
}
