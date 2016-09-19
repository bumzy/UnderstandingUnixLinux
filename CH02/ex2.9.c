#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

#define BUF_SIZE 20
#define OFF 20000

char buf[BUF_SIZE+1];

void oops(const char *, const char *);

int main(int argc, char const *argv[])
{
	int fd1, fd2, len;
	memset(buf, 0, sizeof(buf));

	if (argc != 2){
        fprintf( stderr, "usage: %s source destination\n", *argv);
        exit(1);
    }

    const char *file_name = argv[1];

	if((fd1 = open(file_name, O_RDONLY)) == -1){
		oops("Cannot open ", file_name);
	}
	if((fd2 = open(file_name, O_WRONLY)) == -1){
		oops("Cannot open ", file_name);
	}

	if(lseek(fd1, OFF, SEEK_END) != -1){
		if(read(fd1, buf, BUF_SIZE) != -1){
			printf("%s\n", buf);
		}
	}

	strcpy(buf, "hello");
	len = strlen(buf);
	if(lseek(fd2, OFF, SEEK_END) != -1){
		if(write(fd2, buf, len) == len){
			printf("Write sucess\n");
		}
	}

    if ( close(fd1) == -1 || close(fd2) == -1)
        oops("Error closing files","");

	return 0;
}

void oops(const char *s1, const char *s2)
{
    fprintf(stderr,"Error: %s ", s1);
    perror(s2);
    exit(1);
}
