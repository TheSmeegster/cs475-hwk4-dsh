/*
 * main.c
 *
 *  Created on: Mar 17 2017
 *      Author: david
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/stat.h>
#include <string.h>
#include "dsh.h"

int main(int argc, char **argv)
{
	char cmdline[MAXBUF]; // stores user input from commmand line
	int childRunning = 0; //Used to halt main process if a child is running and parent is supposed to wait

	//Runs as long as allowed, and "exit" has not been input
	while(strcmp(cmdline, "exit") && !childRunning){

		//Prompts user for input
		printf("dsh> ");
		gets(cmdline);

		//Built-in print working directory
		if(!strcmp(cmdline, "pwd")){

			char cwdPath[256]; //Maximum directory path size

			//Accessed and prints the current working directory
			if(getcwd(cwdPath, sizeof(cwdPath)) != NULL){
				printf("cwd = %s\n", cwdPath);
			}

		} else if(cmdline[0] == 'c' && cmdline[1] == 'd'){ //Checks if the input contains "cd" and changes directory if this is the case

			//Splits out the "cd" to isolate the new directory path
			char * newDirPath = strtok(cmdline, "cd ");

			//Changes directory to new path
			int newLoc = chdir(newDirPath);

			//Responds with a new directory indication if successful or fail if the path was not found
			if(newLoc){
				printf("New Directory = %s\n", newDirPath);
			} else {
				printf("Incorrect path\n");
			}
			
		} else if(cmdline[0] == '/'){ //Accesses indicated file

			//Splits at every " " (used to locate a final '&' symbol)
			char * info = strtok(cmdline, " ");

			//Checks if the indicated file is a file/executable
			int fileCk = isFile(cmdline);

			//If indicated file is a file and executable, forks
			if(fileCk){

				pid_t process = fork();
				
				if(info[3] == '&'){ //If '&' is present at the end, runs child while waiting for more input on parent
					if(process == 0){					
						execv(&info[0], argv);
					}
				} else { //If the last input is not a '&' symbol, halts parent while running
				
					if(process == 0){
						childRunning = 1;
						execv(&info[0], argv);
					} else {
						wait(NULL);
						childRunning = 0;
					}
				}
			} else { //Incorrect path
				printf("Command not found, incorrect path\n");
			}
		} else if(strcmp(cmdline, "exit")){ //Searches for a file or unknown path if a random name is given

			//First checks if the file can be directly accessed
			if (access(cmdline, F_OK) == 0) {
				execv(cmdline, argv);

			} else { //If file cannot be directly accessed, tests given file path against all possible PATH values
				char *PATH = getenv("PATH");
				int arrSize = strlen(PATH);

				//Splitting return of PATH
				char *splitPath = strtok(PATH, ":");

				//Iterates through the split PATH and tests if the file input is accessable through any paths
				for(int i = 0; i < arrSize; i++){

					splitPath = strtok(NULL, ":");

					//Executes file if it exists and is executable
					if (access(&splitPath[i] + *cmdline, F_OK) == 0) {
						execv(&splitPath[i], argv);
					}
				}	
			}
		}
	}
	return 0;
}
