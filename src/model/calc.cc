#include "calc.h"

#include <cmath>
#include <functional>
#include <map>
#include <stack>
#include <stdexcept>
#include <variant>

namespace s21 {
void Calc::InfixToPostfix() {
  while (!stack_.empty()) stack_.pop();
  for (size_t i = 0, size = infix_.size(); i < size; ++i) {
    if (std::isspace(infix_[i]))
      ;
    else if (infix_[i] == 'x')
      HandleVariable();
    else if (std::isdigit(infix_[i]))
      i = HandleNumber(i);
    else if (std::isalpha(infix_[i]))
      i = HandleFunction(i);
    else
      HandleSymbol(i);
  }
  PushAll();
}

void Calc::HandleVariable() {
  postfix_.push_back(Token(kVariable, kDefault, nullptr));
  PushFunctionFromTop();
}

size_t Calc::HandleNumber(size_t i) {
  size_t j;
  double n = std::stod(infix_.substr(i), &j);
  postfix_.push_back(Token(kNumber, kDefault, n));
  PushFunctionFromTop();
  return --i + j;
}

size_t Calc::HandleFunction(size_t i) {
  std::string func;
  while (std::isalpha(infix_[i]) && infix_[i] != 'x') func += infix_[i++];
  if (func == "mod")
    HandleOperator('%');
  else if (token_map.find(func) != token_map.end())
    stack_.push(token_map.at(func));
  else
    throw std::logic_error("unknown function: " + func);
  return --i;
}

void Calc::HandleSymbol(size_t i) {
  if (infix_[i] == '+' && IsUnary(i)) return;
  if (infix_[i] == '-' && IsUnary(i))
    HandleOperator('~');
  else if (token_map.find(std::string(1, infix_[i])) != token_map.end())
    HandleOperator(infix_[i]);
  else if (infix_[i] == '(')
    stack_.push(Token(kBracket, kDefault, nullptr));
  else if (infix_[i] == ')')
    HandleBracket(i);
  else
    throw std::logic_error(std::string("unknown symbol: ") + infix_[i]);
}

void Calc::HandleOperator(char op) {
  Token t = token_map.at(std::string(1, op));
  while (!stack_.empty() && stack_.top().type_ == kBinary &&
         (stack_.top().priority_ > t.priority_ ||
          (stack_.top().priority_ == t.priority_ &&
           stack_.top().priority_ < kHigh))) {
    postfix_.push_back(stack_.top());
    stack_.pop();
  }
  stack_.push(t);
}

void Calc::HandleBracket(size_t i) {
  if (PrevIsBracket(i)) throw std::logic_error("empty brackets");
  while (true) {
    if (stack_.empty()) throw std::logic_error("invalid expression brackets");
    Token t = stack_.top();
    stack_.pop();
    if (t.type_ == kBracket) break;
    postfix_.push_back(t);
  }
  PushFunctionFromTop();
}

bool Calc::PrevIsBracket(size_t i) {
  while (std::isspace(infix_[--i]))
    ;
  return infix_[i] == '(';
}

bool Calc::IsUnary(size_t i) {
  return (infix_[i] == '+' || infix_[i] == '-') &&
         (postfix_.empty() || PrevIsBracket(i));
}

void Calc::PushFunctionFromTop() {
  if (!stack_.empty() && stack_.top().type_ == kUnary) {
    postfix_.push_back(stack_.top());
    stack_.pop();
  }
}

void Calc::PushAll() {
  while (!stack_.empty()) {
    Token t = stack_.top();
    if (t.type_ == kBracket) throw std::logic_error("invalid expression push");
    stack_.pop();
    postfix_.push_back(t);
  }
}

void Calc::LoadExpression(const std::string &expression) {
  if (infix_ != expression) {
    infix_ = expression;
    postfix_.clear();
    InfixToPostfix();
  }
}

//===========================
double Calc::Calculate(double x) {
  while (!result_.empty()) result_.pop();
  for (auto t : postfix_) {
    std::visit(overloaded{[&](double f) { PushResult(f); },
                          [&](unary f) {
                            double a = PopResult();
                            PushResult(f(a));
                          },
                          [&](binary f) {
                            double b = PopResult();
                            double a = PopResult();
                            PushResult(f(a, b));
                          },
                          [&](auto) { PushResult(x); }},
               t.function_);
  }
  if (result_.size() != 1) throw std::logic_error("invalid expression calc");
  return result_.top();
}

void Calc::PushResult(double d) { result_.push(d); }

double Calc::PopResult() {
  if (result_.empty()) throw std::logic_error("invalid expression pop");
  double res = result_.top();
  result_.pop();
  return res;
}

//===========================
Graph Calc::CaculateGraph(double x_min, double x_max) {
  if (x_min >= x_max) throw std::logic_error("x min is greater than x max");
  const int N = 1000;
  std::vector<double> graph_x, graph_y;
  double dx = (x_max - x_min) / (N - 1);
  for (double x = x_min; x < x_max; x += dx) {
    graph_x.push_back(x);
    graph_y.push_back(Calculate(x));
  }
  return std::make_pair(graph_x, graph_y);
}

}  // namespace s21