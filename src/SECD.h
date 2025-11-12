#pragma once

#include "AST.h"

typedef syntax_tree::AST AST;
typedef std::shared_ptr<syntax_tree::ASTNode> Node;
typedef std::vector<std::shared_ptr<syntax_tree::ASTNode>> Vector;
typedef std::shared_ptr<syntax_tree::ListNode> ListNode;
typedef std::shared_ptr<syntax_tree::LiteralInt> LiteralInt;
typedef std::shared_ptr<syntax_tree::LiteralNil> LiteralNil;
typedef std::shared_ptr<syntax_tree::LiteralBool> LiteralBool;

class SECD {
    Node _stack;
    Node _environ;
    Node _control;
    Node _dump;

    Node _true;
    Node _false;
    Node _nil;

    Node _work;

    bool stopped;

    void _ld();
    void _ldc();
    void _ldf();
    void _ap();
    void _rtn();
    void _dum();
    void _rap();
    void _sel();
    void _join();
    void _car();
    void _cdr();
    void _atom();
    void _cons();
    void _equal();
    void _add();
    void _sub();
    void _mul();
    void _dive();
    void _rem();
    void _le();
    void _stop();

    void printSecdState(bool recLimit);

public:
    SECD();
    ~SECD();

    AST execute(Node fn, Node arg, bool showSteps, bool recLimit);
    
};