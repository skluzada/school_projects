#pragma once
#include "stemmer.h"
#include "index.h"
#include <stack>
#include <vector>
#include <string>

class booleanExpression{
public:
  booleanExpression(const std::string & exp);
  // evaluate boolean expression using KMP algorithm
  bool evaluate_KMP(const std::string & text);
  // evaluate boolean expression using inverted index
  bool evaluate_invertedIndex(const Index & index, const int fileIndex);
private:
  std::string boolExp;
  std::vector<std::string> postfix;
  std::vector<std::string> evaluated;
  Stemmer s;
  bool isOperator(const char ch);
  bool isOperatorOrBracket(const char ch);
  bool isOperatorOrBracket(const std::string & ch);
  bool isUnaryOperator(const std::string & ch);
  bool isBinaryOperator(const std::string & ch);
  int operatorPrecedence(const char op);
  // my implementation of Shutting-yard algorithm
  // https://en.wikipedia.org/wiki/Shunting-yard_algorithm
  void parseToPostfix();
  bool evaluate();
};
