#ifndef __UISolveQuiz_h__Quizzes_skluzada__
#define __UISolveQuiz_h__Quizzes_skluzada__
#include "handler.h"
#include "uiPrinter.h"
#include <ncurses.h>
#include <string>
#include <vector>

class UISolveQuiz : public UIPrinter{
public:
  /**
  * Constructor
  *
  * @param[in] handler Quiz Handler where all the quizzes are stored
  */
  UISolveQuiz(QuizHandler & handler);
  /**
  * Method to show the UI to solve a quiz
  */
  void show();
  /**
  * Method to show the scrolling options where the user chooses the quiz he wants to solve
  */
  void chooseQuiz();
  /**
  * Method to show some basic informations about the chosen quiz
  *
  * @param[in] title Title of the chosen quiz
  */
  void overviewQuiz(const std::string & title);
  /**
  * Method where the result is showed
  */
  void solveQuiz(const Quiz * quiz);
  /**
  * Method that creates a solve question window and then call a function to show the question based on its type
  *
  * @param[in] question Pointer to the question that is beeing solved
  * @param[in] title Title fo the quiz that is beeing solved
  * @param[in] nQuesion Number of question that is beeing solved
  * @param[in] nQuestions Number of questions shown in the quiz
  * @param[in] nPage Number of page that is beeing showed (when the quiz is separated into pages)
  * @param[in] nPages Number of pages (when the quiz is separated into pages)
  *
  * @return Number of points gained
  */
  double solveQuestion(const Question * question, const std::string & title, int nQuestion, int nQuestions, int nPage = -1, int nPages = -1);
  /**
  * Method that shows a single choice question + get and evaluate user's answer
  *
  * @param[in] win Pointer to the window where the question will be showed
  * @param[in] question Pointer to the question that is beeing solved
  *
  * @return Number of points gained
  */
  double solveQSingleChoice(WINDOW * win, const Question * question);
  /**
  * Method that shows a multiple choice question + get and evaluate user's answer
  *
  * @param[in] win Pointer to the window where the question will be showed
  * @param[in] question Pointer to the question that is beeing solved
  *
  * @return Number of points gained
  */
  double solveQMultipleChoice(WINDOW * win, const Question * question);
  /**
  * Method that shows a numeric question + get and evaluate user's answer
  *
  * @param[in] win Pointer to the window where the question will be showed
  * @param[in] question Pointer to the question that is beeing solved
  *
  * @return Number of points gained
  */
  double solveQNumeric(WINDOW * win, const Question * question);
  /**
  * Method that shows a text question + get and evaluate user's answer
  *
  * @param[in] win Pointer to the window where the question will be showed
  * @param[in] question Pointer to the question that is beeing solved
  *
  * @return Number of points gained
  */
  double solveQText(WINDOW * win, const Question * question);
  /**
  * Method that shows an ordering question + get and evaluate user's answer
  *
  * @param[in] win Pointer to the window where the question will be showed
  * @param[in] question Pointer to the question that is beeing solved
  *
  * @return Number of points gained
  */
  double solveQOrdering(WINDOW * win, const Question * question);
private:
  /** Pointer to the Quiz Handler where all the quizzes are stored */
  QuizHandler * handlerPtr;
};

#endif //__UISolveQuiz_h__Quizzes_skluzada__
