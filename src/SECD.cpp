#include "SECD.h"

SECD::SECD() {
    _true = std::make_shared<syntax_tree::LiteralBool>("LiteralBool", true);
    _false = std::make_shared<syntax_tree::LiteralBool>("LiteralBool", false);
    _nil = std::make_shared<syntax_tree::LiteralNil>("NIL");
}

SECD::~SECD() {}

AST SECD::execute(Node fn) {
    _stack = _nil;
    _environ = _nil;
    _control = fn;
    _dump = _nil;

    _work = _nil;

    stopped = false;

    while (!stopped) {
        if (auto id = std::dynamic_pointer_cast<syntax_tree::Identifier>(_control->car())) {
            if      (id->getValue() == "LD")    { _ld(); }
            else if (id->getValue() == "LDC")   { _ldc(); }
            else if (id->getValue() == "LDF")   { _ldf(); }
            else if (id->getValue() == "AP")    { _ap(); }
            else if (id->getValue() == "RTN")   { _rtn(); }
            else if (id->getValue() == "DUM")   { _dum(); }
            else if (id->getValue() == "RAP")   { _rap(); }
            else if (id->getValue() == "SEL")   { _sel(); }
            else if (id->getValue() == "JOIN")  { _join(); }
            else if (id->getValue() == "CAR")   { _car(); }
            else if (id->getValue() == "CDR")   { _cdr(); }
            else if (id->getValue() == "ATOM")  { _atom(); }
            else if (id->getValue() == "CONS")  { _cons(); }
            else if (id->getValue() == "EQUAL") { _equal(); }
            else if (id->getValue() == "ADD")   { _add(); }
            else if (id->getValue() == "SUB")   { _sub(); }
            else if (id->getValue() == "MUL")   { _mul(); }
            else if (id->getValue() == "DIVE")  { _dive(); }
            else if (id->getValue() == "REM")   { _rem(); }
            else if (id->getValue() == "LE")    { _le(); }
            else if (id->getValue() == "STOP")  { _stop(); }
        }
    }

    return syntax_tree::AST(_stack->getStatement(0));
}

void SECD::_ld()
{
}

void SECD::_ldc() {
    _stack = _control->cdr()->car()->cons(_stack);
    _control = _control->cdr()->cdr();
}

void SECD::_ldf()
{
}

void SECD::_ap()
{
}

void SECD::_rtn()
{
}

void SECD::_dum()
{
}

void SECD::_rap()
{
}

void SECD::_sel()
{
}

void SECD::_join()
{
}

void SECD::_car() {
    _stack = _stack->car()->car()->cons(_stack->cdr());
    _control = _control->cdr();
}

void SECD::_cdr() {
    _stack = _stack->car()->cdr()->cons(_stack->cdr());
    _control = _control->cdr();
}

void SECD::_atom() {
    bool is_atom = (std::dynamic_pointer_cast<syntax_tree::ListNode>(_stack->car()) == nullptr);
    auto new_bool = std::make_shared<syntax_tree::LiteralBool>("LiteralBool", is_atom);
    _stack = new_bool->cons(_stack->cdr());
    _control = _control->cdr();
}

void SECD::_cons() {
    _stack = _stack->car()->cons(_stack->cdr()->car())->cons(_stack->cdr()->cdr());
    _control = _control->cdr();
}

void SECD::_equal() {
    auto first = _stack->cdr()->car();
    auto second = _stack->car();

    bool first_is_atom = (std::dynamic_pointer_cast<syntax_tree::ListNode>(first) == nullptr);
    bool second_is_atom = (std::dynamic_pointer_cast<syntax_tree::ListNode>(second) == nullptr);

    if (first_is_atom || second_is_atom) {
        if (auto first_lit = std::dynamic_pointer_cast<syntax_tree::LiteralInt>(first)) {
            if (auto second_lit = std::dynamic_pointer_cast<syntax_tree::LiteralInt>(second)) {
                auto new_bool = std::make_shared<syntax_tree::LiteralBool>("LiteralBool", first_lit->getValue() 
                                                                                        == second_lit->getValue());
                _stack = new_bool->cons(_stack->cdr()->cdr());
                _control = _control->cdr();
                return;
            }
        }
        if (auto first_lit = std::dynamic_pointer_cast<syntax_tree::LiteralBool>(first)) {
            if (auto second_lit = std::dynamic_pointer_cast<syntax_tree::LiteralBool>(second)) {
                auto new_bool = std::make_shared<syntax_tree::LiteralBool>("LiteralBool", first_lit->getValue() 
                                                                                        == second_lit->getValue());
                _stack = new_bool->cons(_stack->cdr()->cdr());
                _control = _control->cdr();
                return;
            }
        }
        if (auto first_lit = std::dynamic_pointer_cast<syntax_tree::Identifier>(first)) {
            if (auto second_lit = std::dynamic_pointer_cast<syntax_tree::Identifier>(second)) {
                auto new_bool = std::make_shared<syntax_tree::LiteralBool>("LiteralBool", first_lit->getValue() 
                                                                                        == second_lit->getValue());
                _stack = new_bool->cons(_stack->cdr()->cdr());
                _control = _control->cdr();
                return;
            }
        }
        auto new_bool = std::make_shared<syntax_tree::LiteralBool>("LiteralBool", false);
        _stack = new_bool->cons(_stack->cdr()->cdr());
        _control = _control->cdr();
        return;
    }

    throw std::runtime_error("Equal operation requires 1 or 2 atom operands");
}

