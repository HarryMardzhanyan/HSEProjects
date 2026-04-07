#include "lexer.hpp"
#include <string>
#include <iostream>
#include <stdexcept>
#include <vector>
#include <cctype>
#include <algorithm>

// Основной конструктор для получения выражения
Lexer::Lexer(const std::string &expr): expression(expr), current_token("", lexem_t::EOEX, 0) {}

// Список всех поддерживаемых функций
const std::vector<std::string> Lexer::functions = {
    "sin", "cos", "tan", "asin", "acos", "atan", "exp", "log", "sqrt"
};


// Вспомогательные функции для получения символов
// Функция для получения символа под индексом position и передвижение position вперед на 1 символ
char Lexer::get_char() {
    if (position < expression.length()) {
        return expression[position++];
    }
    return '\0';
}

// Функция для получения символа под индексом position
char Lexer::peek_char() {
    if (position < expression.length()) {
        return expression[position];
    }
    return '\0';
}

// Функция для пропуска пробелов
void Lexer::skip_space() {
    while (position < expression.length() && std::isspace(peek_char())) {
        ++position;
    }
}


// Вспомогательные функции для того, чтобы узнать символ или идентификатор
bool Lexer::is_alpha(char c) const {
    return (std::isalpha(c) || c == '_');
}

bool Lexer::is_number(char c) const {
    return std::isdigit(c);
}

bool Lexer::is_operation(char c) const {
    return (c == '+' || c == '-' || c == '*' || c == '/' || c == '^');
}

bool Lexer::is_function(const std::string &name) const {
    std::string low_name = name;
    return std::find(functions.begin(), functions.end(), low_name) != functions.end();
}


// Функции для парсинга слагаемых (переменных и функций) в выражении
Token Lexer::scan_identificator() {
    size_t start = position;
    std::string value;

    while (is_alpha(peek_char()) || is_number(peek_char())) {
        value += get_char();
    }
    std::transform(value.begin(), value.end(), value.begin(), ::tolower);

    if (is_function(value)) {
        return Token{value, lexem_t::FUNCTION, start};
    }

    return Token{value, lexem_t::VARIABLE, start};
}

Token Lexer::scan_number() {
    size_t start = position;
    std::string value;
    bool has_dot = false;
    bool digit_before_dot = false;
    bool digit_after_dot = false;

    char first_char = get_char();
    value += first_char;

    if (first_char == '0') {
        if (peek_char() == '.') {
            digit_before_dot = true;
        } else if (is_number(peek_char())) {
            throw std::runtime_error("ERROR: The fractional number contains leading zeros");
        } else {
            digit_before_dot = true;
        }
    }
    else {
        digit_before_dot = true;
    }

    while (is_number(peek_char()) || peek_char() == '.') {
        if (peek_char() == '.') {
            if (has_dot) {
                throw std::runtime_error("ERROR: Many dots in a fractional number");
            }
            has_dot = true;
            value += get_char();
        }
        else {
            if (has_dot) {
                digit_after_dot = true;
            }
            value += get_char();
        }
    }

    // Экспоненциальная форма записи числа
    if ((peek_char() == 'e' || peek_char() == 'E') && digit_before_dot) {
        if (first_char == '0') {
            throw std::runtime_error("ERROR: In exponential form, the mantissa cannot start with zero");
        }

        value += get_char();
        digit_after_dot = true;

        if (peek_char() == '+' || peek_char() == '-') {
            value += get_char();
        }

        if (!is_number(peek_char())) {
            throw std::runtime_error("ERROR: In exponential form of a number, a number is required after 'e'");
        }
        while (is_number(peek_char())) {
            value += get_char();
        }
    }

    if (has_dot && !digit_after_dot) {
        throw std::runtime_error("ERROR: The number must have digits after the period");
    }

    // Если буква после цифры
    if (is_alpha(peek_char())) {
        throw std::runtime_error("ERROR: The number cannot be immediately followed by an identifier");
    }

    return Token{value, lexem_t::NUMBER, start};
}

Token Lexer::scan_operation() {
    size_t start = position;
    char c = get_char();
    return Token{std::string(1, c), lexem_t::OPERATION, start};
}

Token Lexer::next_token() {
    skip_space();

    if (position >= expression.length()) {
        current_token = Token{"", lexem_t::EOEX, position};
        return current_token;
    }

    char current_char = peek_char();

    switch (current_char) {
        case '(':
            current_token = Token{"(", lexem_t::LPAREN, position++};
            break;

        case ')':
            current_token = Token{")", lexem_t::RPAREN, position++};
            break;
        
        default:
            if (is_alpha(current_char)) {
                current_token = scan_identificator();
                break;
            }
            else if (is_operation(current_char)) {
                current_token = scan_operation();
                break;
            }
            else if (is_number(current_char)) {
                current_token = scan_number();
                break;
            }
            else {
                throw std::runtime_error("ERROR lexer: unknown token");
            }
    }

    return current_token;
}

Token Lexer::peek_token() {
    size_t old_pos = position;
    Token tok = next_token();
    position = old_pos;
    return tok;
}

bool Lexer::has_next() const {
    size_t temp_pos = position;
    while (temp_pos < expression.length() && std::isspace(expression[temp_pos])) {
        ++temp_pos;
    }
    return (temp_pos < expression.length());
}