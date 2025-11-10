#include <iostream>
#include "AST.h"

#include "SECD.h"

extern syntax_tree::AST analize(int argc, char* argv[]);

int main(int argc, char* argv[])
{   
    syntax_tree::AST ast = analize(argc, argv);

    std::cout << "-----------------------------\n";
    std::cout << "----Abstract syntax tree:----\n";
    std::cout << "-----------------------------\n";
    ast.print();

    SECD* secd = new SECD();
    syntax_tree::AST result = secd->execute(ast.getRoot());

    std::cout << "-----------------------------\n";
    std::cout << "---------Result AST:---------\n";
    std::cout << "-----------------------------\n";
    result.print(false);
    std::cout << "-----------------------------\n";
    std::cout << "-------Result flat AST:------\n";
    std::cout << "-----------------------------\n";
    result.print(true);
    std::cout << "\n";
    
    return 0;
}