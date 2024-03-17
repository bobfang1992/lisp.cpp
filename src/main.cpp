#include <iostream>

#include <editline/readline.h>

#include <peglib.h>

int main(int argc, char **argv) {

    /* Print Version and Exit Information */
    std::cout << "Lisp.cpp\n";
    std::cout << "Press Ctrl+c to Exit\n";

    peg::parser parser(R"(
Lispy     <-  Expr+
Expr      <-  Number / '(' _ Operator _ ExprList _ ')'
ExprList  <-  Expr (_ Expr)*
Number    <-  '-'? [0-9]+
Operator  <-  '+' / '-' / '*' / '/'
_         <-  [ \t\r\n]*
    )");

    parser.set_logger([](size_t line, size_t col, const std::string& msg, const std::string &rule) {
        std::cout << "Error:" << line << ":" << col << ": " << msg << "\n";
    });

    parser["Operator"] = [](const peg::SemanticValues& val) {
        std::cout << "Operator " << val.token_to_string()  << "\n";
    };

    parser["Lispy"] = [](const peg::SemanticValues& val) {
        std::cout << "Lispy " << val.token_to_string()  << "\n";
    };

    parser["Expr"] = [](const peg::SemanticValues& val) {
        std::cout << "Expr " << val.token_to_string()  << "\n";
    };

    parser["Number"] = [](const peg::SemanticValues& val) {
        std::cout << "Number " << val.token_to_string()  << "\n";
    };

    assert(static_cast<bool>(parser) == true);

    /* In a never ending loop */
    while (true) {

        /* Output our prompt and get input */
        char *input = readline("lisp.cpp> ");

        /* Add input to history */
        add_history(input);

        if (std::string(input) == "exit") {
            exit(0);
        }
        /* Echo input back to user */
        std::cout << "parsing result: " << std::boolalpha << parser.parse(input) << "\n";

        /* Free retrieved input */
        free(input);

    }

    return 0;
}