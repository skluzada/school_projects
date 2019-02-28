#ifndef __question_h__Quizzes_skluzada__
#define __question_h__Quizzes_skluzada__
#include "answer.h"
#include <string>
#include <vector>

class Question{
public:
  /**
  * Constructor
  *
  * @param[in] question Text of the question
  * @param[in] correctAnswer Correct answer
  * @param[in] difficulty Question's difficulty
  * @param[in] points Number of points when the question is answered correctly
  */
  Question(const std::string & question, Answer * correctAnswer, int difficulty = 0, double points = 1);
  /**
  * Virtual destructor deletes answer
  */
  virtual ~Question();
  /**
  * @return Question's difficulty
  */
  int getDifficulty() const;
  /**
  * @return Number of points when the question is answered correctly
  */
  double getMaxPoints() const;
  /**
  * @param[in] d New difficulty value
  */
  void setDifficutly(int d);
  /**
  * @param[in] p New points value
  */
  void setMaxPoints(double p);
  /**
  * Method that returns the question's elements
  *
  * @return Vector that contains all the question's elements
  */
  virtual std::vector<std::string> getQuestion() const = 0;
  /**
  * Method that returns the question's type
  *
  * @return Question's type
  */
  virtual std::string getQuestionType() const = 0;
  /**
  * Method that is used to export the question
  *
  * @return Vector that contains all the question's elements needed to export the question
  */
  virtual std::vector<std::string> getQuestionExport() const = 0;
  /**
  * Method that returns number of points gained for the user's answer
  *
  * @return Number of points gained for the user's answer
  */
  virtual double getPoints(const UserAnswer & userAnswer) const = 0;
protected:
  /** Question's text */
  std::string question;
  /** Correct answer */
  Answer * answer;
  /** Question's difficulty */
  int difficulty;
  /** Points gained for answering the question correctly */
  double points;
};

/**
* Class QSingleChoice represents a question where exactly one choice is correct
*/
class QSingleChoice : public Question{
public:
  QSingleChoice(const std::string & question, const std::vector<std::string> & choices, Answer * correctAnswer = NULL, int difficulty = 0, double points = 1);
  /**
  * Virtual default destructor
  */
  virtual ~QSingleChoice();
  virtual std::vector<std::string> getQuestion() const;
  virtual std::string getQuestionType() const;
  virtual std::vector<std::string> getQuestionExport() const;
  /**
  * Method that returns number of points gained for the user's answer
  *
  * @return If the answer is correct return max number of points, else return 0
  */
  virtual double getPoints(const UserAnswer & userAnswer) const;
private:
  /** Options to choose from */
  std::vector<std::string> choices;
};

/**
* Class QMultipleChoice represents a question where any number of choices are correct
*/
class QMultipleChoice : public Question{
public:
  QMultipleChoice(const std::string & question, const std::vector<std::string> & choices, Answer * correctAnswer = NULL, int difficulty = 0, double points = 1);
  /**
  * Virtual default destructor
  */
  virtual ~QMultipleChoice();
  virtual std::vector<std::string> getQuestion() const;
  virtual std::string getQuestionType() const;
  virtual std::vector<std::string> getQuestionExport() const;
  /**
  * Method that returns number of points gained for the user's answer
  *
  * @return If the answer is correct return max number of points, else if the answer is at least 67% correct return 50% of the points, else return 0
  */
  virtual double getPoints(const UserAnswer & userAnswer) const;
private:
  /** Options to choose from */
  std::vector<std::string> choices;
};

/**
* Class QNumeric represents a question that is answered by a number
*/
class QNumeric : public Question{
public:
  QNumeric(const std::string & question, Answer * correctAnswer = NULL, int difficulty = 0, double points = 1);
  /**
  * Virtual default destructor
  */
  virtual ~QNumeric();
  virtual std::vector<std::string> getQuestion() const;
  virtual std::string getQuestionType() const;
  virtual std::vector<std::string> getQuestionExport() const;
  /**
  * Method that returns number of points gained for the user's answer
  *
  * @return If the numeric answer is correct return max number of points, else return 0
  */
  virtual double getPoints(const UserAnswer & userAnswer) const;
};

/**
* Class QText represents a question that is answered by a text
*/
class QText : public Question{
public:
  QText(const std::string & question, Answer * correctAnswer = NULL, int difficulty = 0, double points = 1);
  /**
  * Virtual default destructor
  */
  virtual ~QText();
  virtual std::vector<std::string> getQuestion() const;
  virtual std::string getQuestionType() const;
  virtual std::vector<std::string> getQuestionExport() const;
  /**
  * Method that returns number of points gained for the user's answer
  *
  * @return If the text answer is correct return max number of points, else return 0
  */
  virtual double getPoints(const UserAnswer & userAnswer) const;
};

/**
* Class QOrder represents a question where user has to order the choices
*/
class QOrdering : public Question{
public:
  QOrdering(const std::string & question, const std::vector<std::string> & choices, Answer * correctAnswer = NULL, int difficulty = 0, double points = 1);
  /**
  * Virtual default destructor
  */
  virtual ~QOrdering();
  virtual std::vector<std::string> getQuestion() const;
  virtual std::string getQuestionType() const;
  virtual std::vector<std::string> getQuestionExport() const;
  /**
  * Method that returns number of points gained for the user's answer
  *
  * @return If the answer is correct return max number of points,, else if the order is at least 67% correct return 50% of the points else return 0
  */
  virtual double getPoints(const UserAnswer & userAnswer) const;
private:
  /** Options to order */
  std::vector<std::string> choices;
};

#endif // __question_h__Quizzes_skluzada__
