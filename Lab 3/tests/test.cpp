#define CATCH_CONFIG_MAIN

#include "catch.hpp"
#include <memory>
#include <unordered_map>
#include "lexer.hpp"
#include "parser.hpp"
#include "ast.hpp"
#include "visitor.hpp"

TEST_CASE("Lexer: basic tokens 1", "[lexer]") {
    Lexer lex("x + 3.14");
    
    auto token = lex.next_token();
    REQUIRE(token == "x");
    REQUIRE(token.type == lexem_t::VARIABLE);
    
    token = lex.next_token();
    REQUIRE(token == "+");
    REQUIRE(token.type == lexem_t::OPERATION);
    
    token = lex.next_token();
    REQUIRE(token == "3.14");
    REQUIRE(token.type == lexem_t::NUMBER);
}

TEST_CASE("Lexer: basic tokens 2", "[lexer]") {
    Lexer lex("+x");
    
    auto token = lex.next_token();
    REQUIRE(token == "+");
    REQUIRE(token.type == lexem_t::OPERATION);
    
    token = lex.next_token();
    REQUIRE(token == "x");
    REQUIRE(token.type == lexem_t::VARIABLE);
}

TEST_CASE("Lexer: basic tokens 3", "[lexer]") {
    Lexer lex("a//b");
    
    auto token = lex.next_token();
    REQUIRE(token == "a");
    REQUIRE(token.type == lexem_t::VARIABLE);
    
    token = lex.next_token();
    REQUIRE(token == "/");
    REQUIRE(token.type == lexem_t::OPERATION);

    token = lex.next_token();
    REQUIRE(token == "/");
    REQUIRE(token.type == lexem_t::OPERATION);

    token = lex.next_token();
    REQUIRE(token == "b");
    REQUIRE(token.type == lexem_t::VARIABLE);
}

TEST_CASE("Lexer: basic tokens 4", "[lexer]") {
    Lexer lex("1.0");
    
    auto token = lex.next_token();
    REQUIRE(token == "1.0");
    REQUIRE(token.type == lexem_t::NUMBER);
}

TEST_CASE("Lexer: basic tokens 5", "[lexer]") {
    Lexer lex("1 2");
    
    auto token = lex.next_token();
    REQUIRE(token == "1");
    REQUIRE(token.type == lexem_t::NUMBER);
    
    token = lex.next_token();
    REQUIRE(token == "2");
    REQUIRE(token.type == lexem_t::NUMBER);
}

TEST_CASE("Lexer: basic tokens 6", "[lexer]") {
    Lexer lex(")(");
    
    auto token = lex.next_token();
    REQUIRE(token == ")");
    REQUIRE(token.type == lexem_t::RPAREN);
    
    token = lex.next_token();
    REQUIRE(token == "(");
    REQUIRE(token.type == lexem_t::LPAREN);
}

TEST_CASE("Lexer: basic tokens 7", "[lexer]") {
    Lexer lex("a @ b");
    
    auto token = lex.next_token();
    REQUIRE(token == "a");
    REQUIRE(token.type == lexem_t::VARIABLE);
    
    REQUIRE_THROWS(lex.next_token());
}

TEST_CASE("Lexer: basic tokens 8", "[lexer]") {
    Lexer lex("a1");
    
    auto token = lex.next_token();
    REQUIRE(token == "a1");
    REQUIRE(token.type == lexem_t::VARIABLE);
}

TEST_CASE("Lexer: basic tokens 9", "[lexer]") {
    Lexer lex("12345678901234567890123456789012345678901234567890.12345678901234567890");
    
    auto token = lex.next_token();
    REQUIRE(token == "12345678901234567890123456789012345678901234567890.12345678901234567890");
    REQUIRE(token.type == lexem_t::NUMBER);
}

TEST_CASE("Lexer: basic tokens 10", "[lexer]") {
    Lexer lex("        a   +     b   ");
    
    auto token = lex.next_token();
    REQUIRE(token == "a");
    REQUIRE(token.type == lexem_t::VARIABLE);

    token = lex.next_token();
    REQUIRE(token == "+");
    REQUIRE(token.type == lexem_t::OPERATION);

    token = lex.next_token();
    REQUIRE(token == "b");
    REQUIRE(token.type == lexem_t::VARIABLE);
}

TEST_CASE("Lexer: basic tokens 11", "[lexer]") {
    Lexer lex("(a + b)");
    
    auto token = lex.next_token();
    REQUIRE(token == "(");
    REQUIRE(token.type == lexem_t::LPAREN);

    token = lex.next_token();
    REQUIRE(token == "a");
    REQUIRE(token.type == lexem_t::VARIABLE);

    token = lex.next_token();
    REQUIRE(token == "+");
    REQUIRE(token.type == lexem_t::OPERATION);

    token = lex.next_token();
    REQUIRE(token == "b");
    REQUIRE(token.type == lexem_t::VARIABLE);

    token = lex.next_token();
    REQUIRE(token == ")");
    REQUIRE(token.type == lexem_t::RPAREN);
}

TEST_CASE("Lexer: basic tokens 12", "[lexer]") {
    Lexer lex("(a ++ b)");
    
    auto token = lex.next_token();
    REQUIRE(token == "(");
    REQUIRE(token.type == lexem_t::LPAREN);

    token = lex.next_token();
    REQUIRE(token == "a");
    REQUIRE(token.type == lexem_t::VARIABLE);

    token = lex.next_token();
    REQUIRE(token == "+");
    REQUIRE(token.type == lexem_t::OPERATION);
    token = lex.next_token();
    REQUIRE(token == "+");
    REQUIRE(token.type == lexem_t::OPERATION);

    token = lex.next_token();
    REQUIRE(token == "b");
    REQUIRE(token.type == lexem_t::VARIABLE);

    token = lex.next_token();
    REQUIRE(token == ")");
    REQUIRE(token.type == lexem_t::RPAREN);
}

TEST_CASE("Lexer: basic tokens 13", "[lexer]") {
    Lexer lex("a - b");
    
    auto token = lex.next_token();
    REQUIRE(token == "a");
    REQUIRE(token.type == lexem_t::VARIABLE);

    token = lex.next_token();
    REQUIRE(token == "-");
    REQUIRE(token.type == lexem_t::OPERATION);

    token = lex.next_token();
    REQUIRE(token == "b");
    REQUIRE(token.type == lexem_t::VARIABLE);
}

TEST_CASE("Lexer: basic tokens 14", "[lexer]") {
    Lexer lex("a * b");
    
    auto token = lex.next_token();
    REQUIRE(token == "a");
    REQUIRE(token.type == lexem_t::VARIABLE);

    token = lex.next_token();
    REQUIRE(token == "*");
    REQUIRE(token.type == lexem_t::OPERATION);

    token = lex.next_token();
    REQUIRE(token == "b");
    REQUIRE(token.type == lexem_t::VARIABLE);
}

TEST_CASE("Lexer: basic tokens 15", "[lexer]") {
    Lexer lex("a / b");
    
    auto token = lex.next_token();
    REQUIRE(token == "a");
    REQUIRE(token.type == lexem_t::VARIABLE);

    token = lex.next_token();
    REQUIRE(token == "/");
    REQUIRE(token.type == lexem_t::OPERATION);

    token = lex.next_token();
    REQUIRE(token == "b");
    REQUIRE(token.type == lexem_t::VARIABLE);
}

TEST_CASE("Lexer: basic tokens 16", "[lexer]") {
    Lexer lex("a ^ b");
    
    auto token = lex.next_token();
    REQUIRE(token == "a");
    REQUIRE(token.type == lexem_t::VARIABLE);

    token = lex.next_token();
    REQUIRE(token == "^");
    REQUIRE(token.type == lexem_t::OPERATION);

    token = lex.next_token();
    REQUIRE(token == "b");
    REQUIRE(token.type == lexem_t::VARIABLE);
}

TEST_CASE("Lexer: basic tokens 17", "[lexer]") {
    Lexer lex("+a");

    auto token = lex.next_token();
    REQUIRE(token == "+");
    REQUIRE(token.type == lexem_t::OPERATION);

    token = lex.next_token();
    REQUIRE(token == "a");
    REQUIRE(token.type == lexem_t::VARIABLE);
}

TEST_CASE("Lexer: basic tokens 18", "[lexer]") {
    Lexer lex("-a");

    auto token = lex.next_token();
    REQUIRE(token == "-");
    REQUIRE(token.type == lexem_t::OPERATION);

    token = lex.next_token();
    REQUIRE(token == "a");
    REQUIRE(token.type == lexem_t::VARIABLE);
}

TEST_CASE("Lexer: basic tokens 19", "[lexer]") {
    Lexer lex("++a");
    
    auto token = lex.next_token();
    REQUIRE(token == "+");
    REQUIRE(token.type == lexem_t::OPERATION);

    token = lex.next_token();
    REQUIRE(token == "+");
    REQUIRE(token.type == lexem_t::OPERATION);

    token = lex.next_token();
    REQUIRE(token == "a");
    REQUIRE(token.type == lexem_t::VARIABLE);
}

