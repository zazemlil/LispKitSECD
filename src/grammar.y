%{
#include "Scanner.hpp"
%}

%require "3.7.4"
%language "C++"
%defines "Parser.hpp"
%output "Parser.cpp"

%define api.parser.class {Parser}
%define api.namespace {lisp_for_kids}
%define api.value.type variant
%param {yyscan_t scanner}
%parse-param {syntax_tree::AST& result}

%locations

%code requires {
    #include "AST.h"
}

%code provides
{
    #define YY_DECL \
        int yylex(lisp_for_kids::Parser::semantic_type *yylval, \
                lisp_for_kids::Parser::location_type* yylloc, \
                yyscan_t yyscanner)
    YY_DECL;
}

%nonassoc <std::string> T_IDENTIFIER
%nonassoc <int> T_LITERAL_INT
%nonassoc T_LITERAL_NIL T_LITERAL_TRUE T_LITERAL_FALSE
%nonassoc T_PARENTHESIS_OPEN T_PARENTHESIS_CLOSE
%nonassoc T_END_OF_FILE

%type <std::shared_ptr<syntax_tree::ASTNode>> s expr atom list num id

%%

s: expr T_END_OF_FILE {
    result = syntax_tree::AST($1);
    YYACCEPT;
};


expr: atom { $$ = $1; }
    | T_PARENTHESIS_OPEN list T_PARENTHESIS_CLOSE { $$ = $2; };


atom: id { $$ = $1; }
    | num { $$ = $1; }
    | T_LITERAL_NIL { $$ = std::make_shared<syntax_tree::LiteralNil>("NIL"); }
    | T_LITERAL_TRUE { $$ = std::make_shared<syntax_tree::LiteralBool>("LiteralBool", true); }
    | T_LITERAL_FALSE { $$ = std::make_shared<syntax_tree::LiteralBool>("LiteralBool", false); };

list: expr list {
        auto l = std::make_shared<syntax_tree::ListNode>("LIST");
        l->addStatement($1);
        l->addStatements($2->getStatements());
        $$ = l;
    }
    | %empty { $$ = std::make_shared<syntax_tree::LiteralNil>("NIL"); };

num: T_LITERAL_INT { $$ = std::make_shared<syntax_tree::LiteralInt>("LiteralInt", $1); };
id: T_IDENTIFIER { $$ = std::make_shared<syntax_tree::Identifier>("Identifier", $1); };

%%

void lisp_for_kids::Parser::error(const location_type& loc, const std::string& msg) {
    const char* text = yyget_text(scanner);
    int length = yyget_leng(scanner);
    
    std::cerr << msg << " at (Line: " << loc.begin.line << ", Column: " << loc.begin.column
            << ", Last token: '" << std::string(text, length) << "')\n";
}