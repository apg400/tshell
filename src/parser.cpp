// PARSER definition

#include "parser.h"
#include "pipe.h"
#include "redirect.h"

Parser::Parser(char* in) {
    input = in;
}

// Uses strtok to split input string into individual words
// returns a vector of char* as the parsed string
std::vector<char*> Parser::parse() {
    std::vector<char*> output;
    char hold[256];
    hold[0] = '\0';

    for (int i = 0; input[i] != '\0' && input[i] != '#'; i++) {
        switch (input[i]) {
            case ' ': if (strlen(hold) > 0) output.push_back(strdup(hold));
                      hold[0] = '\0';
                      break;
            case ';':
            case '(':
            case ')':
            case '[':
            case ']': if (strlen(hold) > 0) output.push_back(strdup(hold));
                      output.push_back(strndup(&input[i], 1));
                      hold[0] = '\0';
                      break;
            case '|': if (input[i+1] == '|') {
                          if (strlen(hold) > 0) output.push_back(strdup(hold));
                          output.push_back(strndup(&input[i], 2));
                          hold[0] = '\0';
                          i++;
                      } else {
                          strncat(hold, &input[i], 1);
                      }
                      break;
            case '&': if (input[i+1] == '&') {
                          if (strlen(hold) > 0) output.push_back(strdup(hold));
                          output.push_back(strndup(&input[i], 2));
                          hold[0] = '\0';
                          i++;
                      } else {
                          strncat(hold, &input[i], 1);
                      }
                      break; 
            default: strncat(hold, &input[i], 1);
        } 

    }

    if (strlen(hold) > 0) {
        output.push_back(strdup(hold));
    }

    return output;
}

// Contrusts an Operator object and returns it
// used to make generating the Execution Tree easier
Action* Parser::buildOperator(Action* left, Action* right, char* type) {
    if (strcmp(type, "&&") == 0) {
        return new And(left, right);
    } else if (strcmp(type, "||") == 0) { 
        return new Or(left, right);
    } else if (strcmp(type, ";") == 0) { 
        return new Connect(left, right);
    } else {
        return new Pipe(left, right);
    }
}

//
Action* Parser::buildCommand(char* input) {
    std::vector<char*> cmd;
    if (strlen(input) == 0)
        return new Command(cmd);

    char* token = strtok(input, " ");
    while (token != NULL) {
        cmd.push_back(token);
        token = strtok(NULL, " ");
    }
    
    bool input_redirect = false;
    bool output_redirect = false;
    bool append_redirect = false;
    char* filename = nullptr;

    for (auto c : cmd) {
        if (strcmp(c, ">") == 0) {
            output_redirect = true;
        } 

        if (strcmp(c, ">>") == 0) {
            append_redirect = true;
        }

        if (strcmp(c, "<") == 0) {
            input_redirect = true;
        }
    }

    if (output_redirect || input_redirect || append_redirect) {
        if (cmd.size() == 1) {
            perror("invalid syntax");
            cmd.clear();
            return new Command(cmd);
        } else if (cmd.size() == 2) {
            if (strcmp(cmd[0], ">") != 0) {
                perror("invalid syntax");
                cmd.clear();
                return new Command(cmd); 
            }
        } else {
            if (strcmp(cmd[cmd.size()-1], ">") == 0 || strcmp(cmd[cmd.size()-1], "<") == 0 || strcmp(cmd[cmd.size()-1], ">>") == 0) {
                perror("invalid syntax");
                cmd.clear();
                return new Command(cmd);
            } else {
                filename = cmd[cmd.size()-1];
                cmd.pop_back();
                cmd.pop_back();
            }
        }
    }
    
    Action* final_command = nullptr;

    if (strcmp(cmd[0], "test") == 0) {
        final_command =  new TestCMD(cmd);
    } else if (strcmp(cmd[0], "[") == 0) {
        cmd.pop_back();
        final_command = new TestCMD(cmd);
    } else {
        final_command =  new Command(cmd);
    }

    if (input_redirect) {
        return new RedirectInput(final_command, filename);
    } else if (output_redirect) {
        return new RedirectOutput(final_command, filename);
    } else if (append_redirect) {
        return new AppendOutput(final_command, filename);
    } else {
        return final_command;
    }
}

