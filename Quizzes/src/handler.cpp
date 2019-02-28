#include "handler.h"
#include <cstdio>
#include <cstdlib>
#include <dirent.h>
using namespace std;

QuizHandler::QuizHandler()
: quizzes(), path(DefaultPath){}

QuizHandler::~QuizHandler(){
  // iterater through the map and delete all the quizzes
  for(auto it : quizzes){
    delete it.second;
  }
}

void QuizHandler::addQuiz(Quiz * quiz){
  // the insertion won't fail, because the uniqueness of the title is checked in createQuiz function
  quizzes.insert(std::make_pair(quiz->getTitle(), quiz));
}

bool QuizHandler::isUniqueTitle(const std::string & title){
  auto it = quizzes.find(title);
  // if the find function returns end, the title is not in the map
  if (it == quizzes.end()){
    return 1;
  }
  return 0;
}

std::vector<std::string> QuizHandler::getTitles() const{
  vector<string> out;
  // iterater through the map and add the titles to the vector
  for(auto it : quizzes){
    out.push_back(it.first);
  }
  return out;
}

std::vector<std::string> QuizHandler::getTitlesInPath() const{
  vector<string> titles;
  string title;
  // go through all the files in the path, insert only thos ending with _quiz.txt
  DIR * dir;
  struct dirent * ent;
  if ((dir = opendir(path.c_str())) != NULL){
    while((ent = readdir(dir)) != NULL){
      title = ent->d_name;
      if (title.size() < 10)
        continue;
      if (title.substr(title.size()-9) == "_quiz.txt")
        titles.push_back(title.substr(0, title.size()-9));
    }
  }
  closedir(dir);
  return titles;
}

const Quiz* QuizHandler::getQuizPtr(const std::string & title) const{
  auto it = quizzes.find(title);
  // if the find function returns end, the title is not in the map
  if (it == quizzes.end()){
    return NULL;
  }
  return it->second;
}

bool QuizHandler::setPath(std::string & pat){
  // if path doesn't end with a slash
  if (pat[pat.size()-1] != '/')
    pat+='/';

  // test if it's possible to fopen() and remove() file in the given path
  string file_name = pat + "test_file_quiz.txt";
  FILE *test_file = fopen(file_name.c_str(), "a+");
  if (test_file == NULL)
    return false;

  if (remove(file_name.c_str()) != 0)
    return false;

  path = pat;
  return true;
}

std::string QuizHandler::getPath() const{
  return path;
}

bool QuizHandler::exportQuiz(const std::string & title) const{
  const Quiz * quiz = getQuizPtr(title);
  if (quiz == NULL)
    return false;

  ofstream outFile;
  string outFileName = path + title + "_quiz.txt";
  outFile.open(outFileName.c_str(), ofstream::trunc);

  if (!outFile.good())
    return false;

  // print quiz parameters first
  outFile << quiz->getTitle() << endl;
  outFile << quiz->getTopic() << endl;
  outFile << to_string(quiz->getNumberOfQuestions()) << endl;
  outFile << quiz->getOrder() << endl;
  outFile << to_string(quiz->getMaxNumberOfQuestions()) << endl;

  // then print all questions with answers
  vector<string> question;
  for(int i = 0; i < quiz->getMaxNumberOfQuestions(); i++){
    question = quiz->getQuestion(i)->getQuestionExport();
    for(int j = 0; j < (int)question.size(); j++){
      outFile << question[j] << endl;
    }
  }
  outFile.close();
  return true;
}

int QuizHandler::exportQuizzes(const std::vector<std::string> & titles) const{
  int exported = 0;
  for(size_t i = 0; i < titles.size(); i++){
    exported += exportQuiz(titles[i]);
  }
  return exported;
}

