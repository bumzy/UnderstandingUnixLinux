#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

#define BUF_SIZE 20

char buf[BUF_SIZE+1];

void oops(char *, char *);

int main(int argc, char const *argv[])
{
	int fd1, fd2, fd3, len = BUF_SIZE;
	char *file_name;
	memset(buf, 0, sizeof(buf));

	if (argc != 2){
        fprintf( stderr, "usage: %s source destination\n", *argv);
        exit(1);
    }

    file_name = argv[1];

	if((fd1 = open(file_name, O_RDONLY)) == -1){
		oops("Cannot open ", file_name);
	}
	if((fd2 = open(file_name, O_WRONLY)) == -1){
		oops("Cannot open ", file_name);
	}
	if((fd3 = open(file_name, O_RDONLY)) == -1){
		oops("Cannot open ", file_name);
	}
	if(read(fd1, buf, BUF_SIZE) == -1){
		oops("Read error from ", file_name);
	}
	printf("%s\n", buf);
	strcpy(buf, "test 123...");
	len = strlen(buf);
	if(write(fd2, buf, len) != len){
		oops("Write error to ", file_name);
	}
	if(read(fd3, buf, BUF_SIZE) == -1){
		oops("Read error from ", file_name);
	}
	printf("%s\n", buf);

    if ( close(fd1) == -1 || close(fd2) == -1 || close(fd3) == -1)
        oops("Error closing files","");
	return 0;
}

void oops(char *s1, char *s2)
{
    fprintf(stderr,"Error: %s ", s1);
    perror(s2);
    exit(1);
}
