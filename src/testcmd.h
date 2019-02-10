#ifndef TESTCMD
#define TESTCMD

#include "command.h"
#include <sys/stat.h>
#include <iostream>

class TestCMD : public Command {
private:
    struct stat buff;

public:
    TestCMD(std::vector<char*> args) : Command(args) { }
    // args is { "test", "flag", "file" }
    // null is no longer in args

    virtual bool execute(int readfile, int writefile) {
        if (args.size() < 2) {        
            std::cout << "no path given" << std::endl;
            std::cout << "usage: test <flag>* path/file" << std::endl;
            std::cout << "(flags)  -e  [default] checks if file/directory exists" << std::endl;
            std::cout << "         -f  checks if file exists" << std::endl;
            std::cout << "         -d  checks if directory exists" << std::endl;
            return false;
        } else if (args.size() == 2) { //Checks size to see if user inputed flag. If no flag, run as -e
            if (stat(args[1], &buff) == 0) {
		std::cout << "(True)" << std::endl;
		return true;
	    }
	}	
	else if (strcmp(args[1], "-d") == 0) { //Checks to see if path leads to a directory
            if(stat(args[2], &buff) == 0 && S_ISDIR(buff.st_mode)) {
	        std::cout << "(True)" << std::endl;
		return true;
	    }
	}
	else if (strcmp(args[1], "-f") == 0) { //Checks to see if path leads to a regular file
	    if(stat(args[2], &buff) == 0 && S_ISREG(buff.st_mode)) {
		std::cout << "(True)" << std::endl;
		return true;
	    }
	}
	else if (strcmp(args[1], "-e") == 0) { //Checks to see if path is either a directory or a regular file
            if (stat(args[2], &buff) == 0) {
		std::cout << "(True)" << std::endl;
	        return true;
	    }
	}
	else { //If flag given is not 'd', 'f', or 'e', simply returns false.
            std::cout << "invalid flag" << std::endl;
	    std::cout << "usage: test <flag>* path/file" << std::endl;
            std::cout << "(flags)  -e  [default] checks if file/directory exists" << std::endl;
            std::cout << "         -f  checks if file exists" << std::endl;
            std::cout << "         -d  checks if directory exists" << std::endl;
	    return false;
	}
        
        std::cout << "(False)" << std::endl;
        return false; 
    }


};

#endif