TEST_CASE("Lexer: basic tokens 20", "[lexer]") {
    Lexer lex("--a");
    
    auto token = lex.next_token();
    REQUIRE(token == "-");
    REQUIRE(token.type == lexem_t::OPERATION);

    token = lex.next_token();
    REQUIRE(token == "-");
    REQUIRE(token.type == lexem_t::OPERATION);

    token = lex.next_token();
    REQUIRE(token == "a");
    REQUIRE(token.type == lexem_t::VARIABLE);
}

TEST_CASE("Lexer: basic tokens 21", "[lexer]") {
    Lexer lex("---+++lev");
    
    auto token = lex.next_token();
    REQUIRE(token == "-");
    REQUIRE(token.type == lexem_t::OPERATION);
    token = lex.next_token();
    REQUIRE(token == "-");
    REQUIRE(token.type == lexem_t::OPERATION);
    token = lex.next_token();
    REQUIRE(token == "-");
    REQUIRE(token.type == lexem_t::OPERATION);

    token = lex.next_token();
    REQUIRE(token == "+");
    REQUIRE(token.type == lexem_t::OPERATION);
    token = lex.next_token();
    REQUIRE(token == "+");
    REQUIRE(token.type == lexem_t::OPERATION);
    token = lex.next_token();
    REQUIRE(token == "+");
    REQUIRE(token.type == lexem_t::OPERATION);

    token = lex.next_token();
    REQUIRE(token == "lev");
    REQUIRE(token.type == lexem_t::VARIABLE);
}

TEST_CASE("Lexer: basic tokens 22", "[lexer]") {
    Lexer lex("lev ()");

    auto token = lex.next_token();
    REQUIRE(token == "lev");
    REQUIRE(token.type == lexem_t::VARIABLE);
    
    token = lex.next_token();
    REQUIRE(token == "(");
    REQUIRE(token.type == lexem_t::LPAREN);

    token = lex.next_token();
    REQUIRE(token == ")");
    REQUIRE(token.type == lexem_t::RPAREN);
}

TEST_CASE("Lexer: basic tokens 23", "[lexer]") {
    Lexer lex("-(a+b)");

    auto token = lex.next_token();
    REQUIRE(token == "-");
    REQUIRE(token.type == lexem_t::OPERATION);
    token = lex.next_token();
    REQUIRE(token == "(");
    REQUIRE(token.type == lexem_t::LPAREN);

    token = lex.next_token();
    REQUIRE(token == "a");
    REQUIRE(token.type == lexem_t::VARIABLE);
    token = lex.next_token();
    REQUIRE(token == "+");
    REQUIRE(token.type == lexem_t::OPERATION);
    token = lex.next_token();
    REQUIRE(token == "b");
    REQUIRE(token.type == lexem_t::VARIABLE);

    token = lex.next_token();
    REQUIRE(token == ")");
    REQUIRE(token.type == lexem_t::RPAREN);
}

TEST_CASE("Lexer: basic tokens 24", "[lexer]") {
    Lexer lex("a*-b");
    
    auto token = lex.next_token();
    REQUIRE(token == "a");
    REQUIRE(token.type == lexem_t::VARIABLE);

    token = lex.next_token();
    REQUIRE(token == "*");
    REQUIRE(token.type == lexem_t::OPERATION);
    token = lex.next_token();
    REQUIRE(token == "-");
    REQUIRE(token.type == lexem_t::OPERATION);

    token = lex.next_token();
    REQUIRE(token == "b");
    REQUIRE(token.type == lexem_t::VARIABLE);
}

TEST_CASE("Lexer: basic tokens 25", "[lexer]") {
    Lexer lex("a^-2");
    
    auto token = lex.next_token();
    REQUIRE(token == "a");
    REQUIRE(token.type == lexem_t::VARIABLE);

    token = lex.next_token();
    REQUIRE(token == "^");
    REQUIRE(token.type == lexem_t::OPERATION);
    token = lex.next_token();
    REQUIRE(token == "-");
    REQUIRE(token.type == lexem_t::OPERATION);

    token = lex.next_token();
    REQUIRE(token == "2");
    REQUIRE(token.type == lexem_t::NUMBER);
}

TEST_CASE("Lexer: basic tokens 26", "[lexer]") {
    Lexer lex("a*-2s3");
    
    auto token = lex.next_token();
    REQUIRE(token == "a");
    REQUIRE(token.type == lexem_t::VARIABLE);

    token = lex.next_token();
    REQUIRE(token == "*");
    REQUIRE(token.type == lexem_t::OPERATION);
    token = lex.next_token();
    REQUIRE(token == "-");
    REQUIRE(token.type == lexem_t::OPERATION);

    REQUIRE_THROWS(lex.next_token());
}

TEST_CASE("Lexer: basic tokens 27", "[lexer]") {
    Lexer lex("a+-b");
    
    auto token = lex.next_token();
    REQUIRE(token == "a");
    REQUIRE(token.type == lexem_t::VARIABLE);

    token = lex.next_token();
    REQUIRE(token == "+");
    REQUIRE(token.type == lexem_t::OPERATION);
    token = lex.next_token();
    REQUIRE(token == "-");
    REQUIRE(token.type == lexem_t::OPERATION);

    token = lex.next_token();
    REQUIRE(token == "b");
    REQUIRE(token.type == lexem_t::VARIABLE);
}

TEST_CASE("Lexer: basic tokens 28", "[lexer]") {
    Lexer lex("a--b");
    
    auto token = lex.next_token();
    REQUIRE(token == "a");
    REQUIRE(token.type == lexem_t::VARIABLE);

    token = lex.next_token();
    REQUIRE(token == "-");
    REQUIRE(token.type == lexem_t::OPERATION);
    token = lex.next_token();
    REQUIRE(token == "-");
    REQUIRE(token.type == lexem_t::OPERATION);

    token = lex.next_token();
    REQUIRE(token == "b");
    REQUIRE(token.type == lexem_t::VARIABLE);
}

TEST_CASE("Lexer: basic tokens 29", "[lexer]") {
    Lexer lex("a**b");
    
    auto token = lex.next_token();
    REQUIRE(token == "a");
    REQUIRE(token.type == lexem_t::VARIABLE);

    token = lex.next_token();
    REQUIRE(token == "*");
    REQUIRE(token.type == lexem_t::OPERATION);
    token = lex.next_token();
    REQUIRE(token == "*");
    REQUIRE(token.type == lexem_t::OPERATION);

    token = lex.next_token();
    REQUIRE(token == "b");
    REQUIRE(token.type == lexem_t::VARIABLE);
}

TEST_CASE("Lexer: basic tokens 30", "[lexer]") {
    Lexer lex("a^^b");
    
    auto token = lex.next_token();
    REQUIRE(token == "a");
    REQUIRE(token.type == lexem_t::VARIABLE);

    token = lex.next_token();
    REQUIRE(token == "^");
    REQUIRE(token.type == lexem_t::OPERATION);
    token = lex.next_token();
    REQUIRE(token == "^");
    REQUIRE(token.type == lexem_t::OPERATION);

    token = lex.next_token();
    REQUIRE(token == "b");
    REQUIRE(token.type == lexem_t::VARIABLE);
}

TEST_CASE("Lexer: basic tokens 31", "[lexer]") {
    Lexer lex("a+++b");
    
    auto token = lex.next_token();
    REQUIRE(token == "a");
    REQUIRE(token.type == lexem_t::VARIABLE);

    token = lex.next_token();
    REQUIRE(token == "+");
    REQUIRE(token.type == lexem_t::OPERATION);
    token = lex.next_token();
    REQUIRE(token == "+");
    REQUIRE(token.type == lexem_t::OPERATION);
    token = lex.next_token();
    REQUIRE(token == "+");
    REQUIRE(token.type == lexem_t::OPERATION);

    token = lex.next_token();
    REQUIRE(token == "b");
    REQUIRE(token.type == lexem_t::VARIABLE);
}

TEST_CASE("Lexer: basic tokens 32", "[lexer]") {
    Lexer lex("2.7 - foo(x) * (5 + 2.654)");
    
    auto token = lex.next_token();
    REQUIRE(token == "2.7");
    REQUIRE(token.type == lexem_t::NUMBER);

    token = lex.next_token();
    REQUIRE(token == "-");
    REQUIRE(token.type == lexem_t::OPERATION);

    token = lex.next_token();
    REQUIRE(token == "foo");

    token = lex.next_token();
    REQUIRE(token == "(");
    REQUIRE(token.type == lexem_t::LPAREN);
    token = lex.next_token();
    REQUIRE(token == "x");
    REQUIRE(token.type == lexem_t::VARIABLE);
    token = lex.next_token();
    REQUIRE(token == ")");
    REQUIRE(token.type == lexem_t::RPAREN);

    token = lex.next_token();
    REQUIRE(token == "*");
    REQUIRE(token.type == lexem_t::OPERATION);

    token = lex.next_token();
    REQUIRE(token == "(");
    REQUIRE(token.type == lexem_t::LPAREN);
    token = lex.next_token();
    REQUIRE(token == "5");
    REQUIRE(token.type == lexem_t::NUMBER);
    token = lex.next_token();
    REQUIRE(token == "+");
    REQUIRE(token.type == lexem_t::OPERATION);
    token = lex.next_token();
    REQUIRE(token == "2.654");
    REQUIRE(token.type == lexem_t::NUMBER);
    token = lex.next_token();
    REQUIRE(token == ")");
    REQUIRE(token.type == lexem_t::RPAREN);
}

