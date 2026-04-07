#include "ast.hpp"
#include "visitor.hpp"
#include <sstream>
#include <algorithm>

// NumberNode
NumberNode::NumberNode(double val) : value(val) {}

std::string NumberNode::to_string() const {
    return std::to_string(value);
}

std::unique_ptr<ASTNode> NumberNode::clone() const {
    return std::make_unique<NumberNode>(value);
}

void NumberNode::accept(Visitor &visitor) {
    visitor.visit(*this);
}


// VariableNode
VariableNode::VariableNode(std::string nm): name(nm) {}

std::string VariableNode::to_string() const {
    return name;
}

std::unique_ptr<ASTNode> VariableNode::clone() const {
    return std::make_unique<VariableNode>(name);
}

void VariableNode::accept(Visitor &visitor) {
    visitor.visit(*this);
}


// FunctionNode
FunctionNode::FunctionNode(Function func, std::unique_ptr<ASTNode> arg) : function(func), argument(std::move(arg)) {}

std::string func_to_string(Function f) {
    switch (f) {
        case Function::SIN: return "sin";
        case Function::COS: return "cos";
        case Function::TAN: return "tan";
        case Function::ASIN: return "asin";
        case Function::ACOS: return "acos";
        case Function::ATAN: return "atan";
        case Function::LOG: return "log";
        case Function::EXP: return "exp";
        case Function::SQRT: return "sqrt";
    }
    return "schlyapa";
}

std::string FunctionNode::to_string() const {
    return func_to_string(function) + "(" + argument->to_string() + ")";
}

std::unique_ptr<ASTNode> FunctionNode::clone() const {
    return std::make_unique<FunctionNode>(function, argument->clone());
}

void FunctionNode::accept(Visitor &visitor) {
    visitor.visit(*this);
}


// BinaryOpNode
BinaryOpNode::BinaryOpNode(BinaryOp o, std::unique_ptr<ASTNode> l, std::unique_ptr<ASTNode> r) : operation(o), left(std::move(l)), right(std::move(r)) {}

std::string BinaryOpNode::to_string() const {
    std::string op_str;

    switch (operation) {
        case BinaryOp::ADD: op_str = "+"; break;
        case BinaryOp::SUB: op_str = "-"; break;
        case BinaryOp::MUL: op_str = "*"; break;
        case BinaryOp::DIV: op_str = "/"; break;
        case BinaryOp::POW: op_str = "^"; break;
    }
    return "(" + left->to_string() + op_str + right->to_string() + ")";
}

std::unique_ptr<ASTNode> BinaryOpNode::clone() const {
    return std::make_unique<BinaryOpNode>(operation, left->clone(), right->clone());
}

void BinaryOpNode::accept(Visitor &visitor) {
    visitor.visit(*this);
}

// UnaryOpNode
UnaryOpNode::UnaryOpNode(UnaryOp o, std::unique_ptr<ASTNode> oprnd) : operation(o), operand(std::move(oprnd)) {}

std::string UnaryOpNode::to_string() const {
    std::string op_str;

    switch (operation) {
        case UnaryOp::PLUS: op_str = "+"; break;
        case UnaryOp::MINUS: op_str = "-"; break;
    }
    return "(" + op_str + operand->to_string() + ")";
}

std::unique_ptr<ASTNode> UnaryOpNode::clone() const {
    return std::make_unique<UnaryOpNode>(operation, operand->clone());
}

void UnaryOpNode::accept(Visitor &visitor) {
    visitor.visit(*this);
}