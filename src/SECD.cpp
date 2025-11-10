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

    return syntax_tree::AST(_stack);
}

void SECD::_ld()
{
}

void SECD::_ldc() {
    _control->cdr()->car()->cons(_stack)->printFlat();
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

void SECD::_add()
{
}

void SECD::_sub()
{
}

void SECD::_mul()
{
}

void SECD::_dive()
{
}

void SECD::_rem()
{
}

void SECD::_le()
{
}

void SECD::_stop() {
    stopped = true;
}
