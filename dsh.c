/*
 * dsh.c
 *
 *  Created on: Aug 2, 2013
 *      Author: chiu
 */
#include "dsh.h"

#include <dirent.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <wait.h>
#include <sys/types.h>
#include <errno.h>
#include <err.h>
#include <sys/stat.h>
#include <string.h>

// TODO: Your function definitions (declarations in dsh.h)

//Used to check if a given path points to a executable file
int isFile(char* path){

    //Works as a boolean for if the indicated path is a valid path
    int isFile = 1;

    if (access(path, F_OK | X_OK) != 0) {
        isFile = 0;
    }

    return isFile;
};