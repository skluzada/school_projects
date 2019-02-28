#ifndef __quiz_h__Quizzes_skluzada__
#define __quiz_h__Quizzes_skluzada__
#include "question.h"
#include <vector>
#include <string>


const int questionsPerPage = 3;

/** Auxiliary function that act as a pseudo-random number generator*/
int rng(int i);

/**
* Class Quiz represent a quiz of multiple questions
*/
class Quiz{
public:
  /**
  * Constructor
  *
  * @param[in] title Title of the quiz (is unique)
  * @param[in] topic Topic of the quiz
  * @param[in] numberOfQuestions Number of questions shown in each quiz
  * @param[in] order Specify how are the quetions selected
  */
  Quiz(const std::string & title, const std::string & topic, int numberOfQuestions = 0, const std::string order = "");
  /**
  * Destructor deletes all the questions
  */
  ~Quiz();
  /**
  * Method to add a questions to the quiz
  *
  * @param[in] questions Question pointer that is added to the quiz's question vector
  */
  void addQuestion(Question * question);
  /**
  * Method to change the title of the quiz
  *
  * @param[in] tit New quiz's title
  */
  void setTitle(const std::string & tit);
  /**
  * Method to change the topic of the quiz
  *
  * @param[in] top New quiz's topic
  */
  void setTopic(const std::string & top);
  /**
  * Method to change the number of questions in each quiz
  *
  * @param[in] num New number of questions in each quiz
  */
  void setNumberOfQuestions(int num);
  /**
  * Method to change the quiz's order
  *
  * @param[in] ord New number of questions in each quiz
  */
  void setOrder(const std::string & ord);
  /**
  * Method that returns a pointer to a question at the given position
  *
  * @param[in] pos Position of the question
  *
  * @return Pointer to a question at the given position or NULL if the position is greater than number of questions
  */
  const Question * getQuestion(int pos) const;
  /**
  * Method used by the UI to know what questions and in what order it should show
  *
  * @param[in] questionPool Question indexes that are in the question pool (were not shown yet)
  * @param[in] successRate What percentage of the questions the user has answered correctly
  * @param[in] nQuestions Specify the number of question in each part of the quiz if the quiz is separated to parts
  *
  * @return Vector of question's positions in the quiz's vector of questions
  */
  std::vector<int> getQuestionsPos(std::vector<int> & questionPool, double successRate = -1, int nQuestions = questionsPerPage) const;
  /**
  * Method to get a question with the closest difficulty to the difficulty passed to this function
  *
  * @param[in] questionPool Question indexes that are in the question pool
  * @param[in] diff Difficulty that we're looking for
  *
  * @return Position of the question with the closest difficulty to the difficulty passed to this function
  */
  int getClosestQuestionPos(std::vector<int> & questionPool, int diff) const;
  /**
  * Method to get the quiz's title
  *
  * @return Quiz title
  */
  std::string getTitle() const;
  /**
  * Method to get the quiz's topic
  *
  * @return Quiz topic
  */
  std::string getTopic() const;
  /**
  * Method to get the number of questions shown in each quiz
  *
  * @return Number of questions shown in each quiz
  */
  int getNumberOfQuestions() const;
  /**
  * Method to get the number of questions in the quiz's pool
  *
  * @return Number of questions in the quiz's pool
  */
  int getMaxNumberOfQuestions() const;
  /**
  * Method to get the average difficulty of the quiz's questions
  *
  * @return Average difficulty of the quiz's questions
  */
  double getAverageDifficulty() const;
  /**
  * Method to get the quiz's order
  *
  * @return Quiz order
  */
  std::string getOrder() const;
private:
  /** Quiz's questions pool */
  std::vector<Question*> questions;
  /** Quiz's title */
  std::string title;
  /** Quiz's topic */
  std::string topic;
  /** Number of questions shown in each quiz */
  int numberOfQuestions;
  /** Order that specify which questions and in what order they are shown in the quiz */
  std::string order;
};

#endif //__quiz_h__Quizzes_skluzada__
