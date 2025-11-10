#pragma once

#include "AST.h"

typedef syntax_tree::AST AST;
typedef std::shared_ptr<syntax_tree::ASTNode> Node;
typedef std::vector<std::shared_ptr<syntax_tree::ASTNode>> Vector;
typedef std::shared_ptr<syntax_tree::ListNode> ListNode;
typedef std::shared_ptr<syntax_tree::LiteralInt> LiteralInt;
typedef std::shared_ptr<syntax_tree::LiteralNil> LiteralNil;
typedef std::shared_ptr<syntax_tree::LiteralBool> LiteralBool;

class SECD
{
private:
    Node _stack;
    Node _environ;
    Node _control;
    Node _dump;

    Node _true;
    Node _false;
    Node _nil;

    Node _work;

    bool stopped;

public:
    SECD();
    ~SECD();

    AST execute(Node fn);
};