TEST_CASE("Lexer: basic tokens 33", "[lexer]") {
    Lexer lex("0.05500555^0.0054");
    
    auto token = lex.next_token();
    REQUIRE(token == "0.05500555");
    REQUIRE(token.type == lexem_t::NUMBER);

    token = lex.next_token();
    REQUIRE(token == "^");
    REQUIRE(token.type == lexem_t::OPERATION);

    token = lex.next_token();
    REQUIRE(token == "0.0054");
    REQUIRE(token.type == lexem_t::NUMBER);
}

TEST_CASE("Lexer: basic tokens 34", "[lexer]") {
    Lexer lex("_x^2 - _y^2 - _c^2");
    
    auto token = lex.next_token();
    REQUIRE(token == "_x");
    REQUIRE(token.type == lexem_t::VARIABLE);
    token = lex.next_token();
    REQUIRE(token == "^");
    REQUIRE(token.type == lexem_t::OPERATION);
    token = lex.next_token();
    REQUIRE(token == "2");
    REQUIRE(token.type == lexem_t::NUMBER);
    token = lex.next_token();
    REQUIRE(token == "-");
    REQUIRE(token.type == lexem_t::OPERATION);
    token = lex.next_token();
    REQUIRE(token == "_y");
    REQUIRE(token.type == lexem_t::VARIABLE);
    token = lex.next_token();
    REQUIRE(token == "^");
    REQUIRE(token.type == lexem_t::OPERATION);
    token = lex.next_token();
    REQUIRE(token == "2");
    REQUIRE(token.type == lexem_t::NUMBER);
    token = lex.next_token();
    REQUIRE(token == "-");
    REQUIRE(token.type == lexem_t::OPERATION);
    token = lex.next_token();
    REQUIRE(token == "_c");
    REQUIRE(token.type == lexem_t::VARIABLE);
    token = lex.next_token();
    REQUIRE(token == "^");
    REQUIRE(token.type == lexem_t::OPERATION);
    token = lex.next_token();
    REQUIRE(token == "2");
    REQUIRE(token.type == lexem_t::NUMBER);
}

TEST_CASE("Lexer: basic tokens 35", "[lexer]") {
    Lexer lex("aaa - bbb + ccc^(ddd - gfd)");
    
    auto token = lex.next_token();
    REQUIRE(token == "aaa");
    REQUIRE(token.type == lexem_t::VARIABLE);
    token = lex.next_token();
    REQUIRE(token == "-");
    REQUIRE(token.type == lexem_t::OPERATION);
    token = lex.next_token();
    REQUIRE(token == "bbb");
    REQUIRE(token.type == lexem_t::VARIABLE);
    token = lex.next_token();
    REQUIRE(token == "+");
    REQUIRE(token.type == lexem_t::OPERATION);
    token = lex.next_token();
    REQUIRE(token == "ccc");
    REQUIRE(token.type == lexem_t::VARIABLE);
    token = lex.next_token();
    REQUIRE(token == "^");
    REQUIRE(token.type == lexem_t::OPERATION);
    token = lex.next_token();
    REQUIRE(token == "(");
    REQUIRE(token.type == lexem_t::LPAREN);
    token = lex.next_token();
    REQUIRE(token == "ddd");
    REQUIRE(token.type == lexem_t::VARIABLE);
    token = lex.next_token();
    REQUIRE(token == "-");
    REQUIRE(token.type == lexem_t::OPERATION);
    token = lex.next_token();
    REQUIRE(token == "gfd");
    REQUIRE(token.type == lexem_t::VARIABLE);
    token = lex.next_token();
    REQUIRE(token == ")");
    REQUIRE(token.type == lexem_t::RPAREN);
}

TEST_CASE("Lexer: basic tokens 36", "[lexer]") {
    Lexer lex("0.0000005 - 0.0000005");
    
    auto token = lex.next_token();
    REQUIRE(token == "0.0000005");
    REQUIRE(token.type == lexem_t::NUMBER);
    token = lex.next_token();
    REQUIRE(token == "-");
    REQUIRE(token.type == lexem_t::OPERATION);
    token = lex.next_token();
    REQUIRE(token == "0.0000005");
    REQUIRE(token.type == lexem_t::NUMBER);
}

TEST_CASE("Lexer: basic tokens 37", "[lexer]") {
    Lexer lex("0^0");
    
    auto token = lex.next_token();
    REQUIRE(token == "0");
    REQUIRE(token.type == lexem_t::NUMBER);
    token = lex.next_token();
    REQUIRE(token == "^");
    REQUIRE(token.type == lexem_t::OPERATION);
    token = lex.next_token();
    REQUIRE(token == "0");
    REQUIRE(token.type == lexem_t::NUMBER);
}

TEST_CASE("Lexer: basic tokens 38", "[lexer]") {
    Lexer lex("(0.005)^(0.05)");
    
    auto token = lex.next_token();
    REQUIRE(token == "(");
    REQUIRE(token.type == lexem_t::LPAREN);
    token = lex.next_token();
    REQUIRE(token == "0.005");
    REQUIRE(token.type == lexem_t::NUMBER);
    token = lex.next_token();
    REQUIRE(token == ")");
    REQUIRE(token.type == lexem_t::RPAREN);
    token = lex.next_token();
    REQUIRE(token == "^");
    REQUIRE(token.type == lexem_t::OPERATION);
    token = lex.next_token();
    REQUIRE(token == "(");
    REQUIRE(token.type == lexem_t::LPAREN);
    token = lex.next_token();
    REQUIRE(token == "0.05");
    REQUIRE(token.type == lexem_t::NUMBER);
    token = lex.next_token();
    REQUIRE(token == ")");
    REQUIRE(token.type == lexem_t::RPAREN);
}

TEST_CASE("Lexer: basic tokens 39", "[lexer]") {
    Lexer lex("5.5 * x - y^(2 * x)");
    
    auto token = lex.next_token();
    REQUIRE(token == "5.5");
    REQUIRE(token.type == lexem_t::NUMBER);
    token = lex.next_token();
    REQUIRE(token == "*");
    REQUIRE(token.type == lexem_t::OPERATION);
    token = lex.next_token();
    REQUIRE(token == "x");
    REQUIRE(token.type == lexem_t::VARIABLE);
    token = lex.next_token();
    REQUIRE(token == "-");
    REQUIRE(token.type == lexem_t::OPERATION);
    token = lex.next_token();
    REQUIRE(token == "y");
    REQUIRE(token.type == lexem_t::VARIABLE);
    token = lex.next_token();
    REQUIRE(token == "^");
    REQUIRE(token.type == lexem_t::OPERATION);
    token = lex.next_token();
    REQUIRE(token == "(");
    REQUIRE(token.type == lexem_t::LPAREN);
    token = lex.next_token();
    REQUIRE(token == "2");
    REQUIRE(token.type == lexem_t::NUMBER);
    token = lex.next_token();
    REQUIRE(token == "*");
    REQUIRE(token.type == lexem_t::OPERATION);
    token = lex.next_token();
    REQUIRE(token == "x");
    REQUIRE(token.type == lexem_t::VARIABLE);
    token = lex.next_token();
    REQUIRE(token == ")");
    REQUIRE(token.type == lexem_t::RPAREN);
}

TEST_CASE("Lexer: basic tokens 40", "[lexer]") {
    Lexer lex(".2");
    
    REQUIRE_THROWS(lex.next_token());
}

TEST_CASE("Lexer: basic tokens 41", "[lexer]") {
    Lexer lex("2.");
    
    REQUIRE_THROWS(lex.next_token());
}

TEST_CASE("Lexer: basic tokens 42", "[lexer]") {
    Lexer lex(".");
    
    REQUIRE_THROWS(lex.next_token());
}

TEST_CASE("Lexer: basic tokens 43", "[lexer]") {
    Lexer lex("..");
    
    REQUIRE_THROWS(lex.next_token());
}

TEST_CASE("Lexer: basic tokens 44", "[lexer]") {
    Lexer lex("2..3");
    
    REQUIRE_THROWS(lex.next_token());
}

TEST_CASE("Lexer: basic tokens 45", "[lexer]") {
    Lexer lex(".0.1");
    
    REQUIRE_THROWS(lex.next_token());
}

TEST_CASE("Lexer: basic tokens 46", "[lexer]") {
    Lexer lex("2.3.4");
    
    REQUIRE_THROWS(lex.next_token());
}

TEST_CASE("Lexer: basic tokens 47", "[lexer]") {
    Lexer lex("0..1");
    
    REQUIRE_THROWS(lex.next_token());
}

TEST_CASE("Lexer: basic tokens 48", "[lexer]") {
    Lexer lex("2 2");
    
    auto token = lex.next_token();
    REQUIRE(token == "2");
    REQUIRE(token.type == lexem_t::NUMBER);
    token = lex.next_token();
    REQUIRE(token == "2");
    REQUIRE(token.type == lexem_t::NUMBER);
}

