#pragma once

#include "ast.hpp"
#include <memory>
#include <string>

// Абстрактный класс для реализации паттерна visitor
class Visitor {
public:
    virtual ~Visitor() = default;                   // Виртуальный деструктор
    virtual void visit(NumberNode& node) = 0;       // Посетить узел с числом
    virtual void visit(VariableNode& node) = 0;     // Посетить узел с переменной
    virtual void visit(FunctionNode& node) = 0;     // Посетить узел с функцией
    virtual void visit(BinaryOpNode& node) = 0;     // Посетить узел с бинарной операцией
    virtual void visit(UnaryOpNode& node) = 0;      // Посетить узел с унарной операцией
};



// EVALUATOR VISITOR
class EvaluatorVisitor: public Visitor {
private:
    // Хэш-таблица: переменная, значение переменной
    std::unordered_map<std::string, double> variables;

    // Результат вычисления определенного узла (текущего)
    double result = 0;

    // Рекурсивная функция для вычисления
    double evaluate_node(const std::unique_ptr<ASTNode>& node);

    // Небольшой контекст. Он применяет функцию к аргументу, причем сразу же проверяет её на область определения
    static double mini_context(const Function func, double arg);

public:
    // Основной конструктор для инициализации таблицы переменных с значениями
    explicit EvaluatorVisitor(const std::unordered_map<std::string, double>& vars): variables(vars) {};

    // Функция для получения значения результата
    double get_result() const {return result;};

    // Атрибуты наследуемого класса
    void visit(NumberNode& node) override;
    void visit(VariableNode& node) override;
    void visit(FunctionNode& node) override;
    void visit(BinaryOpNode& node) override;
    void visit(UnaryOpNode& node) override;
};



// DIFFERENTIATOR VISITOR
class DifferentiatorVisitor: public Visitor {
private:
    // Имя переменной, по которой берется производная
    std::string variable;

    // Указатель на узел результата - дерево производной
    std::unique_ptr<ASTNode> derivative;

    // Рекурсивная функция для дифференцирования узла
    std::unique_ptr<ASTNode> differentiate_node(const std::unique_ptr<ASTNode>& node);

public:
    // Конструктор для инициализации переменной variable (смотри выше)
    explicit DifferentiatorVisitor(const std::string& var);

    // Функция для получения значения результата дифференцирования
    std::unique_ptr<ASTNode> get_derivative();

    // Атрибуты наследуемого класса
    // Производная от числа
    void visit(NumberNode& node) override;

    // Производная от переменной
    void visit(VariableNode& node) override;

    // Производная от функции
    void visit(FunctionNode& node) override;

    // Производная бинарной операции, то есть производная от суммы, разности, умножения, деления или возведения в степень
    void visit(BinaryOpNode& node) override;

    // Производная от унарной операции
    void visit(UnaryOpNode& node) override;
};



// Основные команды
namespace CalcCommands {
    // Функция для вычисления значения всего выражения
    double evaluate(const std::unique_ptr<ASTNode>& ast, const std::unordered_map<std::string, double>& variables);

    // Функция для нахождения производной
    std::unique_ptr<ASTNode> differentiate(const std::unique_ptr<ASTNode>& ast, const std::string& variable);
}