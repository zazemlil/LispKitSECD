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

void SECD::_car()
{
}

void SECD::_cdr()
{
}

void SECD::_atom()
{
}

void SECD::_cons()
{
}

void SECD::_equal()
{
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