bool QuizHandler::importQuiz(const std::string & title){
  // if quiz with the same title is already in the handler
  if (getQuizPtr(title) != NULL)
    return false;

  ifstream inFile;
  string inFileName = path + title + "_quiz.txt";
  inFile.open(inFileName.c_str());

  if (!inFile.good())
    return false;

  string titleIn;
  getline(inFile, titleIn);
  if (!titleIn.size() || titleIn != title)
    return false;

  string topic;
  getline(inFile, topic);
  if (!topic.size())
    return false;

  string numberOfQuestions_string;
  getline(inFile, numberOfQuestions_string);
  int numberOfQuestions = atoi(numberOfQuestions_string.c_str());
  if (numberOfQuestions == 0) // if atoi() failed
    return false;

  string order;
  getline(inFile, order);
  if (!order.size())
    return false;

  string maxNumberOfQuestions_string;
  getline(inFile, maxNumberOfQuestions_string);
  int maxNumberOfQuestions = atoi(maxNumberOfQuestions_string.c_str());
  if (maxNumberOfQuestions == 0) // if atoi() failed
    return false;

  Quiz * quiz = new Quiz(title, topic, numberOfQuestions, order);

  // read question type and then call the appropriate function to import question
  string questionType;
  for(int i = 0; i < maxNumberOfQuestions; i++){
    getline(inFile, questionType);

    // get the parameters that are common for all the questions
    string question;
    getline(inFile, question);
    if (!question.size()){
      delete quiz;
      return false;
    }

    string difficulty_string;
    getline(inFile, difficulty_string);
    int difficulty = atoi(difficulty_string.c_str());
    if (difficulty > 10 || difficulty < 0){
      delete quiz;
      return false;
    }

    string points_string;
    getline(inFile, points_string);
    double points = atof(points_string.c_str());
    if (points == 0){ // if atof() failed
      delete quiz;
      return false;
    }

    if (questionType == "QSingleChoice"){
      if (!importQSingleChoice(quiz, inFile, question, difficulty, points)){
        delete quiz;
        return false;
      }
    }
    else if (questionType == "QMultipleChoice"){
      if (!importQMultipleChoice(quiz, inFile, question, difficulty, points)){
        delete quiz;
        return false;
      }
    }
    else if (questionType == "QText"){
      if (!importQText(quiz, inFile, question, difficulty, points)){
        delete quiz;
        return false;
      }
    }
    else if (questionType == "QNumeric"){
      if (!importQNumeric(quiz, inFile, question, difficulty, points)){
        delete quiz;
        return false;
      }
    }
    else if (questionType == "QOrdering"){
      if (!importQOrdeing(quiz, inFile, question, difficulty, points)){
        delete quiz;
        return false;
      }
    }
    else{
      delete quiz;
      return false;
    }
  }

  addQuiz(quiz);
  return true;
}

int QuizHandler::importQuizzes(const std::vector<std::string> & titles){
  int imported = 0;
  for(size_t i = 0; i < titles.size(); i++){
    imported += importQuiz(titles[i]);
  }
  return imported;
}

bool QuizHandler::importQSingleChoice(Quiz * quiz, ifstream & inFile, const std::string & question, int difficulty, double points) const{
  string nChoices_string;
  getline(inFile, nChoices_string);
  int nChoices = atoi(nChoices_string.c_str());
  if (nChoices == 0) // if atoi() fails
    return false;

  vector<string> choices;
  string choice;
  for(int i = 0; i < nChoices; i++){
    getline(inFile, choice);
    choices.push_back(choice);
  }

  string answerType;
  getline(inFile, answerType);
  if (answerType != "AChoices")
    return false;

  bool trueSet = 0;
  vector<int> answers;
  string answ;
  for(int i = 0; i < nChoices; i++){
    getline(inFile, answ);
    if (answ == "0"){
      answers.push_back(0);
    }
    else if (answ == "1"){
      answers.push_back(1);
      if (trueSet) // if there are 2 correct answers in single choice question
        return false;
      trueSet = true;
    }
    else { // if an answer is not 0 or 1
      return false;
    }
  }

  if (!trueSet) // if there was no correct answer
    return false;

  Answer * answer = new AChoices(answers);
  Question * quest = new QSingleChoice(question, choices, answer, difficulty, points);
  quiz->addQuestion(quest);
  return true;
}

