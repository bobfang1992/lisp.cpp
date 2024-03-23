#include <iostream>

#include <editline/readline.h>
#include <peglib.h>

std::string LispGrammar = R"(
Expr      <-  Number / '(' _ Operator _ ExprList _ ')'
ExprList  <-  Expr (_ Expr)*
Number    <-  '-'? [0-9]+
Operator  <-  '+' / '-' / '*' / '/'
_         <-  [ \t\r\n]*
)";

void eval(const peg::Ast &ast) {
  if (ast.name == "Number") {
    std::cout << "Number: " << ast.token << "\n";
  } else if (ast.name == "Operator") {
    std::cout << "Operator: " << ast.token << "\n";
  } else if (ast.name == "Expr") {
    std::cout << "Expr: " << ast.nodes.size() << "\n";
    for (auto &node : ast.nodes) {
      eval(*node);
    }
  } else if (ast.name == "ExprList") {
    std::cout << "ExprList: " << ast.nodes.size() << "\n";
    for (auto &node : ast.nodes) {
      eval(*node);
    }
  }
}

int main(int argc, char **argv) {

  /* Print Version and Exit Information */
  std::cout << "Lisp.cpp\n";
  std::cout << "Press Ctrl+c to Exit\n";

  peg::parser parser(LispGrammar);

  parser.set_logger([](size_t line, size_t col, const std::string &msg,
                       const std::string &rule) {
    std::cout << "Error:" << line << ":" << col << ": " << msg << "\n";
  });

  parser.enable_ast();

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

    std::shared_ptr<peg::Ast> ast;

    auto success = parser.parse(input, ast);

    if (!success) {
      std::cout << "Syntax Error\n";
      return 1;
    }

    ast = parser.optimize_ast(ast);

    std::cout << "AST: " << peg::ast_to_s(ast) << "\n\n";

    eval(*ast);

    /* Free retrieved input */
    free(input);
  }

  return 0;
}