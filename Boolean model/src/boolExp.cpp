#include "boolExp.h"
#include "kmp.h"

booleanExpression::booleanExpression(const std::string & exp) : boolExp(exp){
  parseToPostfix();
  for(size_t i = 0; i < postfix.size(); i++){
    if (!isOperatorOrBracket(postfix[i])){
      // postfix[i] = s.stemmer(postfix[i]);
    }
  }
}

bool booleanExpression::isOperator(const char ch){
  if (ch == '&' ||
      ch == '!' ||
      ch == '|'){
    return true;
  }
  return false;
}

bool booleanExpression::isOperatorOrBracket(const char ch){
  if (ch == '&' ||
      ch == '!' ||
      ch == '|' ||
      ch == '(' ||
      ch == ')'){
    return true;
  }
  return false;
}

bool booleanExpression::isOperatorOrBracket(const std::string & ch){
  if (ch == "&" ||
      ch == "!" ||
      ch == "|" ||
      ch == "(" ||
      ch == ")"){
    return true;
  }
  return false;
}

bool booleanExpression::isUnaryOperator(const std::string & ch){
  if (ch == "!"){
    return true;
  }
  return false;
}

bool booleanExpression::isBinaryOperator(const std::string & ch){
  if (ch == "&" ||
      ch == "|"){
    return true;
  }
  return false;
}

int booleanExpression::operatorPrecedence(const char op){
  if (op == '!')
    return 10;
  if (op == '&' || op == '|')
    return 5;
  if (op == '(')
    return 1;

  return -1;
}

void booleanExpression::parseToPostfix(){
  std::stack<char> operators;
  size_t expLen = boolExp.size();
  size_t i = 0;
  while (i < expLen){
    if (isOperator(boolExp[i])){
      if (operators.empty()){
        operators.push(boolExp[i]);
        i++;
      }
      else if (operatorPrecedence(boolExp[i]) > operatorPrecedence(operators.top())){
        operators.push(boolExp[i]);
        i++;
      }
      else {
        while (!operators.empty() && !(operatorPrecedence(boolExp[i]) > operatorPrecedence(operators.top()))){
          std::string op (1, operators.top());
          postfix.push_back(op);
          operators.pop();
        }
        operators.push(boolExp[i]);
        i++;
      }
    }
    else if (boolExp[i] == '('){
      operators.push(boolExp[i]);
      i++;
    }
    else if (boolExp[i] == ')'){
      while(!operators.empty() && operators.top() != '('){
        std::string op (1, operators.top());
        postfix.push_back(op);
        operators.pop();
      }
      operators.pop();
      i++;
    }
    else {
      std::string word = "";
      while(i < expLen && !isOperatorOrBracket(boolExp[i])){
        word += boolExp[i];
        i++;
      }
      postfix.push_back(word);
    }
  }

  while(!operators.empty()){
    std::string op (1, operators.top());
    postfix.push_back(op);
    operators.pop();
  }
}

bool booleanExpression::evaluate_KMP(const std::string & text){
  evaluated.clear();
  for(size_t i = 0; i < postfix.size(); i++){
    if (!isOperatorOrBracket(postfix[i])){
      evaluated.push_back(std::to_string(KMP_search(text, postfix[i])));
    }
    else {
      evaluated.push_back(postfix[i]);
    }
  }
  return evaluate();
}

bool booleanExpression::evaluate_invertedIndex(const Index & index, const int fileIndex){
  evaluated.clear();
  for(size_t i = 0; i < postfix.size(); i++){
    if (!isOperatorOrBracket(postfix[i])){
      evaluated.push_back(std::to_string(index.searchInvertedIndex(postfix[i], fileIndex)));
    }
    else {
      evaluated.push_back(postfix[i]);
    }
  }
  return evaluate();
}

bool booleanExpression::evaluate(){
  std::stack<bool> operands;
  for(size_t i = 0; i < evaluated.size(); i++){
    if (!isOperatorOrBracket(evaluated[i])){
      operands.push(std::stoi(evaluated[i]));
    }
    else {
      if (isUnaryOperator(evaluated[i])){
        bool operand_1 = operands.top();
        operands.pop();
        operands.push(!operand_1);
      }
      else {
        bool operand_1 = operands.top();
        operands.pop();
        bool operand_2 = operands.top();
        operands.pop();
        if (evaluated[i] == "&")
          operands.push(operand_1 && operand_2);
        else if (postfix[i] == "|")
          operands.push(operand_1 || operand_2);
      }
    }
  }
  return operands.top();
}