TEST_CASE("Lexer: basic tokens 49", "[lexer]") {
    Lexer lex("fdf fdf");

    auto token = lex.next_token();
    REQUIRE(token == "fdf");
    REQUIRE(token.type == lexem_t::VARIABLE);
    token = lex.next_token();
    REQUIRE(token == "fdf");
    REQUIRE(token.type == lexem_t::VARIABLE);
}

TEST_CASE("Lexer: basic tokens 50", "[lexer]") {
    Lexer lex("20a2 + 345.6");
    
    REQUIRE_THROWS(lex.next_token());
}

TEST_CASE("Lexer: basic tokens 51", "[lexer]") {
    Lexer lex("()");
    
    auto token = lex.next_token();
    REQUIRE(token == "(");
    REQUIRE(token.type == lexem_t::LPAREN);
    token = lex.next_token();
    REQUIRE(token == ")");
    REQUIRE(token.type == lexem_t::RPAREN);
}

TEST_CASE("Lexer: basic tokens 52", "[lexer]") {
    Lexer lex("(())");
    
    auto token = lex.next_token();
    REQUIRE(token == "(");
    REQUIRE(token.type == lexem_t::LPAREN);
    token = lex.next_token();
    REQUIRE(token == "(");
    REQUIRE(token.type == lexem_t::LPAREN);
    token = lex.next_token();
    REQUIRE(token == ")");
    REQUIRE(token.type == lexem_t::RPAREN);
    token = lex.next_token();
    REQUIRE(token == ")");
    REQUIRE(token.type == lexem_t::RPAREN);
}

TEST_CASE("Lexer: basic tokens 53", "[lexer]") {
    Lexer lex("((a))");
    
    auto token = lex.next_token();
    REQUIRE(token == "(");
    REQUIRE(token.type == lexem_t::LPAREN);
    token = lex.next_token();
    REQUIRE(token == "(");
    REQUIRE(token.type == lexem_t::LPAREN);
    token = lex.next_token();
    REQUIRE(token == "a");
    REQUIRE(token.type == lexem_t::VARIABLE);
    token = lex.next_token();
    REQUIRE(token == ")");
    REQUIRE(token.type == lexem_t::RPAREN);
    token = lex.next_token();
    REQUIRE(token == ")");
    REQUIRE(token.type == lexem_t::RPAREN);
}

TEST_CASE("Lexer: basic tokens 54", "[lexer]") {
    Lexer lex("a()");
    
    auto token = lex.next_token();
    REQUIRE(token == "a");
    REQUIRE(token.type == lexem_t::VARIABLE);
    token = lex.next_token();
    REQUIRE(token == "(");
    REQUIRE(token.type == lexem_t::LPAREN);
    token = lex.next_token();
    REQUIRE(token == ")");
    REQUIRE(token.type == lexem_t::RPAREN);
}

TEST_CASE("Lexer: basic tokens 55", "[lexer]") {
    Lexer lex("sin()");
    
    auto token = lex.next_token();
    REQUIRE(token == "sin");
    REQUIRE(token.type == lexem_t::FUNCTION);
    token = lex.next_token();
    REQUIRE(token == "(");
    REQUIRE(token.type == lexem_t::LPAREN);
    token = lex.next_token();
    REQUIRE(token == ")");
    REQUIRE(token.type == lexem_t::RPAREN);
}

TEST_CASE("Lexer: basic tokens 56", "[lexer]") {
    Lexer lex("(a)(b)");
    
    auto token = lex.next_token();
    REQUIRE(token == "(");
    REQUIRE(token.type == lexem_t::LPAREN);
    token = lex.next_token();
    REQUIRE(token == "a");
    REQUIRE(token.type == lexem_t::VARIABLE);
    token = lex.next_token();
    REQUIRE(token == ")");
    REQUIRE(token.type == lexem_t::RPAREN);
    token = lex.next_token();
    REQUIRE(token == "(");
    REQUIRE(token.type == lexem_t::LPAREN);
    token = lex.next_token();
    REQUIRE(token == "b");
    REQUIRE(token.type == lexem_t::VARIABLE);
    token = lex.next_token();
    REQUIRE(token == ")");
    REQUIRE(token.type == lexem_t::RPAREN);
}

TEST_CASE("Lexer: basic tokens 57", "[lexer]") {
    Lexer lex("))(");
    
    auto token = lex.next_token();
    REQUIRE(token == ")");
    REQUIRE(token.type == lexem_t::RPAREN);
    token = lex.next_token();
    REQUIRE(token == ")");
    REQUIRE(token.type == lexem_t::RPAREN);
    token = lex.next_token();
    REQUIRE(token == "(");
    REQUIRE(token.type == lexem_t::LPAREN);
}

TEST_CASE("Lexer: basic tokens 58", "[lexer]") {
    Lexer lex("))()");
    
    auto token = lex.next_token();
    REQUIRE(token == ")");
    REQUIRE(token.type == lexem_t::RPAREN);
    token = lex.next_token();
    REQUIRE(token == ")");
    REQUIRE(token.type == lexem_t::RPAREN);
    token = lex.next_token();
    REQUIRE(token == "(");
    REQUIRE(token.type == lexem_t::LPAREN);
    token = lex.next_token();
    REQUIRE(token == ")");
    REQUIRE(token.type == lexem_t::RPAREN);
}

TEST_CASE("Lexer: basic tokens 59", "[lexer]") {
    Lexer lex("(.4)");
    
    auto token = lex.next_token();
    REQUIRE(token == "(");
    REQUIRE(token.type == lexem_t::LPAREN);
    REQUIRE_THROWS(lex.next_token());
}

TEST_CASE("Lexer: basic tokens 60", "[lexer]") {
    Lexer lex(").3()");
    
    auto token = lex.next_token();
    REQUIRE(token == ")");
    REQUIRE(token.type == lexem_t::RPAREN);
    REQUIRE_THROWS(lex.next_token());
}


TEST_CASE("Lexer: basic tokens 61", "[lexer]") {
    Lexer lex(")2.r)");
    
    auto token = lex.next_token();
    REQUIRE(token == ")");
    REQUIRE(token.type == lexem_t::RPAREN);
    REQUIRE_THROWS(lex.next_token());
}

TEST_CASE("Lexer: basic tokens 62", "[lexer]") {
    Lexer lex("2.r+   6");
    
    REQUIRE_THROWS(lex.next_token());
}

TEST_CASE("Lexer: basic tokens 63", "[lexer]") {
    Lexer lex("2   +       5");
    
    auto token = lex.next_token();
    REQUIRE(token == "2");
    REQUIRE(token.type == lexem_t::NUMBER);
    token = lex.next_token();
    REQUIRE(token == "+");
    REQUIRE(token.type == lexem_t::OPERATION);
    token = lex.next_token();
    REQUIRE(token == "5");
    REQUIRE(token.type == lexem_t::NUMBER);
}

TEST_CASE("Lexer: basic tokens 64", "[lexer]") {
    Lexer lex("2 ^             xrxr_");
    
    auto token = lex.next_token();
    REQUIRE(token == "2");
    REQUIRE(token.type == lexem_t::NUMBER);
    token = lex.next_token();
    REQUIRE(token == "^");
    REQUIRE(token.type == lexem_t::OPERATION);
    token = lex.next_token();
    REQUIRE(token == "xrxr_");
    REQUIRE(token.type == lexem_t::VARIABLE);
}

TEST_CASE("Lexer: basic tokens 65", "[lexer]") {
    Lexer lex("x w e r");
    
    auto token = lex.next_token();
    REQUIRE(token == "x");
    REQUIRE(token.type == lexem_t::VARIABLE);
    token = lex.next_token();
    REQUIRE(token == "w");
    REQUIRE(token.type == lexem_t::VARIABLE);
    token = lex.next_token();
    REQUIRE(token == "e");
    REQUIRE(token.type == lexem_t::VARIABLE);
    token = lex.next_token();
    REQUIRE(token == "r");
    REQUIRE(token.type == lexem_t::VARIABLE);
}

TEST_CASE("Lexer: basic tokens 66", "[lexer]") {
    Lexer lex("a$b");
    
    auto token = lex.next_token();
    REQUIRE(token == "a");
    REQUIRE(token.type == lexem_t::VARIABLE);
    REQUIRE_THROWS(lex.next_token());
}

TEST_CASE("Lexer: basic tokens 67", "[lexer]") {
    Lexer lex("a;b");
    
    auto token = lex.next_token();
    REQUIRE(token == "a");
    REQUIRE(token.type == lexem_t::VARIABLE);
    REQUIRE_THROWS(lex.next_token());
}

TEST_CASE("Lexer: basic tokens 68", "[lexer]") {
    Lexer lex("a?b");
    
    auto token = lex.next_token();
    REQUIRE(token == "a");
    REQUIRE(token.type == lexem_t::VARIABLE);
    REQUIRE_THROWS(lex.next_token());
}

TEST_CASE("Lexer: basic tokens 69", "[lexer]") {
    Lexer lex("a#b");
    
    auto token = lex.next_token();
    REQUIRE(token == "a");
    REQUIRE(token.type == lexem_t::VARIABLE);
    REQUIRE_THROWS(lex.next_token());
}

