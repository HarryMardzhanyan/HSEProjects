// cd build (папка build уже существует)
// make clean && cmake .. && make

#include <iostream>
#include <string>
#include <unordered_map>
#include <algorithm>
#include <cctype>

#include "lexer.hpp"
#include "parser.hpp"
#include "visitor.hpp"

int main() {
    std::string command;
    std::cin >> command;

    try {
        if (command == "evaluate") {
            int n;
            std::cin >> n;

            std::vector<std::string> var_names(n);
            for (int i=0; i<n; ++i) {
                std::cin >> var_names[i];
            }

            std::unordered_map<std::string, double> variables;
            for (int i=0; i<n; ++i) {
                double value;
                std::cin >> value;
                variables[var_names[i]] = value;
            }

            std::string expression;
            std::getline(std::cin >> std::ws, expression);

            Lexer lex(expression);
            Parser parser(lex);
            auto ast = parser.parse();
            double result = CalcCommands::evaluate(ast, variables);
            std::cout.precision(6);
            std::cout << result << std::endl;
        }
        
        else if (command == "derivative") {
            int n;
            std::cin >> n;

            std::vector<std::string> var_names(n);
            for (int i=0; i<n; ++i) {
                std::cin >> var_names[i];
            }

            std::unordered_map<std::string, double> variables;
            for (int i=0; i<n; ++i) {
                double value;
                std::cin >> value;
                variables[var_names[i]] = value;
            }

            std::string expression;
            std::getline(std::cin >> std::ws, expression);

            Lexer lex(expression);
            Parser parser(lex);
            auto ast = parser.parse();
            auto deriv = CalcCommands::differentiate(ast, var_names[0]);
            std::cout << deriv->to_string() << std::endl;
        }

        else if (command == "evaluate_derivative") {
            int n;
            std::cin >> n;

            std::vector<std::string> var_names(n);
            for (int i=0; i<n; ++i) {
                std::cin >> var_names[i];
            }

            std::unordered_map<std::string, double> variables;
            for (int i=0; i<n; ++i) {
                double value;
                std::cin >> value;
                variables[var_names[i]] = value;
            }

            std::string expression;
            std::getline(std::cin >> std::ws, expression);

            Lexer lex(expression);
            Parser parser(lex);
            auto ast = parser.parse();
            auto deriv = CalcCommands::differentiate(ast, var_names[0]);
            double result = CalcCommands::evaluate(deriv, variables);
            std::cout << result << std::endl;
        }
    }
    catch (const std::exception& e) {
        std::cout << e.what() << std::endl;
    }
}