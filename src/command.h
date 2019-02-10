// Command header file
#ifndef COMMAND_H
#define COMMAND_H

#include "action.h"

#include <vector>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>

#include <iostream>

class Command : public Action {
protected:
    std::vector<char*> args;
    bool empty;
public:
    Command(std::vector<char*> command) {
        if (command.empty()) {
	    empty = true;
	} else {
            args = command;
            //args.push_back(NULL);
            empty = false;
        }
    }

    virtual bool execute(int readfile, int writefile) {
        if (empty) {
            //perror("command not found");
            return false;
        }
        
        args.push_back(NULL);

        // if command is exit then it terminates shell
        if (strcmp(args[0], "exit") == 0) {
            exit(0);
        }

        int STD_IN;
        int STD_OUT;
        if ((STD_IN = dup(0)) == -1) perror("unable to duplicate stream");
        if ((STD_OUT = dup(1)) == -1) perror("unable to duplacte stream");

        if (dup2(readfile, 0) == -1) perror("unable to duplicate stream");
        if (dup2(writefile, 1) == -1) perror("unable to duplicate stream");

        int status;  
        pid_t childID;
	
        childID = fork(); // creates new child process

        // executes when fork fails
        if (childID == -1) { 
            perror("error creating process");
            exit(1);
        }

        // executes when child process has succesfully started
        if (childID == 0) {
            if (execvp(args[0], args.data()) == -1) {
                perror("command not found");
                exit(1);
            } else { 
                exit(0); 
            }
        }
		
        // waits for child process tto finish before continuing
        if (waitpid(childID, &status, 0) == -1) {
            perror("waitpid error");
        }

        if (dup2(STD_IN, 0) == -1) perror("error reseting stream");
        if (dup2(STD_OUT, 1) == -1) perror("error reseting stream");
		
        // checks what exit status the child process ended with
        if (WEXITSTATUS(status) == 0) 
            return true;
        return false;
    }
};

#endif
