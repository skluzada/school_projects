#include "question.h"
using namespace std;

//------------------Question-implementation------------------------------------
Question::Question(const std::string & question, Answer * correctAnswer, int difficulty, double points)
: question(question), answer(correctAnswer), difficulty(difficulty), points(points){}

Question::~Question(){
  delete answer;
}

int Question::getDifficulty() const{
  return difficulty;
}

double Question::getMaxPoints() const{
  return points;
}

void Question::setDifficutly(int d){
  difficulty = d;
}

void Question::setMaxPoints(double p){
  points = p;
}

//------------------QSingleChoice-implementation-------------------------------
QSingleChoice::QSingleChoice(const std::string & question, const std::vector<std::string> & choices, Answer * correctAnswer, int difficulty, double points)
: Question(question, correctAnswer, difficulty, points), choices(choices){}

QSingleChoice::~QSingleChoice(){}

std::vector<std::string> QSingleChoice::getQuestion() const{
  std::vector<std::string> out;
  out.push_back(question);
  for(auto it : choices){
    out.push_back(it);
  }

  return out;
}

std::string QSingleChoice::getQuestionType() const{
  string questionType = "QSingleChoice";
  return questionType;
}

std::vector<std::string> QSingleChoice::getQuestionExport() const{
  vector<string> out;
  out.push_back("QSingleChoice");
  out.push_back(question);
  out.push_back(to_string(difficulty));
  out.push_back(to_string(points));
  out.push_back(to_string(choices.size()));
  for(auto it : choices){
    out.push_back(it);
  }

  vector<string> answ = answer->getAnswerExport();
  out.insert(out.end(), answ.begin(), answ.end());

  return out;
}

double QSingleChoice::getPoints(const UserAnswer & userAnswer) const{
  if (answer->evaluate(userAnswer) == 1)
    return points;

  return 0;
}

//------------------QMultipleChoice-implementation-----------------------------
QMultipleChoice::QMultipleChoice(const std::string & question, const std::vector<std::string> & choices, Answer * correctAnswer, int difficulty, double points)
: Question(question, correctAnswer, difficulty, points), choices(choices){}

QMultipleChoice::~QMultipleChoice(){}

std::vector<std::string> QMultipleChoice::getQuestion() const{
  std::vector<std::string> out;
  out.push_back(question);
  for(auto it : choices){
    out.push_back(it);
  }

  return out;
}

std::string QMultipleChoice::getQuestionType() const{
  string questionType = "QMultipleChoice";
  return questionType;
}

std::vector<std::string> QMultipleChoice::getQuestionExport() const{
  vector<string> out;
  out.push_back("QMultipleChoice");
  out.push_back(question);
  out.push_back(to_string(difficulty));
  out.push_back(to_string(points));
  out.push_back(to_string(choices.size()));
  for(auto it : choices){
    out.push_back(it);
  }

  vector<string> answ = answer->getAnswerExport();
  out.insert(out.end(), answ.begin(), answ.end());

  return out;
}

double QMultipleChoice::getPoints(const UserAnswer & userAnswer) const{
  double correct = answer->evaluate(userAnswer);
  if (correct == 1)
    return points;
  if (correct >= 0.67)
    return points * 0.5;

  return 0;
}

//------------------QNumeric-implementation------------------------------------
QNumeric::QNumeric(const std::string & question, Answer * correctAnswer, int difficulty, double points)
: Question(question, correctAnswer, difficulty, points){}

QNumeric::~QNumeric(){}

std::vector<std::string> QNumeric::getQuestion() const{
  std::vector<std::string> out;
  out.push_back(question);

  return out;
}

std::string QNumeric::getQuestionType() const{
  string questionType = "QNumeric";
  return questionType;
}

std::vector<std::string> QNumeric::getQuestionExport() const{
  vector<string> out;
  out.push_back("QNumeric");
  out.push_back(question);
  out.push_back(to_string(difficulty));
  out.push_back(to_string(points));

  vector<string> answ = answer->getAnswerExport();
  out.insert(out.end(), answ.begin(), answ.end());

  return out;
}

double QNumeric::getPoints(const UserAnswer & userAnswer) const{
  return points * answer->evaluate(userAnswer);
}

//------------------QText-implementation---------------------------------------
QText::QText(const std::string & question, Answer * correctAnswer, int difficulty, double points)
: Question(question, correctAnswer, difficulty, points){}

QText::~QText(){}

std::vector<std::string> QText::getQuestion() const{
  std::vector<std::string> out;
  out.push_back(question);

  return out;
}

std::string QText::getQuestionType() const{
  string questionType = "QText";
  return questionType;
}

std::vector<std::string> QText::getQuestionExport() const{
  vector<string> out;
  out.push_back("QText");
  out.push_back(question);
  out.push_back(to_string(difficulty));
  out.push_back(to_string(points));

  vector<string> answ = answer->getAnswerExport();
  out.insert(out.end(), answ.begin(), answ.end());

  return out;
}

double QText::getPoints(const UserAnswer & userAnswer) const{
  return points * answer->evaluate(userAnswer);
}

//------------------QOrdering-implementation-----------------------------------
QOrdering::QOrdering(const std::string & question, const std::vector<std::string> & choices, Answer * correctAnswer, int difficulty, double points)
: Question(question, correctAnswer, difficulty, points), choices(choices){}

QOrdering::~QOrdering(){}

std::vector<std::string> QOrdering::getQuestion() const{
  std::vector<std::string> out;
  out.push_back(question);
  for(auto it : choices){
    out.push_back(it);
  }

  return out;
}

std::string QOrdering::getQuestionType() const{
  string questionType = "QOrdering";
  return questionType;
}

std::vector<std::string> QOrdering::getQuestionExport() const{
  vector<string> out;
  out.push_back("QOrdering");
  out.push_back(question);
  out.push_back(to_string(difficulty));
  out.push_back(to_string(points));
  out.push_back(to_string(choices.size()));
  for(auto it : choices){
    out.push_back(it);
  }

  vector<string> answ = answer->getAnswerExport();
  out.insert(out.end(), answ.begin(), answ.end());

  return out;
}

double QOrdering::getPoints(const UserAnswer & userAnswer) const{
  double correct = answer->evaluate(userAnswer);
  if (correct == 1)
    return points;
  if (correct >= 0.5)
    return points * 0.5;

  return 0;
}