bool QuizHandler::importQMultipleChoice(Quiz * quiz, ifstream & inFile, const std::string & question, int difficulty, double points) const{
  string nChoices_string;
  getline(inFile, nChoices_string);
  int nChoices = atoi(nChoices_string.c_str());
  if (nChoices == 0) // if atoi() fails
    return false;

  vector<string> choices;
  string choice;
  for(int i = 0; i < nChoices; i++){
    getline(inFile, choice);
    choices.push_back(choice);
  }

  string answerType;
  getline(inFile, answerType);
  if (answerType != "AChoices")
    return false;

  vector<int> answers;
  string answ;
  for(int i = 0; i < nChoices; i++){
    getline(inFile, answ);
    if (answ == "0"){
      answers.push_back(0);
    }
    else if (answ == "1"){
      answers.push_back(1);
    }
    else { // if an answer is not 0 or 1
      return false;
    }
  }

  Answer * answer = new AChoices(answers);
  Question * quest = new QMultipleChoice(question, choices, answer, difficulty, points);
  quiz->addQuestion(quest);
  return true;
}

bool QuizHandler::importQText(Quiz * quiz, ifstream & inFile, const std::string & question, int difficulty, double points) const{
  string answerType;
  getline(inFile, answerType);
  if (answerType != "AText")
    return false;

  string nAnswers_string;
  getline(inFile, nAnswers_string);
  int nAnswers = atoi(nAnswers_string.c_str());
  if (nAnswers == 0) // if atoi() fails
    return false;

  vector<string> answers;
  string answ;
  for(int i = 0; i < nAnswers; i++){
    getline(inFile, answ);
    if (!answ.size())
      return false;
    answers.push_back(answ);
  }

  Answer * answer = new AText(answers);
  Question * quest = new QText(question, answer, difficulty, points);
  quiz->addQuestion(quest);
  return true;
}

bool QuizHandler::importQNumeric(Quiz * quiz, ifstream & inFile, const std::string & question, int difficulty, double points) const{
  string answerType;
  getline(inFile, answerType);

  Answer * answer = NULL;
  if (answerType == "ANumber"){
    string number_string;
    getline(inFile, number_string);
    double number = atof(number_string.c_str());
    answer = new ANumber(number);
  }
  else if (answerType == "AInterval"){
    string leftBound_string;
    getline(inFile, leftBound_string);
    double leftBound = atof(leftBound_string.c_str());

    string rightBound_string;
    getline(inFile, rightBound_string);
    double rightBound = atof(rightBound_string.c_str());

    if (leftBound > rightBound)
      return false;

    string leftBounded_string;
    getline(inFile, leftBounded_string);
    bool leftBounded;
    if (leftBounded_string == "0")
      leftBounded = false;
    else if (leftBounded_string == "1")
      leftBounded = true;
    else
      return false;

    string rightBounded_string;
    getline(inFile, rightBounded_string);
    bool rightBounded;
    if (rightBounded_string == "0")
      rightBounded = false;
    else if (rightBounded_string == "1")
      rightBounded = true;
    else
      return false;

    answer = new AInterval(leftBound, rightBound, leftBounded, rightBounded);
  }
  else if (answerType == "AResidueClass"){
    string modulus_string;
    getline(inFile, modulus_string);
    int modulus = atoi(modulus_string.c_str());
    if (modulus == 0) // if atoi() fails
      return false;
    answer = new AResidueClass(modulus);
  }
  else {
    return false;
  }

  Question * quest = new QNumeric(question, answer, difficulty, points);
  quiz->addQuestion(quest);
  return true;
}

bool QuizHandler::importQOrdeing(Quiz * quiz, ifstream & inFile, const std::string & question, int difficulty, double points) const{
  string nChoices_string;
  getline(inFile, nChoices_string);
  int nChoices = atoi(nChoices_string.c_str());
  if (nChoices == 0) // if atoi() fails
    return false;

  vector<string> choices;
  string choice;
  for(int i = 0; i < nChoices; i++){
    getline(inFile, choice);
    choices.push_back(choice);
  }

  string answerType;
  getline(inFile, answerType);
  if (answerType != "AOrder")
    return false;

  vector<int> order;
  string answ;
  int pos;
  for(int i = 0; i < nChoices; i++){
    getline(inFile, answ);
    pos = atoi(answ.c_str());
    if (pos > nChoices || pos < 0)
      return false;
    order.push_back(pos);
  }

  Answer * answer = new AOrder(order);
  Question * quest = new QOrdering(question, choices, answer, difficulty, points);
  quiz->addQuestion(quest);
  return true;
}
