#ifndef SMARTCALC_MODEL_CALC_H_
#define SMARTCALC_MODEL_CALC_H_

#include <cmath>
#include <functional>
#include <map>
#include <stack>
#include <string>
#include <utility>
#include <variant>
#include <vector>

namespace s21 {
template <class... Ts>
struct overloaded : Ts... {
  using Ts::operator()...;
};
template <class... Ts>
overloaded(Ts...) -> overloaded<Ts...>;

using Graph = std::pair<std::vector<double>, std::vector<double>>;
using unary = std::function<double(double)>;
using binary = std::function<double(double, double)>;
using function_variant = std::variant<double, unary, binary, std::nullptr_t>;
enum TokenType { kNumber, kVariable, kBracket, kBinary, kUnary };
enum TokenPriority {
  kDefault,
  kLow,
  kMid,
  kHigh,
};
/// @brief main calculator model
class Calc {
 public:
  Calc() = default;
  ~Calc() = default;
  /// @brief loads infix expression and calls function to translate it into RPN
  /// @param expression mathematical expression in string form
  void LoadExpression(const std::string &expression);

  /// @brief calculates result from RPN
  /// @param x variable value in the expression
  /// @return calculated result
  double Calculate(double x = 0);

  /// @brief calculates x and y values of the loaded expression between x_min
  /// and x_max
  /// @param x_min lower bound of the x axis of the graph
  /// @param x_max upper bound of the x axis of the graph
  /// @return pair of vectors containing x and y values of the graph
  Graph CaculateGraph(double x_min, double x_max);

 private:
  struct Token {
    TokenType type_{};
    TokenPriority priority_{};
    function_variant function_{};
    Token(TokenType type, TokenPriority prio, function_variant f)
        : type_(type), priority_(prio), function_(f) {}
    ~Token() = default;
  };

  std::string infix_{};
  std::vector<Token> postfix_{};
  std::stack<Token> stack_{};
  std::stack<double> result_{};

  const std::map<std::string, Token> token_map{
      {"sin", Token(kUnary, kDefault, sinl)},
      {"cos", Token(kUnary, kDefault, cosl)},
      {"tan", Token(kUnary, kDefault, tanl)},
      {"asin", Token(kUnary, kDefault, asinl)},
      {"acos", Token(kUnary, kDefault, acosl)},
      {"atan", Token(kUnary, kDefault, atanl)},
      {"sqrt", Token(kUnary, kDefault, sqrtl)},
      {"ln", Token(kUnary, kDefault, logl)},
      {"log", Token(kUnary, kDefault, log10l)},
      {"~", Token(kUnary, kDefault, std::negate<double>())},
      {"+", Token(kBinary, kLow, std::plus<double>())},
      {"-", Token(kBinary, kLow, std::minus<double>())},
      {"*", Token(kBinary, kMid, std::multiplies<double>())},
      {"/", Token(kBinary, kMid, std::divides<double>())},
      {"^", Token(kBinary, kHigh, powl)},
      {"%", Token(kBinary, kMid, fmodl)}};

  /// @brief parses the loaded expression and translates it into RPN
  void InfixToPostfix();

  /// @brief parses and sorts functions
  /// @param i index of the first symbol of supposed function in the infix
  /// expression
  /// @return index after the function name
  size_t HandleFunction(size_t i);

  /// @brief parses and sorts variables
  void HandleVariable();

  /// @brief parses and sorts numbers
  /// @param i index of the first symbol of the number in the infix expression
  /// @return index after the parsed number
  size_t HandleNumber(size_t i);

  /// @brief sorts operators
  /// @param op symbol of the operator
  void HandleOperator(char op);

  /// @brief sorts brackets
  /// @param i index of the bracket in the infix expression
  void HandleBracket(size_t i);

  /// @brief parses and sorts symbols that are not numbers or letters
  /// @param i index of the symbol in the infix expression
  void HandleSymbol(size_t i);

  /// @brief checks if top of the stack is a function and pushes it into the
  /// result vector
  void PushFunctionFromTop();

  /// @brief checks if the operator is unary
  /// @param i index of the symbol in the infix expression
  bool IsUnary(size_t i);

  /// @brief checks if previous symbol was an opening bracket
  /// @param i index of the symbol in the infix expression
  bool PrevIsBracket(size_t i);

  /// @brief pushes remaining tokens into the result vector
  void PushAll();

  /// @brief pops a number from the result stack
  /// @return popped number
  double PopResult();

  /// @brief pushes a number into the result stack
  /// @param d number to push
  void PushResult(double d);
};

}  // namespace s21

#endif  // SMARTCALC_MODEL_CALC_H_
