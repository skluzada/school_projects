#ifndef __answer_h__Quizzes_skluzada__
#define __answer_h__Quizzes_skluzada__
#include <vector>
#include <string>

/**
* Auxiliary structure that represent the user's answer
* User's answer can be string, double or a vector of chosen options
*/
struct UserAnswer{
  std::string text;
  double number;
  std::vector<int> vec;
};


/**
* Abstract class Answer
*/
class Answer{
public:
  /**
  * Default constructor
  */
  Answer();
  /**
  * Virtual default destructor
  */
  virtual ~Answer();
  /**
  * Method that evaluates the user's answer
  *
  * @param[in] userAnswer user's answer
  *
  * @return Ratio of how much of the answer is correct
  */
  virtual double evaluate(const UserAnswer & userAnswer) const = 0;
  /**
  * Method that is used to export the answer
  *
  * @return Vector that contains all the answer's parameters
  */
  virtual std::vector<std::string> getAnswerExport() const = 0;
};

/**
* Abstract class ANumeric, all the derived Answers are numeric
*/
class ANumeric : public Answer{
public:
  /**
  * Default constructor
  */
  ANumeric();
  /**
  * Virtual default destructor
  */
  virtual ~ANumeric();
  virtual double evaluate(const UserAnswer & userAnswer) const = 0;
  virtual std::vector<std::string> getAnswerExport() const = 0;
};

/**
* Class ANumber represent answer as a number
*/
class ANumber : public ANumeric{
public:
  /**
  * Constructor
  *
  * @param[in] number Number that represent the correct answer
  */
  ANumber(double number);
  /**
  * Virtual default destructor
  */
  virtual ~ANumber();
  virtual double evaluate(const UserAnswer & userAnswer) const;
  virtual std::vector<std::string> getAnswerExport() const;
private:
  /** Correct number */
  double number;
};

/**
* Class AInterval represent answer as all numbers from an interval
*/
class AInterval : public ANumeric{
public:
  /**
  * Constructor
  *
  * @param[in] leftBound Left bound of the interval
  * @param[in] rightBound Right bound of the interval
  * @param[in] leftBounded Specify whether the interval is left bounded
  * @param[in] rightBounded Specify whether the interval is right bounded
  */
  AInterval(double leftBound, double rightBound, bool leftBounded, bool rightBounded);
  /**
  * Virtual default destructor
  */
  virtual ~AInterval();
  virtual double evaluate(const UserAnswer & userAnswer) const;
  virtual std::vector<std::string> getAnswerExport() const;
private:
  /** Left bound of the interval */
  double leftBound;
  /** Right bound of the interval */
  double rightBound;
  /** Specify whether the interval is left bounded or not */
  bool leftBounded;
  /** Specify whether the interval is right bounded or not */
  bool rightBounded;
};

/**
* Class AResidueClass represent answer as all numbers from a residue class
*/
class AResidueClass : public ANumeric{
public:
  /**
  * Constructor
  *
  * @param[in] modulus Number that defines the residue class containing the correct answers
  */
  AResidueClass(int modulus);
  /**
  * Virtual default destructor
  */
  virtual ~AResidueClass();
  virtual double evaluate(const UserAnswer & userAnswer) const;
  virtual std::vector<std::string> getAnswerExport() const;
private:
  /** Modulus that defines the residue class */
  int modulus;
};

/**
* Class AChoices represent answer as correct choices
*/
class AChoices : public Answer{
public:
  /**
  * Constructor
  *
  * @param[in] correctAnswers Specify what choices are true and what are false
  */
  AChoices(const std::vector<int> & correctAnswers);
  /**
  * Virtual default destructor
  */
  virtual ~AChoices();
  virtual double evaluate(const UserAnswer & userAnswer) const;
  virtual std::vector<std::string> getAnswerExport() const;
private:
  /** Vector that specify which choices are correct and which are false */
  std::vector<int> answers;
};

/**
* Class AText represent answer as correct texts
*/
class AText : public Answer{
public:
  /**
  * Constructor
  *
  * @param[in] correctAnswers Strings that are correct
  */
  AText(const std::vector<std::string> & correctAnswers);
  /**
  * Virtual default destructor
  */
  virtual ~AText();
  virtual double evaluate(const UserAnswer & userAnswer) const;
  virtual std::vector<std::string> getAnswerExport() const;
private:
  /** Vector of correct text answers */
  std::vector<std::string> answers;
};

/**
* Class AOrder represent answer as a correct order
*/
class AOrder : public Answer{
public:
  /**
  * Constructor
  *
  * @param[in] correctOrder Specify what is the correct order
  */
  AOrder(const std::vector<int> correctOrder);
  /**
  * Virtual default destructor
  */
  virtual ~AOrder();
  virtual double evaluate(const UserAnswer & userAnswer) const;
  virtual std::vector<std::string> getAnswerExport() const;
private:
  /** Vector that specifies the correct order of the options */
  std::vector<int> order;
};

#endif // __answer_h__Quizzes_skluzada__