TEST_CASE("Lexer: basic tokens 70", "[lexer]") {
    Lexer lex("e10");
    
    auto token = lex.next_token();
    REQUIRE(token == "e10");
    REQUIRE(token.type == lexem_t::VARIABLE);
}

TEST_CASE("Lexer: basic tokens 71", "[lexer]") {
    Lexer lex("1e");
    
    REQUIRE_THROWS(lex.next_token());
}

TEST_CASE("Lexer: basic tokens 72", "[lexer]") {
    Lexer lex("1e+");
    
    REQUIRE_THROWS(lex.next_token());
}

TEST_CASE("Lexer: basic tokens 73", "[lexer]") {
    Lexer lex("1e-");
    
    REQUIRE_THROWS(lex.next_token());
}

TEST_CASE("Lexer: basic tokens 74", "[lexer]") {
    Lexer lex("0.e23 - (rere)");
    
    REQUIRE_THROWS(lex.next_token());
}

TEST_CASE("Lexer: basic tokens 75", "[lexer]") {
    Lexer lex("_");
    
    auto token = lex.next_token();
    REQUIRE(token == "_");
    REQUIRE(token.type == lexem_t::VARIABLE);
}

TEST_CASE("Lexer: basic tokens 76", "[lexer]") {
    Lexer lex("_1");
    
    auto token = lex.next_token();
    REQUIRE(token == "_1");
    REQUIRE(token.type == lexem_t::VARIABLE);
}

TEST_CASE("Lexer: basic tokens 77", "[lexer]") {
    Lexer lex("a_b_c");
    
    auto token = lex.next_token();
    REQUIRE(token == "a_b_c");
    REQUIRE(token.type == lexem_t::VARIABLE);
}

TEST_CASE("Lexer: basic tokens 78", "[lexer]") {
    Lexer lex("A");
    
    auto token = lex.next_token();
    REQUIRE(token == "a");
    REQUIRE(token.type == lexem_t::VARIABLE);
}

TEST_CASE("Lexer: basic tokens 79", "[lexer]") {
    Lexer lex("SIN(x)");
    
    auto token = lex.next_token();
    REQUIRE(token == "sin");
    REQUIRE(token.type == lexem_t::FUNCTION);
    token = lex.next_token();
    REQUIRE(token == "(");
    REQUIRE(token.type == lexem_t::LPAREN);
    token = lex.next_token();
    REQUIRE(token == "x");
    REQUIRE(token.type == lexem_t::VARIABLE);
    token = lex.next_token();
    REQUIRE(token == ")");
    REQUIRE(token.type == lexem_t::RPAREN);
}

TEST_CASE("Lexer: basic tokens 80", "[lexer]") {
    Lexer lex("Sin1(x)");
    
    auto token = lex.next_token();
    REQUIRE(token == "sin1");
    
    token = lex.next_token();
    REQUIRE(token == "(");
    REQUIRE(token.type == lexem_t::LPAREN);
    token = lex.next_token();
    REQUIRE(token == "x");
    REQUIRE(token.type == lexem_t::VARIABLE);
    token = lex.next_token();
    REQUIRE(token == ")");
    REQUIRE(token.type == lexem_t::RPAREN);
}

TEST_CASE("Lexer: basic tokens 81", "[lexer]") {
    Lexer lex("1abc");
    
    REQUIRE_THROWS(lex.next_token());
}

TEST_CASE("Lexer: basic tokens 82", "[lexer]") {
    Lexer lex("0000000000000000000001");
    
    REQUIRE_THROWS(lex.next_token());
}

TEST_CASE("Lexer: basic tokens 83", "[lexer]") {
    Lexer lex("000.000");
    
    REQUIRE_THROWS(lex.next_token());
}

TEST_CASE("Lexer: basic tokens 84", "[lexer]") {
    Lexer lex("sin x");
    
    auto token = lex.next_token();
    REQUIRE(token == "sin");
    REQUIRE(token.type == lexem_t::FUNCTION);
    
    token = lex.next_token();
    REQUIRE(token == "x");
    REQUIRE(token.type == lexem_t::VARIABLE);
}

TEST_CASE("Lexer: basic tokens 85", "[lexer]") {
    Lexer lex("sinx");
    
    auto token = lex.next_token();
    REQUIRE(token == "sinx");
}

TEST_CASE("Lexer: basic tokens 86", "[lexer]") {
    Lexer lex("sin ( x )");
    
    auto token = lex.next_token();
    REQUIRE(token == "sin");
    REQUIRE(token.type == lexem_t::FUNCTION);
    token = lex.next_token();
    REQUIRE(token == "(");
    REQUIRE(token.type == lexem_t::LPAREN);
    token = lex.next_token();
    REQUIRE(token == "x");
    REQUIRE(token.type == lexem_t::VARIABLE);
    token = lex.next_token();
    REQUIRE(token == ")");
    REQUIRE(token.type == lexem_t::RPAREN);
}

TEST_CASE("Lexer: basic tokens 87", "[lexer]") {
    Lexer lex("sin (x)");
    
    auto token = lex.next_token();
    REQUIRE(token == "sin");
    REQUIRE(token.type == lexem_t::FUNCTION);
    token = lex.next_token();
    REQUIRE(token == "(");
    REQUIRE(token.type == lexem_t::LPAREN);
    token = lex.next_token();
    REQUIRE(token == "x");
    REQUIRE(token.type == lexem_t::VARIABLE);
    token = lex.next_token();
    REQUIRE(token == ")");
    REQUIRE(token.type == lexem_t::RPAREN);
}

TEST_CASE("Lexer: basic tokens 88", "[lexer]") {
    Lexer lex("sin(x)+cos(y)");
    
    auto token = lex.next_token();
    REQUIRE(token == "sin");
    REQUIRE(token.type == lexem_t::FUNCTION);
    token = lex.next_token();
    REQUIRE(token == "(");
    REQUIRE(token.type == lexem_t::LPAREN);
    token = lex.next_token();
    REQUIRE(token == "x");
    REQUIRE(token.type == lexem_t::VARIABLE);
    token = lex.next_token();
    REQUIRE(token == ")");
    REQUIRE(token.type == lexem_t::RPAREN);

    token = lex.next_token();
    REQUIRE(token == "+");
    REQUIRE(token.type == lexem_t::OPERATION);

    token = lex.next_token();
    REQUIRE(token == "cos");
    REQUIRE(token.type == lexem_t::FUNCTION);
    token = lex.next_token();
    REQUIRE(token == "(");
    REQUIRE(token.type == lexem_t::LPAREN);
    token = lex.next_token();
    REQUIRE(token == "y");
    REQUIRE(token.type == lexem_t::VARIABLE);
    token = lex.next_token();
    REQUIRE(token == ")");
    REQUIRE(token.type == lexem_t::RPAREN);
}

TEST_CASE("Lexer: basic tokens 89", "[lexer]") {
    Lexer lex("(x+y)*z");
    
    auto token = lex.next_token();
    REQUIRE(token == "(");
    REQUIRE(token.type == lexem_t::LPAREN);
    token = lex.next_token();
    REQUIRE(token == "x");
    REQUIRE(token.type == lexem_t::VARIABLE);
    token = lex.next_token();
    REQUIRE(token == "+");
    REQUIRE(token.type == lexem_t::OPERATION);
    token = lex.next_token();
    REQUIRE(token == "y");
    REQUIRE(token.type == lexem_t::VARIABLE);
    token = lex.next_token();
    REQUIRE(token == ")");
    REQUIRE(token.type == lexem_t::RPAREN);
    token = lex.next_token();
    REQUIRE(token == "*");
    REQUIRE(token.type == lexem_t::OPERATION);
    token = lex.next_token();
    REQUIRE(token == "z");
    REQUIRE(token.type == lexem_t::VARIABLE);
}

TEST_CASE("Lexer: basic tokens 90", "[lexer]") {
    Lexer lex("aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa");
    
    auto token = lex.next_token();
    REQUIRE(token == "aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa");
    REQUIRE(token.type == lexem_t::VARIABLE);
}

TEST_CASE("Lexer: non-basic tokens 91", "[lexer]") {
    Lexer lex("1.6e+1");
    
    auto token = lex.next_token();
    REQUIRE(token == "1.6e+1");
    REQUIRE(token.type == lexem_t::NUMBER);
}

TEST_CASE("Lexer: non-basic tokens 92", "[lexer]") {
    Lexer lex("1.7e-1");
    
    auto token = lex.next_token();
    REQUIRE(token == "1.7e-1");
    REQUIRE(token.type == lexem_t::NUMBER);
}

TEST_CASE("Lexer: non-basic tokens 93", "[lexer]") {
    Lexer lex("1.55e1");
    
    auto token = lex.next_token();
    REQUIRE(token == "1.55e1");
    REQUIRE(token.type == lexem_t::NUMBER);
}

TEST_CASE("Lexer: non-basic tokens 94", "[lexer]") {
    Lexer lex("1.84E9");
    
    auto token = lex.next_token();
    REQUIRE(token == "1.84E9");
    REQUIRE(token.type == lexem_t::NUMBER);
}