void SECD::_add() {
    auto first = std::dynamic_pointer_cast<syntax_tree::LiteralInt>(_stack->cdr()->car());
    auto second = std::dynamic_pointer_cast<syntax_tree::LiteralInt>(_stack->car());
    if (first && second) {
        auto new_num = std::make_shared<syntax_tree::LiteralInt>("LiteralInt", first->getValue() + second->getValue());
        _stack = new_num->cons(_stack->cdr()->cdr());
        _control = _control->cdr();
    }
    else { throw std::runtime_error("Add operation requires integer operands"); }
}

void SECD::_sub() {
    auto first = std::dynamic_pointer_cast<syntax_tree::LiteralInt>(_stack->cdr()->car());
    auto second = std::dynamic_pointer_cast<syntax_tree::LiteralInt>(_stack->car());
    if (first && second) {
        auto new_num = std::make_shared<syntax_tree::LiteralInt>("LiteralInt", first->getValue() - second->getValue());
        _stack = new_num->cons(_stack->cdr()->cdr());
        _control = _control->cdr();
    }
    else { throw std::runtime_error("Sub operation requires integer operands"); }
}

void SECD::_mul() {
    auto first = std::dynamic_pointer_cast<syntax_tree::LiteralInt>(_stack->cdr()->car());
    auto second = std::dynamic_pointer_cast<syntax_tree::LiteralInt>(_stack->car());
    if (first && second) {
        auto new_num = std::make_shared<syntax_tree::LiteralInt>("LiteralInt", first->getValue() * second->getValue());
        _stack = new_num->cons(_stack->cdr()->cdr());
        _control = _control->cdr();
    }
    else { throw std::runtime_error("Mul operation requires integer operands"); }
}

void SECD::_dive() {
    auto first = std::dynamic_pointer_cast<syntax_tree::LiteralInt>(_stack->cdr()->car());
    auto second = std::dynamic_pointer_cast<syntax_tree::LiteralInt>(_stack->car());
    if (first && second) {
        auto new_num = std::make_shared<syntax_tree::LiteralInt>("LiteralInt", first->getValue() / second->getValue());
        _stack = new_num->cons(_stack->cdr()->cdr());
        _control = _control->cdr();
    }
    else { throw std::runtime_error("Dive operation requires integer operands"); }
}

void SECD::_rem() {
    auto first = std::dynamic_pointer_cast<syntax_tree::LiteralInt>(_stack->cdr()->car());
    auto second = std::dynamic_pointer_cast<syntax_tree::LiteralInt>(_stack->car());
    if (first && second) {
        auto new_num = std::make_shared<syntax_tree::LiteralInt>("LiteralInt", first->getValue() % second->getValue());
        _stack = new_num->cons(_stack->cdr()->cdr());
        _control = _control->cdr();
    }
    else { throw std::runtime_error("Rem operation requires integer operands"); }
}

void SECD::_le() {
    auto first = std::dynamic_pointer_cast<syntax_tree::LiteralInt>(_stack->cdr()->car());
    auto second = std::dynamic_pointer_cast<syntax_tree::LiteralInt>(_stack->car());
    if (first && second) {
        auto new_bool = std::make_shared<syntax_tree::LiteralBool>("LiteralBool", first->getValue() <= second->getValue());
        _stack = new_bool->cons(_stack->cdr()->cdr());
        _control = _control->cdr();
    }
    else { throw std::runtime_error("Le operation requires integer operands"); }
}

void SECD::_stop() {
    stopped = true;
}
