#include "SECD.h"

SECD::SECD() {
    _stack = std::make_shared<syntax_tree::LiteralNil>("NIL");
    _environ = std::make_shared<syntax_tree::LiteralNil>("NIL");
    _control = std::make_shared<syntax_tree::LiteralNil>("NIL");
    _dump = std::make_shared<syntax_tree::LiteralNil>("NIL");

    _true = std::make_shared<syntax_tree::LiteralBool>("LiteralBool", true);
    _false = std::make_shared<syntax_tree::LiteralBool>("LiteralBool", false);
    _nil = std::make_shared<syntax_tree::LiteralNil>("NIL");

    _work = std::make_shared<syntax_tree::LiteralNil>("NIL");

    stopped = false;
}

SECD::~SECD() {}

AST SECD::execute(Node fn) {
    std::cout << "executing...\n";
    return syntax_tree::AST();
}