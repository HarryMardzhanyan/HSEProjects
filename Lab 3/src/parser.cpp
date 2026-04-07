#include "parser.hpp"
#include <cctype>
#include <stdexcept>
#include <algorithm>

Parser::Parser(Lexer &lexer) : lexer(lexer) {
    advance();
}

void Parser::advance() {
    if (lexer.has_next()) {
        current_token = lexer.next_token();
    } else {
        current_token = Token("", lexem_t::EOEX, lexer.get_pos());
    }
}

// Дополнительные функции
Function Parser::getFunction(const std::string &s) {
    std::string st = s;

    if (st == "sin") return Function::SIN;
    if (st == "cos") return Function::COS;
    if (st == "tan") return Function::TAN;
    if (st == "asin") return Function::ASIN;
    if (st == "acos") return Function::ACOS;
    if (st == "atan") return Function::ATAN;
    if (st == "log") return Function::LOG;
    if (st == "exp") return Function::EXP;
    if (st == "sqrt") return Function::SQRT;

    throw std::runtime_error("ERROR: Unknown function" + s);
}

BinaryOp Parser::getBinaryOp(const std::string& c) {
    if (c == "+") return BinaryOp::ADD;
    if (c == "-") return BinaryOp::SUB;
    if (c == "*") return BinaryOp::MUL;
    if (c == "/") return BinaryOp::DIV;
    if (c == "^") return BinaryOp::POW;

    throw std::runtime_error("ERROR: Unknown operator" + c);
}

UnaryOp Parser::getUnaryOp(const std::string &c) {
    if (c == "+") return UnaryOp::PLUS;
    if (c == "-") return UnaryOp::MINUS;

    throw std::runtime_error("ERROR: Unknown operator" + c);
}

void Parser::match(lexem_t expected_lexem, const std::string &error_message) {
    if (current_token.type != expected_lexem) {
        throw std::runtime_error("ERROR: " + error_message + " at position " + std::to_string(current_token.position));
    }
}

// Парсер
std::unique_ptr<ASTNode> Parser::parse() {
    auto result = parse_expression();
    if (current_token.type != lexem_t::EOEX) {
        throw std::runtime_error("ERROR: Unexpected token");
    }
    return result;
}

std::unique_ptr<ASTNode> Parser::parse_expression() {
    auto left = parse_muldiv();

    // Как встречаем плюс либо минус (бинарные), то строим узлы
    while (current_token.type == lexem_t::OPERATION && (current_token.value == "+" || current_token.value == "-")) {
        BinaryOp op = getBinaryOp(current_token.value);
        advance();
        auto right = parse_muldiv();
        left = std::make_unique<BinaryOpNode> (op, std::move(left), std::move(right));
    }
    return left;
}

std::unique_ptr<ASTNode> Parser::parse_muldiv() {
    auto left = parse_unary();

    while (current_token.type == lexem_t::OPERATION && (current_token.value == "*" || current_token.value == "/")) {
        BinaryOp op = getBinaryOp(current_token.value);
        advance();
        auto right = parse_unary();
        left = std::make_unique<BinaryOpNode> (op, std::move(left), std::move(right));
    }
    return left;
}

std::unique_ptr<ASTNode> Parser::parse_unary() {
    if (current_token.type == lexem_t::OPERATION && (current_token.value == "+" || current_token.value == "-")) {
        UnaryOp op = getUnaryOp(current_token.value);
        advance();
        return std::make_unique<UnaryOpNode> (op, parse_unary());
    }
    return parse_power();
}

std::unique_ptr<ASTNode> Parser::parse_power() {
    auto base = parse_primary();

    if (current_token.type == lexem_t::OPERATION && current_token.value == "^") {
        advance();
        auto exponent = parse_unary();
        return std::make_unique<BinaryOpNode> (BinaryOp::POW, std::move(base), std::move(exponent));
    }
    return base;
}

std::unique_ptr<ASTNode> Parser::parse_primary() {
    if (current_token.type == lexem_t::NUMBER) {
        double value = std::stod(current_token.value);
        advance();
        return std::make_unique<NumberNode> (value);
    }

    if (current_token.type == lexem_t::VARIABLE) {
        std::string name = current_token.value;
        advance();
        return std::make_unique<VariableNode> (name);
    }

    if (current_token.type == lexem_t::FUNCTION) {
        std::string name = current_token.value;
        return parse_function(name);
    }

    if (current_token.type == lexem_t::LPAREN) {
        advance();                                          // Переходим на токен после скобки
        auto rest_expr = parse_expression();                // Парсим оставшееся выражение до правой круглой скобки
        match(lexem_t::RPAREN, "Expected right paren");     // Необходима правая скобка, так как была использована левая
        advance();                                          // Переходим к следующему токену
        return rest_expr;
    }

    throw std::runtime_error("ERROR: Unknown token " + current_token.value + " at position " + std::to_string(current_token.position));   // Если до данного момента ничего не смогли вернуть, то выкидываем ERROR
}

std::unique_ptr<ASTNode> Parser::parse_function(const std::string& name) {
    Function func = getFunction(name);

    // Сейчас current_token.value равняется функции
    advance();    // А теперь равняется, предположительно, левой круглой скобке

    // Проверим, что она равняется левой скобке
    match(lexem_t::LPAREN, "Expected left paren for the argument");

    advance();    // Съели левую скобку

    // Если мы прошли предыдущий match, то начинаем разбирать аргумент
    auto arg = parse_expression();

    // Необходимо использовать правую скобку вконце аргумента
    match(lexem_t::RPAREN, "Expected right paren");

    advance();

    return std::make_unique<FunctionNode> (func, std::move(arg));
}