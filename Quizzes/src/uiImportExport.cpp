#include "uiImportExport.h"
using namespace std;

UIImportExport::UIImportExport(QuizHandler & handler)
: handlerPtr(&handler){}

void UIImportExport::show(){
  menu();
}

void UIImportExport::menu(const std::string & messg){
  WINDOW * win;
  win = createWindowMenu();

  string header = "IMPORT/EXPORT MENU";
  printHeader(win, header);

  //if message is passed print it, else clear the line
  if (messg.size() > 0)
    printGreen(win, messg, 1);
  else
    printCenterClear(win, "", 1);

  int choice;
  vector<string> options {"IMPORT QUIZZES", "EXPORT QUIZZES", "SETTINGS", "EXIT TO MAIN MENU"};
  choice = navigateOptions(win, options);

  switch(choice){
    case 1: // "IMPORT"
      destroyWindow(win);
      importQuizMenu();
      break;
    case 2: // "EXPORT"
      destroyWindow(win);
      exportQuizMenu();
      break;
    case 3: // "SETTINGS"
      destroyWindow(win);
      settings();
      break;
    case 4: // "EXIT TO MAIN MENU"
      destroyWindow(win);
      return;
  }
}

void UIImportExport::importQuizMenu(){
  WINDOW * win;
  win = createWindowMenu();

  string header = "IMPORT QUIZZES";
  printHeader(win, header);

  int choice;
  vector<string> options = handlerPtr->getTitlesInPath();
  options.push_back("NEXT");
  options.push_back("BACK");
  options.push_back("EXIT TO MAIN MENU");
  vector<int> isHighlighted (options.size(), 0);

  choice = navigateScrollingOptions(win, options, isHighlighted);
  vector<string> titles;
  int imported = 0;
  int chosen = 0;
  switch(choice){
    case 1:
      destroyWindow(win);
      // add chosen quizzes to the vector
      for(int i = 0; i < (int)options.size()-3; i++){
        if (isHighlighted[i])
          titles.push_back(options[i]);
      }
      chosen = titles.size();
      imported = handlerPtr->importQuizzes(titles);
      menu(to_string(imported) + "/" + to_string(chosen) + " quizzes imported");
      break;
      break;
    case 2: // "BACK"
      destroyWindow(win);
      menu();
      break;
    case 3: // "EXIT TO MAIN MENU"
      destroyWindow(win);
      return;
  }
}

void UIImportExport::exportQuizMenu(){
  WINDOW * win;
  win = createWindowMenu();

  string header = "EXPORT QUIZZES";
  printHeader(win, header);

  int choice;
  vector<string> options = handlerPtr->getTitles();
  options.push_back("NEXT");
  options.push_back("BACK");
  options.push_back("EXIT TO MAIN MENU");
  vector<int> isHighlighted (options.size(), 0);

  choice = navigateScrollingOptions(win, options, isHighlighted);
  vector<string> titles;
  int exported = 0;
  int chosen = 0;
  switch(choice){
    case 1:
      destroyWindow(win);
      // add chosen quizzes to the vector
      for(int i = 0; i < (int)options.size()-3; i++){
        if (isHighlighted[i])
          titles.push_back(options[i]);
      }
      chosen = titles.size();
      exported = handlerPtr->exportQuizzes(titles);
      menu(to_string(exported) + "/" + to_string(chosen) + " quizzes exported");
      break;
    case 2: // "BACK"
      destroyWindow(win);
      menu();
      break;
    case 3: // "EXIT TO MAIN MENU"
      destroyWindow(win);
      return;
  }
}

void UIImportExport::settings(){
  WINDOW * win;
  win = createWindowMenu();

  string header = "IMPORT/EXPORT SETTINGS";
  printHeader(win, header);

  int choice;
  vector<string> inputs = {handlerPtr->getPath()};
  vector<int> isInputBox = {1, 0, 0};
  vector<string> options = {"Directory path: (default = ../examples/)" , "BACK", "EXIT TO MAIN MENU"};
  choice = navigateInput(win, options, inputs, isInputBox);

  int err = 1;
  while(err){ // while err = 1 we can't proceed
    switch(choice){
      case 2: // "BACK"
        if (!inputs[0].size()){ // if user input is empty
          choice = navigateInput(win, options, inputs, isInputBox, 2, 0, "Directory path not entered");
          break;
        }

        // setPath() fails when it can't fopen() or remove() a file in the given directory path
        if (!handlerPtr->setPath(inputs[0])){
          choice = navigateInput(win, options, inputs, isInputBox, 2, 0, "Can't open a file in given path");
          break;
        }
        err = 0;
        destroyWindow(win);
        menu();
        break;
      case 3: // "EXIT TO MAIN MENU"
        err = 0;
        destroyWindow(win);
        return;
    }
  }
}
