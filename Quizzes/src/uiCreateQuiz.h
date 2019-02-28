#ifndef __UICreateQuiz_h__Quizzes_skluzada__
#define __UICreateQuiz_h__Quizzes_skluzada__
#include "handler.h"
#include "uiPrinter.h"
#include <ncurses.h>
#include <string>
#include <vector>
#include <functional>

/**
* Class UICreateQuiz is allows user to create quizzes using ncruses ui
*/
class UICreateQuiz : public UIPrinter{
public:
  /**
  * Constructor
  *
  * @param[in] handler Quiz Handler that is used by the UICreateQuiz
  */
  UICreateQuiz(QuizHandler & handler);
  /**
  * Method to show the UI to create a quiz
  */
  void show();
  /**
  * Method that shows a window where title and topic of the quiz is inputed by the user
  */
  void createQuiz();
  /**
  * Method that shows a menu from where new questions can be added to the quiz or the quiz can be finished
  */
  void createQuizMenu();
  /**
  * Method that shows a window where the user chooses what type of question he wants to create
  */
  void selectQuestion();
  /**
  * Method that shows a window where the question's text is inputed by the user
  *
  * @param[in] createAnswerFunction Function where the user will create an answer appropriate to the question type
  * @param[in] input Input shown in the input box
  */
  void createQuestion(std::function<void(std::string&)> createAnswerFunction, const std::vector<std::string> & input = {""});
  /**
  * Method that shows a window where the question's difficulty and points are inputed by the user
  *
  * @param[in] question Pointer to a question which is currently beeing created
  */
  void finishQuestion(Question * question);
  /**
  * Method that shows a window where the user creates an answer for a single choice question
  *
  * @param[in] question Question's text
  */
  void createASingleChoice(std::string & question);
  /**
  * Method that shows a window where the user creates an answer for a multiple choice question
  *
  * @param[in] question Question's text
  */
  void createAMultipleChoice(std::string & question);
  /**
  * Method that shows a window where the user creates choices
  *
  * @param[in] question Question's text
  * @param[in] singleChoice True if single choice, false if multiple choice or ordering question
  * @param[in] ordering True if ordering question, false if single or multiple choice question
  */
  void createAChoices(std::string & question, int singleChoice, int ordering = 0);
  /**
  * Method that shows a window where the user choose which choices are correct
  *
  * @param[in] question Question's text
  * @param[in] singleChoice True if single choice, false if multiple choice
  * @param[in] choices Vector of choices
  */
  void createAChoicesChooseCorrect(std::string & question, int singleChoice, std::vector<std::string> & choices);
  /**
  * Method that shows a window where the user creates an answer for an order question
  *
  * @param[in] question Question's text
  */
  void createAOrder(std::string & question);
  /**
  * Method that shows a window where the user choose the correct order
  *
  * @param[in] question Question's text
  * @param[in] choices Vector of choices
  */
  void createAOrderChooseOrder(std::string & question, std::vector<std::string> & choices);
  /**
  * Method that shows a window where the user chooses the type of numeric answer
  *
  * @param[in] question Question's text
  */
  void createANumeric(std::string & question);
  /**
  * Method that shows a window where the user creates a number answer
  *
  * @param[in] question Question's text
  */
  void createANumber(std::string & question);
  /**
  * Method that shows a window where the user creates an interval answer
  *
  * @param[in] question Question's text
  */
  void createAInterval(std::string & question);
  /**
  * Method that shows a window where the user creates a residue class answer
  *
  * @param[in] question Question's text
  */
  void createAResidueClass(std::string & question);
  /**
  * Method that shows a window where the user creates a text answer
  *
  * @param[in] question Question's text
  */
  void createAText(std::string & question);
  /**
  * Method that shows a window where the user specifies the number of questions in each quiz
  */
  void chooseNumberOfQuestions();
  /**
  * Method that shows a window where the user specifies the order or the way in which the questions are shown in the quiz
  *
  * @param[in] maxQuestions Specifies whether all the questions in the quiz's pool are shown in each quiz
  */
  void chooseQuestionsOrder(int maxQuestions);
private:
  /** Pointer to the Quiz Handler that is used by the UICreateQuiz to save quizzes */
  QuizHandler * handlerPtr;
  /** Pointer to the Quiz that is beeing created */
  Quiz * quiz;
};

#endif //__UICreateQuiz_h__Quizzes_skluzada__
