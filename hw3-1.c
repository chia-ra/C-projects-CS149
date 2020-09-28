/*
 *	in terminal: gcc hello.c -o hello.exe THEN ./hello.exe
 *  Created on: Sep 27, 2020
 *      Author: chiar
 */

#include <stdio.h>
int main()
{
	int x;
	x = fork();
	if (x > 0) {
		printf("In parent process\nMy PID: %d\n", getpid());
	}

	else if (x == 0) {
		sleep(10); //keep zombie in system for 10 secs
		x = fork();
	}

	if (x > 0) {
		printf("In child process\nMy PID :%d\nMy PPID : %d\n", getpid(), getppid());

		while(1)
			sleep(1);

		printf("In child process\nMy PPID: %d\n", getppid());
	}

	else if (x == 0)
		printf("In child's child process\n My PPID : %d\n", getppid());

	return 0;
}



