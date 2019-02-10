#ifndef OPERATOR_H
#define OPERATOR_H

#include "action.h"

class Operator : public Action {
protected:
    Action* left;
    Action* right;
public:
    Operator(Action* l, Action* r) {
        left = l;
        right = r;
    }

    virtual bool execute(int readfile, int writefile) = 0;
};

// And class for &&
class And : public Operator {
public:
    And(Action* l, Action* r) : Operator(l, r) { }

    virtual bool execute(int readfile, int writefile) {
        if (left == nullptr && right == nullptr) return false;
        if (left == nullptr) return false;
        if (right == nullptr) {
            left->execute(readfile, writefile);
            return false;
        } 
        return left->execute(readfile, writefile) ? right->execute(readfile, writefile) : false;
    }
};

// Or class for ||
class Or : public Operator {
public:
    Or(Action* l, Action* r) : Operator(l, r) { }

    virtual bool execute(int readfile, int writefile) {
        bool result = false;
        if (left != nullptr) result = left->execute(readfile, writefile);
        if (right != nullptr) result = result ? true : right->execute(readfile, writefile); 
        return result;
    }
};

// Connect class for ;
class Connect : public Operator {
public:
    Connect(Action *l, Action *r) : Operator(l, r) { }

    virtual bool execute(int readfile, int writefile) {
        if (left != nullptr) left->execute(readfile, writefile);
        if (right != nullptr) return right->execute(readfile, writefile); 
        return false;
    }
};

#endif
