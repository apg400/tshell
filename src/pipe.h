//
#ifndef _PIPE_H
#define _PIPE_H

#include "operator.h"

#include <iostream>
#include <unistd.h>
#include <stdio.h>

class Pipe : public Operator {
public:
    Pipe(Action* left, Action* right) : Operator(left, right) { }

    virtual bool execute(int readfile, int writefile) {
        int fd[2];

        if (pipe(fd) == -1) {
            perror("could not intiate pipe");
        }
        
        if (left == nullptr || right == nullptr) {
            perror("missing argument");
            return false;
        }

        if (!left->execute(readfile, fd[1])) return false;
        close(fd[1]);
        if (!right->execute(fd[0], writefile)) return false;
        close(fd[0]);

        return true;
    }
};

#endif
