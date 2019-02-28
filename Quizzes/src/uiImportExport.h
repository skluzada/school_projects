#ifndef __UIImportExport_h__Quizzes_skluzada__
#define __UIImportExport_h__Quizzes_skluzada__
#include "handler.h"
#include "uiPrinter.h"
#include <ncurses.h>
#include <string>
#include <vector>

/**
* Class UICreateQuiz is allows user to import and export quizzes using ncruses ui
*/
class UIImportExport : public UIPrinter{
public:
  /**
  * Constructor
  *
  * @param[in] handler Quiz Handler that is used when the quizzes are imported or exported
  */
  UIImportExport(QuizHandler & handler);
  /**
  * Method to show the UI to import or export quizzes
  */
  void show();
  /**
  * Method to show a menu, where the user chooses whether he wants to import or export quiz
  *
  * @param[in] messg If some quizzes were imported or exported before, messg specifies how many quizzes were succesfully imported or exported
  */
  void menu(const std::string & messg = "");
  /**
  * Method where the user select the quizzes that he wants to import
  */
  void importQuizMenu();
  /**
  * Method where the user select the quizzes that he wants to export 
  */
  void exportQuizMenu();
  /**
  * Method to show a settings window, where the user can change the directory path used when importing or exporting quizzes
  */
  void settings();
private:
  QuizHandler * handlerPtr;
};


#endif //__UIImportExport_h__Quizzes_skluzada__
