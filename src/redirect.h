//
#ifndef REDIRECT_H
#define REDIRECT_H

#include <fcntl.h>
#include <sys/types.h>

class Redirect : public Action {
protected:
    Action* command;
    char* file;
public:
    Redirect(Action* command, char* file) {
        this->command = command;
        this->file = file;
    }

    virtual bool execute(int readfile, int writefile) = 0;
};

////////

class RedirectInput : public Redirect {
public:
    RedirectInput(Action* command, char* file) : Redirect(command, file) { }

    virtual bool execute(int readfile, int writefile) { 
	if (command == nullptr || file == nullptr) return false;

        if (this->file != nullptr) {
            if ((readfile = open(this->file, O_RDONLY)) == -1) {
                perror("could not open file");
                return false;
            }
        }

        return this->command->execute(readfile, writefile);
    }

};

////////

class RedirectOutput : public Redirect {
public:
    RedirectOutput(Action* command, char* file) : Redirect(command, file) { }

    virtual bool execute(int readfile, int writefile) {
        if (command == nullptr && file == nullptr) return false;

        if (this->file != nullptr) {
            if ((writefile = open(this->file, O_WRONLY | O_TRUNC | O_CREAT, S_IRWXU)) == -1) {
                perror("could not open file");
                return false;
            }
        }

        return this->command->execute(readfile, writefile);
    }

};

////////

class AppendOutput : public Redirect {
public:
    AppendOutput(Action* command, char* file) : Redirect(command, file) { }

    virtual bool execute(int readfile, int writefile) {
        if (command == nullptr && file == nullptr) return false;        

        if (this->file != nullptr) {
            if ((writefile = open(this->file, O_WRONLY | O_APPEND | O_CREAT, S_IRWXU)) == -1) {
                perror("could not open file");
                return false;
            }
        }
         
        return this->command->execute(readfile, writefile);
    }

};


#endif