TEST_CASE("Lexer: non-basic tokens 95", "[lexer]") {
    Lexer lex("1.99e1");
    
    auto token = lex.next_token();
    REQUIRE(token == "1.99e1");
    REQUIRE(token.type == lexem_t::NUMBER);
}

TEST_CASE("Lexer: non-basic tokens 96", "[lexer]") {
    Lexer lex("1.e1");
    
    auto token = lex.next_token();
    REQUIRE(token == "1.e1");
    REQUIRE(token.type == lexem_t::NUMBER);
}

TEST_CASE("Lexer: non-basic tokens 97", "[lexer]") {
    Lexer lex("0.5e1");
    
    REQUIRE_THROWS(lex.next_token());
}

TEST_CASE("Lexer: non-basic tokens 98", "[lexer]") {
    Lexer lex("1e12");
    
    auto token = lex.next_token();
    REQUIRE(token == "1e12");
    REQUIRE(token.type == lexem_t::NUMBER);
}



TEST_CASE("Parser: simple expression", "[parser]") {
    Lexer lex("2 + 3 * x");
    Parser parser(lex);
    auto ast = parser.parse();
    REQUIRE(ast != nullptr);
}





TEST_CASE("EvaluatorVisitor: simple evaluation 1", "[evaluate]") {
    Lexer lex("1 + 1");
    Parser parser(lex);
    auto ast = parser.parse();
    
    std::unordered_map<std::string, double> vars;
    double result = CalcCommands::evaluate(ast, vars);
    REQUIRE(std::abs(result - 2.0) < 1e-6);
}

TEST_CASE("EvaluatorVisitor: simple evaluation 2", "[evaluate]") {
    Lexer lex("1   +        1");
    Parser parser(lex);
    auto ast = parser.parse();
    
    std::unordered_map<std::string, double> vars;
    double result = CalcCommands::evaluate(ast, vars);
    REQUIRE(std::abs(result - 2.0) < 1e-6);
}

TEST_CASE("EvaluatorVisitor: simple evaluation 3", "[evaluate]") {
    Lexer lex("0");
    Parser parser(lex);
    auto ast = parser.parse();
    
    std::unordered_map<std::string, double> vars;
    double result = CalcCommands::evaluate(ast, vars);
    REQUIRE(std::abs(result) < 1e-6);
}

TEST_CASE("EvaluatorVisitor: simple evaluation 4", "[evaluate]") {
    Lexer lex("0 + 0");
    Parser parser(lex);
    auto ast = parser.parse();
    
    std::unordered_map<std::string, double> vars;
    double result = CalcCommands::evaluate(ast, vars);
    REQUIRE(std::abs(result) < 1e-6);
}

TEST_CASE("EvaluatorVisitor: simple evaluation 5", "[evaluate]") {
    Lexer lex("0/1");
    Parser parser(lex);
    auto ast = parser.parse();
    
    std::unordered_map<std::string, double> vars;
    double result = CalcCommands::evaluate(ast, vars);
    REQUIRE(std::abs(result) < 1e-6);
}

TEST_CASE("EvaluatorVisitor: simple evaluation 6", "[evaluate]") {
    Lexer lex("5/0");
    Parser parser(lex);
    auto ast = parser.parse();
    
    std::unordered_map<std::string, double> vars;
    double result = CalcCommands::evaluate(ast, vars);
    REQUIRE(std::abs(result) == std::numeric_limits<double>::infinity());
}

TEST_CASE("EvaluatorVisitor: simple evaluation 7", "[evaluate]") {
    Lexer lex("2 + 4 * 5");
    Parser parser(lex);
    auto ast = parser.parse();
    
    std::unordered_map<std::string, double> vars;
    double result = CalcCommands::evaluate(ast, vars);
    REQUIRE(std::abs(result - 22) < 1e-6);
}

TEST_CASE("EvaluatorVisitor: simple evaluation 8", "[evaluate]") {
    Lexer lex("6 / 3");
    Parser parser(lex);
    auto ast = parser.parse();
    
    std::unordered_map<std::string, double> vars;
    double result = CalcCommands::evaluate(ast, vars);
    REQUIRE(std::abs(result - 2) < 1e-6);
}

TEST_CASE("EvaluatorVisitor: simple evaluation 9", "[evaluate]") {
    Lexer lex("5/2");
    Parser parser(lex);
    auto ast = parser.parse();
    
    std::unordered_map<std::string, double> vars;
    double result = CalcCommands::evaluate(ast, vars);
    REQUIRE(std::abs(result - 2.5) < 1e-6);
}

TEST_CASE("EvaluatorVisitor: simple evaluation 10", "[evaluate]") {
    Lexer lex("2/5");
    Parser parser(lex);
    auto ast = parser.parse();
    
    std::unordered_map<std::string, double> vars;
    double result = CalcCommands::evaluate(ast, vars);
    REQUIRE(std::abs(result - 0.4) < 1e-6);
}

TEST_CASE("EvaluatorVisitor: simple evaluation 11", "[evaluate]") {
    Lexer lex("2^2 + 5");
    Parser parser(lex);
    auto ast = parser.parse();
    
    std::unordered_map<std::string, double> vars;
    double result = CalcCommands::evaluate(ast, vars);
    REQUIRE(std::abs(result - 9) < 1e-6);
}

TEST_CASE("EvaluatorVisitor: simple evaluation 12", "[evaluate]") {
    Lexer lex("2 + 5*2^4");
    Parser parser(lex);
    auto ast = parser.parse();
    
    std::unordered_map<std::string, double> vars;
    double result = CalcCommands::evaluate(ast, vars);
    REQUIRE(std::abs(result - 82) < 1e-6);
}

TEST_CASE("EvaluatorVisitor: simple evaluation 13", "[evaluate]") {
    Lexer lex("(2 + 5)*2^4");
    Parser parser(lex);
    auto ast = parser.parse();
    
    std::unordered_map<std::string, double> vars;
    double result = CalcCommands::evaluate(ast, vars);
    REQUIRE(std::abs(result - 112) < 1e-6);
}

TEST_CASE("EvaluatorVisitor: simple evaluation 14", "[evaluate]") {
    Lexer lex("2 + (5 * 2) ^ 4");
    Parser parser(lex);
    auto ast = parser.parse();
    
    std::unordered_map<std::string, double> vars;
    double result = CalcCommands::evaluate(ast, vars);
    REQUIRE(std::abs(result - 10002) < 1e-6);
}

TEST_CASE("EvaluatorVisitor: simple evaluation 15", "[evaluate]") {
    Lexer lex("2 + 5 * (2 ^ 4)");
    Parser parser(lex);
    auto ast = parser.parse();
    
    std::unordered_map<std::string, double> vars;
    double result = CalcCommands::evaluate(ast, vars);
    REQUIRE(std::abs(result - 82) < 1e-6);
}

TEST_CASE("EvaluatorVisitor: simple evaluation 16", "[evaluate]") {
    Lexer lex("(2 + 5 * 2) ^ 4");
    Parser parser(lex);
    auto ast = parser.parse();
    
    std::unordered_map<std::string, double> vars;
    double result = CalcCommands::evaluate(ast, vars);
    REQUIRE(std::abs(result - 20736) < 1e-6);
}


TEST_CASE("EvaluatorVisitor: simple evaluation 17", "[evaluate]") {
    Lexer lex("((2 + 5) * 2) ^ 4");
    Parser parser(lex);
    auto ast = parser.parse();
    
    std::unordered_map<std::string, double> vars;
    double result = CalcCommands::evaluate(ast, vars);
    REQUIRE(std::abs(result - 38416) < 1e-6);
}

TEST_CASE("EvaluatorVisitor: simple evaluation 18", "[evaluate]") {
    Lexer lex("-2 * 4");
    Parser parser(lex);
    auto ast = parser.parse();
    
    std::unordered_map<std::string, double> vars;
    double result = CalcCommands::evaluate(ast, vars);
    REQUIRE(std::abs(result - (-8)) < 1e-6);
}

TEST_CASE("EvaluatorVisitor: simple evaluation 19", "[evaluate]") {
    Lexer lex("-2^2");
    Parser parser(lex);
    auto ast = parser.parse();
    
    std::unordered_map<std::string, double> vars;
    double result = CalcCommands::evaluate(ast, vars);
    REQUIRE(std::abs(result - (-4)) < 1e-6);
}

TEST_CASE("EvaluatorVisitor: simple evaluation 20", "[evaluate]") {
    Lexer lex("2 + -2^2");
    Parser parser(lex);
    auto ast = parser.parse();
    
    std::unordered_map<std::string, double> vars;
    double result = CalcCommands::evaluate(ast, vars);
    REQUIRE(std::abs(result - (-2)) < 1e-6);
}

TEST_CASE("EvaluatorVisitor: simple evaluation 21", "[evaluate]") {
    Lexer lex("2 + -2^2");
    Parser parser(lex);
    auto ast = parser.parse();
    
    std::unordered_map<std::string, double> vars;
    double result = CalcCommands::evaluate(ast, vars);
    REQUIRE(std::abs(result - (-2)) < 1e-6);
}

