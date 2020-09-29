/*
 * file copy program: should be run from command line in the format as follows: ./hw3-4.exe input.txt copy.txt
 * program creates a pipe, writes content of the input file to it, then a forked child will read from pipe and 
 * write to copy file.
 *
 */


#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <fcntl.h>
int main(int argc, char *argv[])
{
   int pipe1[2],fileSize;
   char memorySize[100],forkMemory[100];
   char* srcName = argv[1];
   char* destName = argv[2];
   if (argc != 3) { //argc must be 3 -> needs 2 filenames inputted
       perror("Filecopy: filecopy.exe [target_file] [destination_file]. \n");
       exit(1);
    }
   
    if (pipe(pipe1) < 0) {
       printf("Error creating pipe %s\n", strerror(errno));
       exit(1);
    }
   
    switch (fork()) {
 
    case -1:
       printf("Fork Error %s\n", strerror(errno));
       exit(1);
    case 0: //child
        close(pipe1[1]);
        ssize_t childBytes = read(pipe1[0], forkMemory, sizeof(forkMemory));   
        close(pipe1[0]);
        int dest_descrptor = open(destName, O_CREAT | O_WRONLY);
        write(dest_descrptor, forkMemory, childBytes);
    default:
        close(pipe1[0]);   
        int srcDesc = open(srcName, O_RDONLY);
        ssize_t num_bytes = read(srcDesc, memorySize, sizeof(memorySize));
        write(pipe1[1], memorySize, num_bytes);
        close(pipe1[1]);   
    }
    return 0;
}
