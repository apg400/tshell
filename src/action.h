// Action class
// base class/interface for command prompt components

#ifndef ACTION_H
#define ACTION_H

class Action {
public:
    virtual bool execute(int readfile, int writefile) = 0;
};

#endif
