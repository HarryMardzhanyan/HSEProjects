#pragma once

#include <memory>
#include <vector>
#include <cmath>
#include <string>
#include <unordered_map>

// Набор типов узлов AST
enum class NodeType {
    NUMBER,
    VARIABLE,
    FUNCTION,
    BINARY_OP,
    UNARY_OP
};

// Набор бинарных операций
enum class BinaryOp {
    ADD, SUB, MUL, DIV, POW
};

// Набор унарных операций
enum class UnaryOp {
    PLUS, MINUS
};

// Набор поддерживаемых функций
enum class Function {
    SIN, COS, TAN, ASIN, ACOS, ATAN,
    LOG, EXP, SQRT
};

// Forward declaration класса Visitor
class Visitor;

// Абстрактная структура для всех узлов для дерева AST
struct ASTNode {
    virtual ~ASTNode() = default;                            // Виртуальный деструктор
    virtual std::string to_string() const = 0;               // Функция для преобразования в строку
    virtual std::unique_ptr<ASTNode> clone() const = 0;      // Функция для создания копии
    virtual void accept(Visitor& visitor) = 0;               // Функция для принятия посетителя, то есть чтобы посетить узел
};


// Узел для числа
struct NumberNode : public ASTNode {
    double value;                                            // Значение числа

    explicit NumberNode(double val);                         // Конструктор от числа

    // Атрибуты наследуемой структуры
    std::string to_string() const override;
    std::unique_ptr<ASTNode> clone() const override;
    void accept(Visitor& visitor) override;
};


// Узел для переменной
struct VariableNode : public ASTNode {
    std::string name;                                         // Имя переменной

    explicit VariableNode(std::string nm);                   // Конструктор от переменной

    // Атрибуты наследуемой структуры
    std::string to_string() const override;
    std::unique_ptr<ASTNode> clone() const override;
    void accept(Visitor& visitor) override;
};


// Узел для функции
struct FunctionNode : public ASTNode {
    Function function;                                              // Функция
    std::unique_ptr<ASTNode> argument;                              // Аргумент функции

    FunctionNode(Function func, std::unique_ptr<ASTNode> arg);      // Конструктор функции и его аргумента

    // Атрибуты наследуемой структуры
    std::string to_string() const override;
    std::unique_ptr<ASTNode> clone() const override;
    void accept(Visitor& visitor) override;
};
// Функция, чтобы преобразовать математическую функцию в строку
std::string func_to_string(Function f);


// Узел для бинарной операции
struct BinaryOpNode : public ASTNode {
    BinaryOp operation;                                        // Бинарная операция
    std::unique_ptr<ASTNode> left;                             // Левый операнд операции
    std::unique_ptr<ASTNode> right;                            // Правый операнд операции

    BinaryOpNode(BinaryOp o, std::unique_ptr<ASTNode> l, std::unique_ptr<ASTNode> r);      // Конструктор операции (буква o) и его операндов (левого и правого соответственно)

    // Атрибуты наследуемой структуры
    std::string to_string() const override;
    std::unique_ptr<ASTNode> clone() const override;
    void accept(Visitor& visitor) override;
};


// Узел для унарной операции
struct UnaryOpNode : public ASTNode {
    UnaryOp operation;                                         // Унарная операция
    std::unique_ptr<ASTNode> operand;                          // Операнд унарной операции

    UnaryOpNode(UnaryOp o, std::unique_ptr<ASTNode> oprnd);
    
    // Атрибуты наследуемой структуры
    std::string to_string() const override;
    std::unique_ptr<ASTNode> clone() const override;
    void accept(Visitor& visitor) override;
};