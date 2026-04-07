#pragma once

#include <string>
#include <iostream>
#include <stdexcept>
#include <vector>
#include <cctype>

enum class lexem_t {
    VARIABLE,
    FUNCTION,
    OPERATION,
    LPAREN,
    RPAREN,
    NUMBER,
    EOEX
};

struct Token {
    std::string value;      // Выражение токена
    lexem_t type;           // Тип токена
    size_t position;        // Первый индекс токена в исходном выражении

    // Конструктор по умолчанию
    Token() : value(""), type(lexem_t::EOEX), position(0) {}

    // Конструктор с параметрами
    Token(const std::string& v, lexem_t t, size_t pos): value(v), type(t), position(pos) {}

    bool operator==(const std::string& v) const {return value == v;};
};

class Lexer {
    // Переменные, содержащие основную информацию о выражении
    std::string expression;     // Само выражение, которое нужно посчитать или продифференцировать
    size_t position = 0;        // Текущая позиция
    Token current_token;        // Токен, который заполняем символами

    // Вспомогательные функции для получения символов
    char get_char();
    char peek_char();
    void skip_space();

    // Вспомогательные функции для того, чтобы узнать символ
    bool is_alpha(char c) const;
    bool is_number(char c) const;
    bool is_operation(char c) const;
    bool is_function(const std::string& name) const;

    // Функции для скана слагаемых в выражении.
    Token scan_identificator();
    Token scan_number();
    Token scan_operation();

    // Список всех поддерживаемых функций
    static const std::vector<std::string> functions;
public:
    // Основной конструктор для получения выражения
    Lexer(const std::string& expr);

    // Методы для получения токенов
    Token next_token();
    Token peek_token();  // Не использован и не нужен в принципе

    // Проверка на наличие следующего токена.
    bool has_next() const;

    // Получение позиции.
    size_t get_pos() const {return position;}
};