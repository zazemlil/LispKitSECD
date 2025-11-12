#include <iostream>
#include "AST.h"

#include "SECD.h"

extern syntax_tree::AST analize(char* arg);

int main(int argc, char* argv[])
{   
    bool s_flag = false;
    int new_argc = argc;
    for (int i = 1; i < new_argc; i++) {
        if (std::string(argv[i]) == "-s") {
            s_flag = true;
            for (int j = i; j < new_argc - 1; j++) {
                argv[j] = argv[j + 1];
            }
            new_argc--;
            i--;
        }
    }
    argc = new_argc;

    if (argc < 2) {
        std::cerr << "Usage: " << argv[0] << " <input_file>" << std::endl;
        return -1;
    }

    syntax_tree::AST ast = analize(argv[1]);

    std::cout << "-----------------------------\n";
    std::cout << "----Abstract syntax tree:----\n";
    std::cout << "-----------------------------\n";
    ast.print();

    syntax_tree::AST arg;
    
    if (argc == 3) {
        arg = analize(argv[2]);
    }

    SECD* secd = new SECD();
    syntax_tree::AST result;
    try {
        result = secd->execute(ast.getRoot(), arg.getRoot(), s_flag);
        std::cout << "SECD execution success.\n";
    }
    catch(const std::exception& e) {
        std::cerr << "SECD execution error: `" << e.what() << "`\n\n";
    }

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