#include "quiz.h"
#include <math.h>
#include <ctime>
#include <algorithm>
#include <cstdlib>
using namespace std;

Quiz::Quiz(const std::string & title, const std::string & topic, int numberOfQuestions, const std::string order)
: questions(), title(title), topic(topic), numberOfQuestions(numberOfQuestions), order(order){}

Quiz::~Quiz(){
  for(int i = questions.size()-1; i >= 0; i--){
    delete questions[i];
  }
}

void Quiz::addQuestion(Question * question){
  questions.push_back(question);
}

void Quiz::setTitle(const std::string & tit){
  title = tit;
}

void Quiz::setTopic(const std::string & top){
  topic = top;
}

void Quiz::setNumberOfQuestions(int num){
  numberOfQuestions = num;
}

void Quiz::setOrder(const std::string & ord){
  order = ord;
}

const Question * Quiz::getQuestion(int pos) const{
  if (pos >= (int)questions.size())
    return NULL;

  return questions[pos];
}

int rng(int i){
  return rand()%i; // return number generated pseudo-randmoly
}

std::vector<int> Quiz::getQuestionsPos(std::vector<int> & questionPool, double successRate, int nQuestions) const{
  srand(unsigned(time(NULL))); // seed the pseudo-random number generator

  // if all the questions are shown in the quiz
  if(getNumberOfQuestions() == getMaxNumberOfQuestions()){
    // if the order is default, return the pool
    if (order == "default")
      return questionPool;
    else{
      // random shuffle vector, using custom rng function
      random_shuffle(questionPool.begin(), questionPool.end(), rng);
      return questionPool;
    }
  }

  if (order == "random"){
    // random shuffle vector, using custom rng function
    random_shuffle(questionPool.begin(), questionPool.end(), rng);
    auto itFirst = questionPool.begin();
    auto itLast = questionPool.begin() + getNumberOfQuestions();
    vector<int> out(itFirst, itLast);
    return out;
  }

  // if none of the previous conditions are met the order is based on difficulty
  vector<int> out;
  if (successRate == -1){ // if no questions were answered yet
    // choose one easy question
    out.push_back(getClosestQuestionPos(questionPool, 1));
    // choose one medium difficulty question
    out.push_back(getClosestQuestionPos(questionPool, 5));
    // choose one hard question
    out.push_back(getClosestQuestionPos(questionPool, 10));
    return out;
  }

  // if the successRate is 0.5 show questions with 5 difficulty etc.
  int diff = successRate*10;
  for(int i = 0; i < nQuestions; i++){
    out.push_back(getClosestQuestionPos(questionPool, diff));
  }
  return out;
}

int Quiz::getClosestQuestionPos(std::vector<int> & questionPool, int diff) const{
  // set closest to first question pos in the pool
  const Question * question = getQuestion(questionPool[0]);
  int closest = questionPool[0];
  int closestDiff = question->getDifficulty();
  int closestIndex = 0;
  int currentDiff;
  // iterate through the pool of questions
  for(size_t i = 0; i < questionPool.size(); i++){
    question = getQuestion(questionPool[i]);
    currentDiff = question->getDifficulty();
    // if the current questions difficulty exuals the difficulty we're looking for
    if (currentDiff == diff){
      closest = questionPool[i];
      closestIndex = i;
      break;
    }
    // if the current questions difficulty is closer than the current closest
    if (abs(diff - currentDiff) < abs(diff - closestDiff)){
      closest = questionPool[i];
      closestDiff = currentDiff;
      closestIndex = i;
    }
  }
  // erase the question's index from the pool so the question is not shown twice
  questionPool.erase(questionPool.begin()+closestIndex);
  return closest;
}

std::string Quiz::getTitle() const{
  return title;
}

std::string Quiz::getTopic() const{
  return topic;
}

int Quiz::getNumberOfQuestions() const{
  return numberOfQuestions;
}

int Quiz::getMaxNumberOfQuestions() const{
  return (int)questions.size();
}

double Quiz::getAverageDifficulty() const{
  double total = 0;
  // get all question difficulties
  for(size_t i = 0; i < questions.size(); i++){
    total += questions[i]->getDifficulty();
  }
  // return total with two decimal places
  return total/questions.size();
}


std::string Quiz::getOrder() const{
  return order;
}
