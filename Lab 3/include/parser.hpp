#pragma once

#include "lexer.hpp"
#include "ast.hpp"
#include <string>
#include <memory>

class Parser {
public:
    explicit Parser(Lexer& lexer);       // Конструктор, принимающий лексер
    std::unique_ptr<ASTNode> parse();    // Метод для построения дерева
private:
    Lexer& lexer;
    Token current_token;                 // Текущий токен
    void advance();                      // Передвигаемся к следующему токену

    // Весь парсинг происходит рекурсивно. Парсеры расположнены в порядке приоритета операций (по возрастанию)
    // Основной парсер - parse_expression()
    std::unique_ptr<ASTNode> parse_expression();
    std::unique_ptr<ASTNode> parse_muldiv();
    std::unique_ptr<ASTNode> parse_unary();
    std::unique_ptr<ASTNode> parse_power();
    std::unique_ptr<ASTNode> parse_primary();
    std::unique_ptr<ASTNode> parse_function(const std::string& name);

    // Дополнительные функции
    // Функция, чтобы преобразовать строку в математическую функцию
    Function getFunction(const std::string& s);

    // Функция, чтобы преобразовать символ в операцию
    BinaryOp getBinaryOp(const std::string &c);
    UnaryOp getUnaryOp(const std::string &c);

    // Функция, чтобы выкидывать ошибки, если отсутствует ожидаемый токен
    void match(lexem_t expected_lexem, const std::string& error_message);
};