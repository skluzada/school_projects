#ifndef __UI_h__Quizzes_skluzada__
#define __UI_h__Quizzes_skluzada__
#include "handler.h"
#include "uiPrinter.h"
#include "uiSolveQuiz.h"
#include "uiCreateQuiz.h"
#include "uiImportExport.h"
#include <ncurses.h>
#include <string>
#include <vector>

/** The minimal width of the source terminal */
const int Window_MinWidth = 80;
/** The minimal height of the source terminal */
const int Window_MinHeight = 24;

class UI : public UIPrinter{
public:
  /**
  * Constructor
  */
  UI();
  /**
  * Method that inicilaze ncurses, check if the terminal is not to small and then shows the Main Menu
  */
  void start();
  /**
  * Method that shows the main menu
  */
  void showMainMenu();
  /**
  * Method that shows the about page
  */
  void showAbout();
private:
  /** Quiz Handler where the quizzes used by the UI are stored */
  QuizHandler handler;
  /** solveQuizMenu is showed when the user select Solve Quiz option from the main menu */
  UISolveQuiz solveQuizMenu;
  /** createQuizMenu is showed when the user select Create Quiz option from the main menu */
  UICreateQuiz createQuizMenu;
  /** importQuizMenu is showed when the user select Import/Export option from the main menu */
  UIImportExport importExportMenu;
};

#endif //__UI_h__Quizzes_skluzada__