// Checks that the number of open and closed parenthesis is the same
bool Parser::checkParenthesis(std::vector<char*> input) {
    int open_count = 0;
    int closed_count = 0;
    
    for (char* p : input) {
        if (strcmp(p, "(") == 0)
            open_count++;
        else if (strcmp(p, ")") == 0)
            closed_count++;
    }

    if (open_count == closed_count) 
        return true;
   
    return false;
}

// Checks if the string is an operator
bool Parser::isOperator(char* op) {
    if (strcmp(op, "&&") == 0 || strcmp(op, "||") == 0 || strcmp(op, ";") == 0 || strcmp(op, "|") == 0)
        return true;
    return false;
}

// Creates all the necessary Action objects and constructs the tree
// returns the Action ptr to the root of the tree
Action* Parser::generateExecTree() {
    std::vector<char*> parsed = parse();
    
    // if it has an uneven number of parenthesis it does not generate a tree
    if (!checkParenthesis(parsed)) {
        perror("missing parenthesis");
        return NULL;
    }

    std::stack<char*> cstack;
    std::queue<char*> cqueue;
    char* temp;
    char cmd[128];
    cmd[0] = '\0';
 
    // transforms input from infix to postfix order and stores it in a queue
    for (char* p : parsed) {
        if (strcmp(p, "(") == 0) {

            if (strlen(cmd) > 0) {
                cstack.push(strdup(cmd));
                cmd[0] = '\0';
            }
            cstack.push(strdup(p));

        } else if (strcmp(p, ")") == 0) {

            if (strlen(cmd) > 0) {
                cstack.push(strdup(cmd));
                cmd[0] = '\0';
            }
            while (!cstack.empty() && strcmp(cstack.top(), "(") != 0) {
                cqueue.push(strdup(cstack.top()));
                cstack.pop();
                if (cstack.empty()) break;
            }
            if (!cstack.empty())
                cstack.pop();

        } else if (isOperator(p)) {

            if (strlen(cmd) > 0) {
                cstack.push(strdup(cmd));
                cmd[0] = '\0';
            }
            while (!cstack.empty() && !isOperator(cstack.top()) 
                    && strcmp(cstack.top(), "(") != 0) {
                cqueue.push(strdup(cstack.top()));
                cstack.pop();
                if (cstack.empty()) break;
            }
            if (!cstack.empty() && isOperator(cstack.top())) {
                cqueue.push(strdup(cstack.top()));
                cstack.pop();
            }
            cstack.push(strdup(p));

        } else {

            strcat(cmd, " ");
            strcat(cmd, p);
        }
    }    
   
    if (strlen(cmd) > 0) {
        cstack.push(cmd);
    }

    while (!cstack.empty()) { 
        cqueue.push(strdup(cstack.top()));
        cstack.pop();
    }

    // builds execution tree using the data in the queue
    // uses algorithm for executing prefix
    std::stack<Action*> estack;
    Action* left;
    Action* right;
    Action* root = NULL;
    while (!cqueue.empty()) {
        if (isOperator(cqueue.front())) {
            if (estack.empty()) {
                root = nullptr;
                estack.push(root);
            } else if (estack.size() == 1) {
                left = estack.top();
                estack.pop();
                right = nullptr;
                root = buildOperator(left, right, cqueue.front());
                estack.push(root);
            } else {
                right = estack.top();
                estack.pop();
                left = estack.top();
                estack.pop();
                root = buildOperator(left, right, cqueue.front());
                estack.push(root);
            }
        } else {
            root = buildCommand(cqueue.front());
            estack.push(root);
        }
        cqueue.pop();
    }

    if (estack.empty())
        root = nullptr;
    else
        root = estack.top();
    
    return root;

}

