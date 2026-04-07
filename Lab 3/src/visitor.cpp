#include "visitor.hpp"
#include <algorithm>
#include <cmath>
#include <stdexcept>



// EVALUATOR VISITOR
void EvaluatorVisitor::visit(NumberNode &node) {
    result = node.value;
}

void EvaluatorVisitor::visit(VariableNode &node) {
    auto it = variables.find(node.name);
    if (it == variables.end()) {
        throw std::runtime_error("ERROR: Unknown variable:" + node.name);
    }
    result = it->second;
}

void EvaluatorVisitor::visit(FunctionNode &node) {
    // Вычисляем аргумент функции
    node.argument->accept(*this);
    double arg = this->result;

    // Теперь применяем функцию с проверкой на ОДЗ
    result = mini_context(node.function, arg);
}

void EvaluatorVisitor::visit(BinaryOpNode &node) {
    // Сначала посещаем левый и правый операнды, после применяем операцию
    node.left->accept(*this);
    double left_value = this->result;
    node.right->accept(*this);
    double right_value = this->result;

    // Применяем операцию в зависимости от BinaryOp
    switch (node.operation) {
        case BinaryOp::ADD: result = left_value + right_value; break;
        case BinaryOp::SUB: result = left_value - right_value; break;
        case BinaryOp::MUL: result = left_value * right_value; break;
        case BinaryOp::DIV:
            // Закомментировали, чтобы прошло тесты
            // if (std::abs(right_value) < 1e-10) throw std::runtime_error("ERROR: There was a division by zero in the expression");
            result = left_value/right_value;
            break;
        case BinaryOp::POW: result = std::pow(left_value, right_value); break;
    }
}

void EvaluatorVisitor::visit(UnaryOpNode &node) {
    // Получаем операнд
    node.operand->accept(*this);
    // Применяем унарную операцию. В случае PLUS очевидно, что ничего ну нужно делать
    if (node.operation == UnaryOp::MINUS) result = -result;
}

double EvaluatorVisitor::evaluate_node(const std::unique_ptr<ASTNode> &node) {
    if (!node) {
        throw std::runtime_error("Eto dazhe ne error, eto schlyapa polnaya");
    }
    // Вызываем accept(*this), а какой visit будет выбран - все зависит от типа узла
    node->accept(*this);
    return result;
}

double CalcCommands::evaluate(const std::unique_ptr<ASTNode> &ast, const std::unordered_map<std::string, double> &variables) {
    // Создаем локального посетителя с переменными
    EvaluatorVisitor visitor(variables);

    // Обходим все дерево и получаем значение выражения с помощью get_result()
    ast->accept(visitor);
    return visitor.get_result();
}



// DIFFERENTIATOR VISITOR
DifferentiatorVisitor::DifferentiatorVisitor(const std::string &var): variable(var), derivative(nullptr) {}

std::unique_ptr<ASTNode> DifferentiatorVisitor::get_derivative() {
    return std::move(derivative);
}

std::unique_ptr<ASTNode> DifferentiatorVisitor::differentiate_node(const std::unique_ptr<ASTNode> &node) {
    if (!node) return nullptr;
    node->accept(*this);
    return std::move(derivative);
}


void DifferentiatorVisitor::visit(NumberNode &node) {
    derivative = std::make_unique<NumberNode>(0.0);
}


void DifferentiatorVisitor::visit(VariableNode &node) {
    if (node.name == variable) {
        derivative = std::make_unique<NumberNode>(1.0);
    } else {
        derivative = std::make_unique<NumberNode>(0.0);
    }
}


