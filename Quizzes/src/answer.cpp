#include "answer.h"
using namespace std;

//------------------Answer-implementation--------------------------------------
Answer::Answer(){}

Answer::~Answer(){}

//------------------ANumeric-implementation------------------------------------
ANumeric::ANumeric(){}

ANumeric::~ANumeric(){}

//------------------ANumber-implementation-------------------------------------
ANumber::ANumber(double number)
: number(number){}

ANumber::~ANumber(){}

double ANumber::evaluate(const UserAnswer & userAnswer) const{
  if (userAnswer.number == number)
    return 1;

  return 0;
}

std::vector<std::string> ANumber::getAnswerExport() const{
  vector<string> out;
  out.push_back("ANumber");
  out.push_back(to_string(number));
  return out;
}


//------------------AInterval-implementation-----------------------------------
AInterval::AInterval(double leftBound, double rightBound, bool leftBounded, bool rightBounded)
: leftBound(leftBound), rightBound(rightBound), leftBounded(leftBounded), rightBounded(rightBounded){}

AInterval::~AInterval(){}

double AInterval::evaluate(const UserAnswer & userAnswer) const{
  if (userAnswer.number > leftBound && userAnswer.number < rightBound)
    return 1;

  if (leftBounded == 1 && userAnswer.number == leftBound)
    return 1;

  if (rightBounded == 1 && userAnswer.number == rightBound)
    return 1;

  return 0;
}

std::vector<std::string> AInterval::getAnswerExport() const{
  vector<string> out;
  out.push_back("AInterval");
  out.push_back(to_string(leftBound));
  out.push_back(to_string(rightBound));
  out.push_back(to_string(leftBounded));
  out.push_back(to_string(rightBounded));
  return out;
}

//------------------AResidueClass-implementation-------------------------------
AResidueClass::AResidueClass(int modulus)
: modulus(modulus){}

AResidueClass::~AResidueClass(){}

double AResidueClass::evaluate(const UserAnswer & userAnswer) const{
  if ((int)userAnswer.number % modulus == 0)
    return 1;

  return 0;
}

std::vector<std::string> AResidueClass::getAnswerExport() const{
  vector<string> out;
  out.push_back("AResidueClass");
  out.push_back(to_string(modulus));
  return out;
}

//------------------AChoices-implementation------------------------------------
AChoices::AChoices(const std::vector<int> & correctAnswers)
: answers(correctAnswers){}

AChoices::~AChoices(){}

double AChoices::evaluate(const UserAnswer & userAnswer) const{
  int correct = 0;
  for(size_t i = 0; i < userAnswer.vec.size(); i++){
    if (userAnswer.vec[i] == answers[i])
      correct++;
  }

  return (double)correct / userAnswer.vec.size();
}

std::vector<std::string> AChoices::getAnswerExport() const{
  vector<string> out;
  out.push_back("AChoices");
  for(auto it : answers){
    out.push_back(to_string(it));
  }
  return out;
}

//------------------AText-implementation--------------------------------------
AText::AText(const std::vector<std::string> & correctAnswers)
: answers(correctAnswers){}

AText::~AText(){}

double AText::evaluate(const UserAnswer & userAnswer) const{
  std::string answer = userAnswer.text;

  for(auto it : answers){
    if (answer == it)
      return 1;
  }

  return 0;
}

std::vector<std::string> AText::getAnswerExport() const{
  vector<string> out;
  out.push_back("AText");
  out.push_back(to_string(answers.size()));
  for(auto it : answers){
    out.push_back(it);
  }
  return out;
}

//------------------AOrder-implementation--------------------------------------
AOrder::AOrder(const std::vector<int> correctOrder)
: order(correctOrder){}

AOrder::~AOrder(){}

double AOrder::evaluate(const UserAnswer & userAnswer) const{
  int correct = 0;
  for(size_t i = 0; i < userAnswer.vec.size(); i++){
    if (order[i] == userAnswer.vec[i])
      correct++;
  }

  return (double)correct / userAnswer.vec.size();
}

std::vector<std::string> AOrder::getAnswerExport() const{
  vector<string> out;
  out.push_back("AOrder");
  for(auto it : order){
    out.push_back(to_string(it));
  }
  return out;
}
