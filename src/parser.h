// PARSER
#ifndef PARSER_H
#define PARSER_H

#include "command.h"
#include "operator.h"
#include "testcmd.h"

#include <string.h>
#include <vector>
#include <stack>
#include <queue>

class Parser {
private:
    char* input;
public:
    Parser(char* in); 
    std::vector<char*> parse();
    Action* buildOperator(Action* left, Action* right, char* type);
    Action* buildCommand(char* input);
    bool isOperator(char* op);
    bool checkParenthesis(std::vector<char*> input);
    Action* generateExecTree();
};

#endif