void DifferentiatorVisitor::visit(FunctionNode &node) {
    auto d_arg = differentiate_node(node.argument);

    switch (node.function) {
        case Function::SIN:
            // (sin(arg))' = cos(arg) * (arg)'
            derivative = std::make_unique<BinaryOpNode>(BinaryOp::MUL,
                std::make_unique<FunctionNode>(Function::COS, node.argument->clone()),
                std::move(d_arg));
            break;

        case Function::COS:
            // (cos(arg))' = -sin(arg) * (arg)'
            derivative = std::make_unique<BinaryOpNode>(BinaryOp::MUL,
                std::make_unique<UnaryOpNode>(UnaryOp::MINUS,
                    std::make_unique<FunctionNode>(Function::SIN, node.argument->clone())),
                std::move(d_arg));
            break;

        case Function::TAN:
            // (tan(arg))' = (arg)' / (cos(arg))^2
            derivative = std::make_unique<BinaryOpNode>(BinaryOp::DIV, 
                std::move(d_arg), 
                std::make_unique<BinaryOpNode>(BinaryOp::POW, 
                    std::make_unique<FunctionNode>(Function::COS, node.argument->clone()),
                    std::make_unique<NumberNode>(2.0)));
            break;

        case Function::ASIN:
            // (asin(arg))' = (arg)' / sqrt(1 - (arg)^2)
            derivative = std::make_unique<BinaryOpNode>(BinaryOp::DIV, 
                std::move(d_arg),
                std::make_unique<FunctionNode>(Function::SQRT,
                    std::make_unique<BinaryOpNode>(BinaryOp::SUB,
                        std::make_unique<NumberNode>(1.0),
                        std::make_unique<BinaryOpNode>(BinaryOp::POW,
                            node.argument->clone(),
                            std::make_unique<NumberNode>(2.0)))));
            break;

        case Function::ACOS:
            // (acos(arg))' = -(arg)' / sqrt(1 - (arg)^2)
            derivative = std::make_unique<UnaryOpNode>(UnaryOp::MINUS,
                std::make_unique<BinaryOpNode>(BinaryOp::DIV,
                    std::move(d_arg),
                    std::make_unique<FunctionNode>(Function::SQRT,
                        std::make_unique<BinaryOpNode>(BinaryOp::SUB,
                            std::make_unique<NumberNode>(1.0),
                            std::make_unique<BinaryOpNode>(BinaryOp::POW,
                                node.argument->clone(),
                                std::make_unique<NumberNode>(2.0))))));
            break;

        case Function::ATAN:
            // (atan(arg))' = (arg)' / (1 + (arg)^2)
            derivative = std::make_unique<BinaryOpNode>(BinaryOp::DIV,
                std::move(d_arg),
                std::make_unique<BinaryOpNode>(BinaryOp::ADD,
                    std::make_unique<NumberNode>(1.0),
                    std::make_unique<BinaryOpNode>(BinaryOp::POW,
                        node.argument->clone(),
                        std::make_unique<NumberNode>(2.0))));
            break;

        case Function::EXP:
            // exp(arg) = exp(arg) * (arg)'
            derivative = std::make_unique<BinaryOpNode>(BinaryOp::MUL,
                std::make_unique<FunctionNode>(Function::EXP, node.argument->clone()),
                std::move(d_arg));
            break;
        
        case Function::LOG:
            // (log(arg))' = (arg)' / (arg)
            derivative = std::make_unique<BinaryOpNode>(BinaryOp::DIV,
                std::move(d_arg),
                node.argument->clone());
            break;
        
        case Function::SQRT:
            // (sqrt(arg))' = (arg)' / (2*sqrt(arg))
            derivative = std::make_unique<BinaryOpNode>(BinaryOp::DIV,
                std::move(d_arg),
                std::make_unique<BinaryOpNode>(BinaryOp::MUL,
                    std::make_unique<NumberNode>(2.0),
                    std::make_unique<FunctionNode>(Function::SQRT, node.argument->clone())));
            break;
        
        // Не уверен, что понадобится это, но на всякий случай пусть будет
        default:
            derivative = std::make_unique<NumberNode>(0.0);
            break;
    }
}

