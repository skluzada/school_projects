#ifndef __handler_h__Quizzes_skluzada__
#define __handler_h__Quizzes_skluzada__
#include "quiz.h"
#include <string>
#include <vector>
#include <map>
#include <fstream>

/** Default directory path that the handler uses to import and export quizzes */
const std::string DefaultPath = "examples/";

/**
* Class QuizHandler is used to store, import and export quizzes
*/
class QuizHandler{
public:
  /**
  * Constructor that sets the path member to the value of DefaultPath
  */
  QuizHandler();
  /**
  * Constructor that delets all the quizzes in the handler
  */
  ~QuizHandler();
  /**
  * Method to add a Quiz into the handler
  *
  * @param[in] quiz Quiz pointer that will be added to the handler
  */
  void addQuiz(Quiz * quiz);
  /**
  * Method that decides whether the passed title is already in the handler or not
  *
  * @param[in] title Title that we're looking for
  *
  * @return True if the title is in the handler, otherwise return false
  */
  bool isUniqueTitle(const std::string & title);
  /**
  * Method to get all the quiz's titles in the handler
  *
  * @return Vector that contains all the titles of the quizzes in the handler
  */
  std::vector<std::string> getTitles() const;
  /**
  * Method to get all the quizzes that can be imported from the DirectoryPath
  *
  * @return Vector of all the titles of the quizzes in the DirectoryPath
  *
  * Every exported quiz is saved in format: title_quiz.txt, so only files that fits this pattern are in the vector
  */
  std::vector<std::string> getTitlesInPath() const;
  /**
  * Method to get a quiz pointer with the passed title
  *
  * @param[in] title Title that we're looking for
  *
  * @return Pointer to a quiz
  */
  const Quiz* getQuizPtr(const std::string & title) const;
  /**
  * Method to set the DirectoryPath used by the handler to import and export quizzes
  *
  * @param[in] pat New DirectoryPath
  *
  * @return True if the directory exists and the program has the permission to create and delete a file there, else return false
  */
  bool setPath(std::string & pat);
  /**
  * Method to get the DirectoryPath used by the handler to import and export quizzes
  *
  * @return Handler's path that is used to import and export quizzes
  */
  std::string getPath() const;
  /**
  * Method to export quizzes
  *
  * @param[in] titles Vector of titles of the quizzes we want to export
  *
  * @return Number of quizzes that were succesfully exported
  */
  int exportQuizzes(const std::vector<std::string> & titles) const;
  /**
  * Method to import quizzes
  *
  * @param[in] titles Vector of titles of the quizzes we want to import
  *
  * @return Number of quizzes that were succesfully imported
  */
  int importQuizzes(const std::vector<std::string> & titles);
private:
  /** Quizzes that are currently in the handler and are availible to be solved or exported */
  std::map<std::string, Quiz*> quizzes;
  /** Directory Path that the handler uses to export or import quizzes */
  std::string path;
  /**
  * Method to export a quiz
  *
  * @param[in] title Title of the quiz that will be exported
  *
  * @return True if the quiz was succesfully exported, else return false
  */
  bool exportQuiz(const std::string & title) const;
  /**
  * Method to import a quiz
  *
  * @param[in] title Title of the quiz that will be imported
  *
  * @return True if the quiz was succesfully imported, else return false
  */
  bool importQuiz(const std::string & title);
  /**
  * Method to import a single choice question
  *
  * @param[in] quiz Quiz to which the question belongs
  * @param[in] inFile File we're importing the question from
  * @param[in] question Question's question text
  * @param[in] difficulty Question's difficulty
  * @param[in] points Question's points
  *
  * @return True if the question was succesfully imported, else return false
  *
  * The common question's parameters like question, difficulty and points are imported before, this method is used to import the specific parameters of the certain question type
  */
  bool importQSingleChoice(Quiz * quiz, std::ifstream & inFile, const std::string & question, int difficulty, double points) const;
  /**
  * Method to import a multiple choice question
  *
  * @param[in] quiz Quiz to which the question belongs
  * @param[in] inFile File we're importing the question from
  * @param[in] question Question's question text
  * @param[in] difficulty Question's difficulty
  * @param[in] points Question's points
  *
  * @return True if the question was succesfully imported, else return false
  *
  * The common question's parameters like question, difficulty and points are imported before, this method is used to import the specific parameters of the certain question type
  */
  bool importQMultipleChoice(Quiz * quiz, std::ifstream & inFile, const std::string & question, int difficulty, double points) const;
  /**
  * Method to import a text question
  *
  * @param[in] quiz Quiz to which the question belongs
  * @param[in] inFile File we're importing the question from
  * @param[in] question Question's question text
  * @param[in] difficulty Question's difficulty
  * @param[in] points Question's points
  *
  * @return True if the question was succesfully imported, else return false
  *
  * The common question's parameters like question, difficulty and points are imported before, this method is used to import the specific parameters of the certain question type
  */
  bool importQText(Quiz * quiz, std::ifstream & inFile, const std::string & question, int difficulty, double points) const;
  /**
  * Method to import a numeric question
  *
  * @param[in] quiz Quiz to which the question belongs
  * @param[in] inFile File we're importing the question from
  * @param[in] question Question's question text
  * @param[in] difficulty Question's difficulty
  * @param[in] points Question's points
  *
  * @return True if the question was succesfully imported, else return false
  *
  * The common question's parameters like question, difficulty and points are imported before, this method is used to import the specific parameters of the certain question type
  */
  bool importQNumeric(Quiz * quiz, std::ifstream & inFile, const std::string & question, int difficulty, double points) const;
  /**
  * Method to import an ordering question
  *
  * @param[in] quiz Quiz to which the question belongs
  * @param[in] inFile File we're importing the question from
  * @param[in] question Question's question text
  * @param[in] difficulty Question's difficulty
  * @param[in] points Question's points
  *
  * @return True if the question was succesfully imported, else return false
  *
  * The common question's parameters like question, difficulty and points are imported before, this method is used to import the specific parameters of the certain question type
  */
  bool importQOrdeing(Quiz * quiz, std::ifstream & inFile, const std::string & question, int difficulty, double points) const;
};







#endif //__handler_h__Quizzes_skluzada__
