/*
 * Fork a child process which will use pipes to receive a string inputted to the parent, 
 * then reverse the letter case of each character in the string. One pipe used for sending
 * OG message, another used to send modified message.
 *
 */


#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<sys/types.h>
#include<string.h>
#include<sys/wait.h>
#include<ctype.h>
int main() {
	//creating 2 pipes
	int p1[2]; // pipe 1
	int p2[2]; // pipe 2
	int i;
	char buf[80];
	int pid;
	if (pipe(p1)< 0) {
		fprintf(stderr, "Error creating Pipe 1" );
		return 1;
	}
	if (pipe(p2)<0) {
		fprintf(stderr, "Error creating Pipe 2" );
		return 1;
	}
	printf("Enter a string : ");
	fgets(buf,sizeof(buf),stdin);
	buf[strlen(buf)-1]='\0';
	pid = fork();
	if (pid < 0) {
		fprintf(stderr, "Fork Error" );
		return 1;
	}
	else if (pid > 0) { //parent
		close(p1[0]); // close reading end pipe1
		write(p1[1], buf, strlen(buf)+1); //write to pipe1 then close
		close(p1[1]);
		wait(NULL);
		close(p2[1]);
		read(p2[0], buf, sizeof(buf)); //reading from pipe2 after child finishes
		printf("Parent received string %s from child\n", buf);
		close(p2[0]);
	}
	else { //child
		close(p1[1]); // close write end pipe1
		read(p1[0], buf, sizeof(buf)); //read parent's input from pipe1
		close(p1[0]); //close both read ends 
		close(p2[0]);
		printf("Child received %s from parent\n",buf);
		for(i = 0; i < strlen(buf); i++) {
			if(islower(buf[i])) { 
				buf[i]=toupper(buf[i]);
			}
			else if(isupper(buf[i])) { 
				buf[i]=tolower(buf[i]);
			}
		}
		// writing converted string to pipe 2
		write(p2[1], buf, strlen(buf)+1);
		close(p2[1]);
		exit(0);
	}
}