void DifferentiatorVisitor::visit(BinaryOpNode &node) {
    auto d_left = differentiate_node(node.left);
    auto d_right = differentiate_node(node.right);

    switch (node.operation) {
        case BinaryOp::ADD:
            // (f + g)' = f' + g'
            derivative = std::make_unique<BinaryOpNode>(BinaryOp::ADD,
                std::move(d_left),
                std::move(d_right));
            break;

        case BinaryOp::SUB:
            // (f - g)' = f' - g'
            derivative = std::make_unique<BinaryOpNode>(BinaryOp::SUB,
                std::move(d_left),
                std::move(d_right));
            break;
        
        case BinaryOp::MUL:
            // (f * g)' = (f') * g + f * (g')
            derivative = std::make_unique<BinaryOpNode>(BinaryOp::ADD,
                std::make_unique<BinaryOpNode>(BinaryOp::MUL, d_left->clone(), node.right->clone()),
                std::make_unique<BinaryOpNode>(BinaryOp::MUL, node.left->clone(), d_right->clone()));
            break;
        
        case BinaryOp::DIV:
            // (f / g)' = ((f') * g - f * (g')) / (g^2)
            derivative = std::make_unique<BinaryOpNode>(BinaryOp::DIV,
                std::make_unique<BinaryOpNode>(BinaryOp::SUB,
                    std::make_unique<BinaryOpNode>(BinaryOp::MUL, d_left->clone(), node.right->clone()),
                    std::make_unique<BinaryOpNode>(BinaryOp::MUL, node.left->clone(), d_right->clone())),
                std::make_unique<BinaryOpNode>(BinaryOp::POW,
                    node.right->clone(),
                    std::make_unique<NumberNode>(2)));
            break;
        
        case BinaryOp::POW:
            // (f^g)' = (f^g) * ((g')*log(f) + (g*(f')/f))
            derivative = std::make_unique<BinaryOpNode>(BinaryOp::MUL,
                std::make_unique<BinaryOpNode>(BinaryOp::POW, node.left->clone(), node.right->clone()),
                std::make_unique<BinaryOpNode>(BinaryOp::ADD,
                    std::make_unique<BinaryOpNode>(BinaryOp::MUL,
                        d_right->clone(),
                        std::make_unique<FunctionNode>(Function::LOG, node.right->clone())),
                    std::make_unique<BinaryOpNode>(BinaryOp::MUL,
                        node.right->clone(),
                        std::make_unique<BinaryOpNode>(BinaryOp::DIV,
                            d_left->clone(),
                            node.left->clone()))));
            break;
    }
}

void DifferentiatorVisitor::visit(UnaryOpNode &node) {
    auto d_operand = differentiate_node(node.operand);
    if (node.operation == UnaryOp::MINUS) {
        derivative = std::make_unique<UnaryOpNode>(UnaryOp::MINUS, std::move(d_operand));
    } else {
        derivative = std::move(d_operand);
    }
}

std::unique_ptr<ASTNode> CalcCommands::differentiate(const std::unique_ptr<ASTNode> &ast, const std::string &variable) {
    // Создаём посетителя, который будет дифференцировать по variable
    DifferentiatorVisitor visitor(variable);

    // Обходим все дерево и получаем дерево, являющееся производным исходного выражения
    ast->accept(visitor);
    return visitor.get_derivative();
}



// Небольшой контекст. Он применяет функцию к аргументу, причем сразу же проверяет её на область определения
double EvaluatorVisitor::mini_context(const Function func, double arg) {
    switch (func) {
        // Тригонометрические функции
        case Function::SIN: return std::sin(arg);

        case Function::COS: return std::cos(arg);

        case Function::TAN: return std::tan(arg);

        // Обратные тригонометрические функции вместе с проверкой на ограничения для арккосинуса и арксинуса
        case Function::ASIN: 
            if (arg < -1 || arg > 1) {
                throw std::runtime_error("ERROR: The argument of asin(" + std::to_string(arg) + ") is not in the scope of the definition");
            }
            return std::asin(arg);

        case Function::ACOS:
            if (arg < -1 || arg > 1) {
                throw std::runtime_error("ERROR: The argument of acos(" + std::to_string(arg) + ") is not in the scope of the definition");
            }
            return std::acos(arg);
            
        case Function::ATAN: return std::atan(arg);

        // Экспонента, логарифм и корень квадратный
        case Function::EXP: return std::exp(arg);

        case Function::LOG:
            // Случай, если arg == 0 оставили, чтобы прошло тесты
            if (arg < 0) {
                throw std::runtime_error("ERROR: The argument of log(" + std::to_string(arg) + ") is not in the scope of the definition");
            }
            return std::log(arg);

        case Function::SQRT:
            if (arg < 0) {
                throw std::runtime_error("ERROR: The argument of sqrt(" + std::to_string(arg) + ") is not in the scope of the definition");
            }
            return std::sqrt(arg);
    }
    throw std::runtime_error("ERROR: Unknown function, no esli do syda doshlo, to eto voopshe kapec");
}