TEST_CASE("EvaluatorVisitor: simple evaluation 22", "[evaluate]") {
    Lexer lex("2 * -2^2");
    Parser parser(lex);
    auto ast = parser.parse();
    
    std::unordered_map<std::string, double> vars;
    double result = CalcCommands::evaluate(ast, vars);
    REQUIRE(std::abs(result - (-8)) < 1e-6);
}

TEST_CASE("EvaluatorVisitor: simple evaluation 23", "[evaluate]") {
    Lexer lex("2^-2");
    Parser parser(lex);
    auto ast = parser.parse();
    
    std::unordered_map<std::string, double> vars;
    double result = CalcCommands::evaluate(ast, vars);
    REQUIRE(std::abs(result - 0.25) < 1e-6);
}

TEST_CASE("EvaluatorVisitor: simple evaluation 24", "[evaluate]") {
    Lexer lex("(-2)^2");
    Parser parser(lex);
    auto ast = parser.parse();
    
    std::unordered_map<std::string, double> vars;
    double result = CalcCommands::evaluate(ast, vars);
    REQUIRE(std::abs(result - 4) < 1e-6);
}

TEST_CASE("EvaluatorVisitor: simple evaluation 25", "[evaluate]") {
    Lexer lex("-2^2");
    Parser parser(lex);
    auto ast = parser.parse();
    
    std::unordered_map<std::string, double> vars;
    double result = CalcCommands::evaluate(ast, vars);
    REQUIRE(std::abs(result - (-4)) < 1e-6);
}

TEST_CASE("EvaluatorVisitor: simple evaluation 26", "[evaluate]") {
    Lexer lex("-2^3");
    Parser parser(lex);
    auto ast = parser.parse();
    
    std::unordered_map<std::string, double> vars;
    double result = CalcCommands::evaluate(ast, vars);
    REQUIRE(std::abs(result - (-8)) < 1e-6);
}

TEST_CASE("EvaluatorVisitor: simple evaluation 27", "[evaluate]") {
    Lexer lex("2^-1");
    Parser parser(lex);
    auto ast = parser.parse();
    
    std::unordered_map<std::string, double> vars;
    double result = CalcCommands::evaluate(ast, vars);
    REQUIRE(std::abs(result - 0.5) < 1e-6);
}

TEST_CASE("EvaluatorVisitor: simple evaluation 28", "[evaluate]") {
    Lexer lex("(-2)^3");
    Parser parser(lex);
    auto ast = parser.parse();
    
    std::unordered_map<std::string, double> vars;
    double result = CalcCommands::evaluate(ast, vars);
    REQUIRE(std::abs(result - (-8)) < 1e-6);
}

TEST_CASE("EvaluatorVisitor: simple evaluation 29", "[evaluate]") {
    Lexer lex("2^3/2");
    Parser parser(lex);
    auto ast = parser.parse();
    
    std::unordered_map<std::string, double> vars;
    double result = CalcCommands::evaluate(ast, vars);
    REQUIRE(std::abs(result - 4) < 1e-6);
}

TEST_CASE("EvaluatorVisitor: simple evaluation 30", "[evaluate]") {
    Lexer lex("2^2^2^2^2");
    Parser parser(lex);
    auto ast = parser.parse();
    
    std::unordered_map<std::string, double> vars;
    double result = CalcCommands::evaluate(ast, vars);
    REQUIRE(std::abs(result) == std::numeric_limits<double>::infinity());
}

TEST_CASE("EvaluatorVisitor: simple evaluation 31", "[evaluate]") {
    Lexer lex("1+1+1+1+1+1+1+1+1+1");
    Parser parser(lex);
    auto ast = parser.parse();
    
    std::unordered_map<std::string, double> vars;
    double result = CalcCommands::evaluate(ast, vars);
    REQUIRE(std::abs(result - 10) < 1e-6);
}

TEST_CASE("EvaluatorVisitor: simple evaluation 32", "[evaluate]") {
    Lexer lex("1-2+3-4+5-6+7-8+9");
    Parser parser(lex);
    auto ast = parser.parse();
    
    std::unordered_map<std::string, double> vars;
    double result = CalcCommands::evaluate(ast, vars);
    REQUIRE(std::abs(result - 5) < 1e-6);
}

TEST_CASE("EvaluatorVisitor: simple evaluation 33", "[evaluate]") {
    Lexer lex("2^3^2");
    Parser parser(lex);
    auto ast = parser.parse();
    
    std::unordered_map<std::string, double> vars;
    double result = CalcCommands::evaluate(ast, vars);
    REQUIRE(std::abs(result - 512) < 1e-6);
}

TEST_CASE("EvaluatorVisitor: simple evaluation 34", "[evaluate]") {
    Lexer lex("3^2^2");
    Parser parser(lex);
    auto ast = parser.parse();
    
    std::unordered_map<std::string, double> vars;
    double result = CalcCommands::evaluate(ast, vars);
    REQUIRE(std::abs(result - 81) < 1e-6);
}

TEST_CASE("EvaluatorVisitor: simple evaluation 35", "[evaluate]") {
    Lexer lex("2^2^2^2");
    Parser parser(lex);
    auto ast = parser.parse();
    
    std::unordered_map<std::string, double> vars;
    double result = CalcCommands::evaluate(ast, vars);
    REQUIRE(std::abs(result - 65536) < 1e-6);
}

TEST_CASE("EvaluatorVisitor: simple evaluation 36", "[evaluate]") {
    Lexer lex("+2");
    Parser parser(lex);
    auto ast = parser.parse();
    
    std::unordered_map<std::string, double> vars;
    double result = CalcCommands::evaluate(ast, vars);
    REQUIRE(std::abs(result - 2) < 1e-6);
}

TEST_CASE("EvaluatorVisitor: simple evaluation 37", "[evaluate]") {
    Lexer lex("++2");
    Parser parser(lex);
    auto ast = parser.parse();
    
    std::unordered_map<std::string, double> vars;
    double result = CalcCommands::evaluate(ast, vars);
    REQUIRE(std::abs(result - 2) < 1e-6);
}

TEST_CASE("EvaluatorVisitor: simple evaluation 38", "[evaluate]") {
    Lexer lex("+-2");
    Parser parser(lex);
    auto ast = parser.parse();
    
    std::unordered_map<std::string, double> vars;
    double result = CalcCommands::evaluate(ast, vars);
    REQUIRE(std::abs(result - (-2)) < 1e-6);
}

TEST_CASE("EvaluatorVisitor: simple evaluation 39", "[evaluate]") {
    Lexer lex("+++--++-++--2");
    Parser parser(lex);
    auto ast = parser.parse();
    
    std::unordered_map<std::string, double> vars;
    double result = CalcCommands::evaluate(ast, vars);
    REQUIRE(std::abs(result - (-2)) < 1e-6);
}

TEST_CASE("EvaluatorVisitor: simple evaluation 40", "[evaluate]") {
    Lexer lex("2 + ----2");
    Parser parser(lex);
    auto ast = parser.parse();
    
    std::unordered_map<std::string, double> vars;
    double result = CalcCommands::evaluate(ast, vars);
    REQUIRE(std::abs(result - 4) < 1e-6);
}

TEST_CASE("EvaluatorVisitor: simple evaluation 41", "[evaluate]") {
    Lexer lex("2 + ---2");
    Parser parser(lex);
    auto ast = parser.parse();
    
    std::unordered_map<std::string, double> vars;
    double result = CalcCommands::evaluate(ast, vars);
    REQUIRE(std::abs(result) < 1e-6);
}

TEST_CASE("EvaluatorVisitor: simple evaluation 42", "[evaluate]") {
    Lexer lex("(2 + 3) * (4 + 5) * (6 + 7)");
    Parser parser(lex);
    auto ast = parser.parse();
    
    std::unordered_map<std::string, double> vars;
    double result = CalcCommands::evaluate(ast, vars);
    REQUIRE(std::abs(result - 585) < 1e-6);
}

TEST_CASE("EvaluatorVisitor: simple evaluation 43", "[evaluate]") {
    Lexer lex("2^0.5");
    Parser parser(lex);
    auto ast = parser.parse();
    
    std::unordered_map<std::string, double> vars;
    double result = CalcCommands::evaluate(ast, vars);
    REQUIRE(std::abs(result - 1.41421356) < 1e-6);
}

TEST_CASE("EvaluatorVisitor: simple evaluation 44", "[evaluate]") {
    Lexer lex("2 + 1e+5 + 2e6 + 2.6e-2");
    Parser parser(lex);
    auto ast = parser.parse();
    
    std::unordered_map<std::string, double> vars;
    double result = CalcCommands::evaluate(ast, vars);
    REQUIRE(std::abs(result - 2100002.026) < 1e-6);
}

TEST_CASE("EvaluatorVisitor: variable evaluation 1", "[evaluate]") {
    Lexer lex("x+y+z");
    Parser parser(lex);
    auto ast = parser.parse();
    
    std::unordered_map<std::string, double> vars = {{"z", 3}, {"x", 1}, {"y", 2}};
    double result = CalcCommands::evaluate(ast, vars);
    REQUIRE(std::abs(result - 6) < 1e-6);
}

