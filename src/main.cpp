#include "parser.h"
#include "action.h"

#include <iostream>
#include <string.h>
#include <unistd.h>

int main() {
    char* user;
    char* hostname = new char[256];

    if (gethostname(hostname, 256) == -1) {
        perror("Could not get hostname");
        delete [] hostname;
        hostname = strdup("host");
    }
	
    if ((user = getlogin()) == NULL) {
        perror("Could not get logged in user");
        user = strdup("user");
    }

    std::string input = " ";
    std::vector<char*> tokens;
	
    while (true) {
        std::cout << user << "@" << hostname << "$ ";
        getline(std::cin, input);

        if (input.empty() || input.find_first_not_of(" ") 
              == std::string::npos) {
            std::cin.clear();
            std::cout << std::endl;
            continue;
        }
	    
        Parser* parser = new Parser((char*)input.c_str()) ;
        Action* root = parser->generateExecTree();
        
        if (root == NULL) {
            std::cout << std::endl;
            continue;
        }

        root->execute(0, 1);

        std::cout << std::endl;

        delete parser;
        delete root;
    }

    delete user;
    delete [] hostname;

    return 0;
}

