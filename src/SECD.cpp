#include "SECD.h"

SECD::SECD() {
    _true = std::make_shared<syntax_tree::LiteralBool>("LiteralBool", true);
    _false = std::make_shared<syntax_tree::LiteralBool>("LiteralBool", false);
    _nil = std::make_shared<syntax_tree::LiteralNil>("NIL");
}

SECD::~SECD() {}

void SECD::printSecdState(bool recLimit) {
    std::cout << "\n###########################\n_Stack = \n";
    if (recLimit) {
        _stack->printRecFlatStack(0, 3);
        std::cout << "\n\n_Environ = \n";
        _environ->printRecFlatStack(0, 3);
        std::cout << "\n\n_Control = \n";
        _control->printRecFlatStack(0, 3);
        std::cout << "\n\n_Dump = \n";
        _dump->printRecFlatStack(0, 3);
    }
    else {
        _stack->printFlatStack();
        std::cout << "\n\n_Environ = \n";
        _environ->printFlatStack();
        std::cout << "\n\n_Control = \n";
        _control->printFlatStack();
        std::cout << "\n\n_Dump = \n";
        _dump->printFlatStack();
    }
    std::cout << "\n---------------------------\n";

    std::cin.get();
}

AST SECD::execute(Node fn, Node arg, bool showSteps, bool recLimit) {
    if (arg != nullptr) { _stack = arg; }
    else { _stack = _nil; }
    
    _environ = _nil;
    _control = fn;
    _dump = _nil;

    _work = _nil;

    stopped = false;

    if (showSteps) printSecdState(recLimit);

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
        if (showSteps) printSecdState(recLimit);
    }

    return syntax_tree::AST(_stack->getStatement(0));
}

void SECD::_ld() {
    _work = _environ;

    auto v1 = std::dynamic_pointer_cast<syntax_tree::LiteralInt>(_control->cdr()->car()->car());
    auto v2 = std::dynamic_pointer_cast<syntax_tree::LiteralInt>(_control->cdr()->car()->cdr()->car());

    for (int i = 1; i <= v1->getValue(); ++i) {
        _work = _work->cdr();
    }
    _work = _work->car();
    for (int i = 1; i <= v2->getValue(); ++i) {
        _work = _work->cdr();
    }
    _work = _work->car();
    _stack = _work->cons(_stack);
    _control = _control->cdr()->cdr();
    _work = _nil;
}

void SECD::_ldc() {
    _stack = _control->cdr()->car()->cons(_stack);
    _control = _control->cdr()->cdr();
}

void SECD::_ldf() {
    _work = _control->cdr()->car()->cons(_environ);
    _stack = _work->cons(_stack);
    _control = _control->cdr()->cdr();
    _work = _nil;
}

void SECD::_ap() {
    _dump = _control->cdr()->cons(_dump);
    _dump = _environ->cons(_dump);
    _dump = _stack->cdr()->cdr()->cons(_dump);
    _environ = _stack->cdr()->car()->cons(_stack->car()->cdr());
    _control = _stack->car()->car();
    _stack = _nil;
}

void SECD::_rtn() {
    _stack = _stack->car()->cons(_dump->car());
    _environ = _dump->cdr()->car();
    _control = _dump->cdr()->cdr()->car();
    _dump = _dump->cdr()->cdr()->cdr();
}

void SECD::_dum() {
    //auto omega = std::make_shared<syntax_tree::ListNode>("LIST");
    //_environ = omega->cons(_environ);
    _environ = _nil->cons(_environ); // 
    _control = _control->cdr();
}

void SECD::_rap() {
    _dump = _control->cdr()->cons(_dump);
    _dump = _environ->cdr()->cons(_dump);
    _dump = _stack->cdr()->cdr()->cons(_dump);
    _environ = _stack->car()->cdr();
    *_environ->car() = *_stack->cdr()->car();
    _control = _stack->car()->car();
    _stack = _nil;
}

void SECD::_sel() {
    if (auto bool_lit = std::dynamic_pointer_cast<syntax_tree::LiteralBool>(_stack->car())) {
        _dump = _control->cdr()->cdr()->cdr()->cons(_dump);

        if (bool_lit->getValue() == true) {
            _control = _control->cdr()->car();
        }
        else {
            _control = _control->cdr()->cdr()->car();
        }
        _stack = _stack->cdr();
    }
    else {
        throw std::runtime_error("Cond error: first param must be Boolean");
    }
}

void SECD::_join() {
    _control = _dump->car();
    _dump = _dump->cdr();
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
        if (auto first_lit = std::dynamic_pointer_cast<syntax_tree::LiteralNil>(first)) {
            if (auto second_lit = std::dynamic_pointer_cast<syntax_tree::LiteralNil>(second)) {
                auto new_bool = std::make_shared<syntax_tree::LiteralBool>("LiteralBool", true);
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