TEST_CASE("EvaluatorVisitor: variable evaluation 2", "[evaluate]") {
    Lexer lex("x^y + y^x");
    Parser parser(lex);
    auto ast = parser.parse();
    
    std::unordered_map<std::string, double> vars = {{"x", 2}, {"y", 3}};
    double result = CalcCommands::evaluate(ast, vars);
    REQUIRE(std::abs(result - 17) < 1e-6);
}

TEST_CASE("EvaluatorVisitor: variable evaluation 3", "[evaluate]") {
    Lexer lex("x^2 + x");
    Parser parser(lex);
    auto ast = parser.parse();
    
    std::unordered_map<std::string, double> vars = {{"x", -2}};
    double result = CalcCommands::evaluate(ast, vars);
    REQUIRE(std::abs(result - 2) < 1e-6);
}

TEST_CASE("EvaluatorVisitor: variable evaluation 4", "[evaluate]") {
    Lexer lex("x + y*z + x^y + z^x");
    Parser parser(lex);
    auto ast = parser.parse();
    
    std::unordered_map<std::string, double> vars = {{"x", 2}, {"y", -3.2}, {"z", 4}};
    double result = CalcCommands::evaluate(ast, vars);
    REQUIRE(std::abs(result - 5.3088188) < 1e-6);
}

TEST_CASE("EvaluatorVisitor: variable evaluation 5", "[evaluate]") {
    Lexer lex("x + y*z + x^y + z^x");
    Parser parser(lex);
    auto ast = parser.parse();
    
    std::unordered_map<std::string, double> vars = {{"x", 0.1}, {"y", -3.2}, {"z", 4.5}};
    double result = CalcCommands::evaluate(ast, vars);
    REQUIRE(std::abs(result - 1571.7555) < 1e-6);
}

TEST_CASE("EvaluatorVisitor: variable evaluation 6", "[evaluate]") {
    Lexer lex("sin(x)");
    Parser parser(lex);
    auto ast = parser.parse();
    
    std::unordered_map<std::string, double> vars = {{"x", 0}};
    double result = CalcCommands::evaluate(ast, vars);
    REQUIRE(std::abs(result) < 1e-6);
}

TEST_CASE("EvaluatorVisitor: variable evaluation 7", "[evaluate]") {
    Lexer lex("sin(cos(x))");
    Parser parser(lex);
    auto ast = parser.parse();
    
    std::unordered_map<std::string, double> vars = {{"x", 1}};
    double result = CalcCommands::evaluate(ast, vars);
    REQUIRE(std::abs(result - 0.51439525) < 1e-6);
}

TEST_CASE("EvaluatorVisitor: variable evaluation 8", "[evaluate]") {
    Lexer lex("sin(x) + y^2");
    Parser parser(lex);
    auto ast = parser.parse();
    
    std::unordered_map<std::string, double> vars = {{"x", 1}, {"y", 2}};
    double result = CalcCommands::evaluate(ast, vars);
    REQUIRE(std::abs(result - 4.84147098) < 1e-6);
}

TEST_CASE("EvaluatorVisitor: variable evaluation 9", "[evaluate]") {
    Lexer lex("sin(x)^2");
    Parser parser(lex);
    auto ast = parser.parse();
    
    std::unordered_map<std::string, double> vars = {{"x", 2}};
    double result = CalcCommands::evaluate(ast, vars);
    REQUIRE(std::abs(result - 0.8268218104) < 1e-6);
}

TEST_CASE("EvaluatorVisitor: variable evaluation 10", "[evaluate]") {
    Lexer lex("sin(x^2)");
    Parser parser(lex);
    auto ast = parser.parse();
    
    std::unordered_map<std::string, double> vars = {{"x", 2}};
    double result = CalcCommands::evaluate(ast, vars);
    REQUIRE(std::abs(result - (-0.75680249)) < 1e-6);
}

TEST_CASE("Derivative at a point: Derivative evaluation 1", "[evaluate_derivative]") {
    Lexer lex("5");
    Parser parser(lex);
    auto ast = parser.parse();

    std::unordered_map<std::string, double> vars = {{"x", 2}};
    auto deriv = CalcCommands::differentiate(ast, "x");
    double result = CalcCommands::evaluate(deriv, vars);
    REQUIRE(std::abs(result) < 1e-6);
}

TEST_CASE("Derivative at a point: Derivative evaluation 2", "[evaluate_derivative]") {
    Lexer lex("x^2");
    Parser parser(lex);
    auto ast = parser.parse();

    std::unordered_map<std::string, double> vars = {{"x", 2}};
    auto deriv = CalcCommands::differentiate(ast, "x");
    double result = CalcCommands::evaluate(deriv, vars);
    REQUIRE(std::abs(result - 4) < 1e-6);
}

TEST_CASE("Derivative at a point: Derivative evaluation 3", "[evaluate_derivative]") {
    Lexer lex("x^3");
    Parser parser(lex);
    auto ast = parser.parse();

    std::unordered_map<std::string, double> vars = {{"x", 3}};
    auto deriv = CalcCommands::differentiate(ast, "x");
    double result = CalcCommands::evaluate(deriv, vars);
    REQUIRE(std::abs(result - 27) < 1e-6);
}

TEST_CASE("Derivative at a point: Derivative evaluation 4", "[evaluate_derivative]") {
    Lexer lex("x*x");
    Parser parser(lex);
    auto ast = parser.parse();

    std::unordered_map<std::string, double> vars = {{"x", 2}};
    auto deriv = CalcCommands::differentiate(ast, "x");
    double result = CalcCommands::evaluate(deriv, vars);
    REQUIRE(std::abs(result - 4) < 1e-6);
}

TEST_CASE("Derivative at a point: Derivative evaluation 5", "[evaluate_derivative]") {
    Lexer lex("1/x");
    Parser parser(lex);
    auto ast = parser.parse();

    std::unordered_map<std::string, double> vars = {{"x", 2}};
    auto deriv = CalcCommands::differentiate(ast, "x");
    double result = CalcCommands::evaluate(deriv, vars);
    REQUIRE(std::abs(result - (-0.25)) < 1e-6);
}

TEST_CASE("Derivative at a point: Derivative evaluation 6", "[evaluate_derivative]") {
    Lexer lex("sin(x)");
    Parser parser(lex);
    auto ast = parser.parse();

    std::unordered_map<std::string, double> vars = {{"x", 0}};
    auto deriv = CalcCommands::differentiate(ast, "x");
    double result = CalcCommands::evaluate(deriv, vars);
    REQUIRE(std::abs(result - 1) < 1e-6);
}

TEST_CASE("Derivative at a point: Derivative evaluation 7", "[evaluate_derivative]") {
    Lexer lex("cos(x)");
    Parser parser(lex);
    auto ast = parser.parse();

    std::unordered_map<std::string, double> vars = {{"x", 0}};
    auto deriv = CalcCommands::differentiate(ast, "x");
    double result = CalcCommands::evaluate(deriv, vars);
    REQUIRE(std::abs(result) < 1e-6);
}

TEST_CASE("Derivative at a point: Derivative evaluation 8", "[evaluate_derivative]") {
    Lexer lex("log(x)");
    Parser parser(lex);
    auto ast = parser.parse();

    std::unordered_map<std::string, double> vars = {{"x", 1}};
    auto deriv = CalcCommands::differentiate(ast, "x");
    double result = CalcCommands::evaluate(deriv, vars);
    REQUIRE(std::abs(result - 1) < 1e-6);
}

TEST_CASE("Derivative at a point: Derivative evaluation 9", "[evaluate_derivative]") {
    Lexer lex("sqrt(x)");
    Parser parser(lex);
    auto ast = parser.parse();

    std::unordered_map<std::string, double> vars = {{"x", 4}};
    auto deriv = CalcCommands::differentiate(ast, "x");
    double result = CalcCommands::evaluate(deriv, vars);
    REQUIRE(std::abs(result - 0.25) < 1e-6);
}

TEST_CASE("Derivative at a point: Derivative evaluation 10", "[evaluate_derivative]") {
    Lexer lex("x*y");
    Parser parser(lex);
    auto ast = parser.parse();

    std::unordered_map<std::string, double> vars = {{"x", 2}, {"y", 3}};
    auto deriv = CalcCommands::differentiate(ast, "x");
    double result = CalcCommands::evaluate(deriv, vars);
    REQUIRE(std::abs(result - 3) < 1e-6);
}

TEST_CASE("Derivative at a point: Derivative evaluation 11", "[evaluate_derivative]") {
    Lexer lex("sin(x*y)");
    Parser parser(lex);
    auto ast = parser.parse();

    std::unordered_map<std::string, double> vars = {{"x", 1}, {"y", 2}};
    auto deriv = CalcCommands::differentiate(ast, "x");
    double result = CalcCommands::evaluate(deriv, vars);
    REQUIRE(std::abs(result - (-0.8322937)) < 1e-6);
}

TEST_CASE("Derivative at a point: Derivative evaluation 12", "[evaluate_derivative]") {
    Lexer lex("y^2");
    Parser parser(lex);
    auto ast = parser.parse();

    std::unordered_map<std::string, double> vars = {{"x", 1}, {"y", 2}};
    auto deriv = CalcCommands::differentiate(ast, "x");
    double result = CalcCommands::evaluate(deriv, vars);
    REQUIRE(std::abs(result) < 1e-6);
}