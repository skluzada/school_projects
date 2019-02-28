#include "ui.h"
#include <iostream>
#include <cstdlib>
using namespace std;

UI::UI()
: handler(), solveQuizMenu(handler), createQuizMenu(handler), importExportMenu(handler){}

void UI::start(){
  initscr(); // start curses mode
  clear(); // the screen is completely cleared
  noecho(); // characters typed by user will not be echoed
  cbreak(); // characters typed by user will be immediately availible
  keypad(stdscr, TRUE); // enable the keypad 

  start_color();
  init_pair(1, COLOR_RED, COLOR_BLACK);
  init_pair(2, COLOR_GREEN, COLOR_BLACK);
  init_pair(3, COLOR_YELLOW, COLOR_BLACK);

  curs_set(0);

  int maxX, maxY;
  getmaxyx(stdscr, maxY, maxX);
  if (maxX < Window_MinWidth || maxY < Window_MinHeight){
    printError(stdscr, "Terminal window too small" , 0);
    wattron(stdscr, COLOR_PAIR(1));
    printCenter(stdscr, "min width = " + to_string(Window_MinWidth) + ", min height = " + to_string(Window_MinHeight), 1);
    printCenter(stdscr, "your width = " + to_string(maxX) + ", your height = " + to_string(maxY), 1);
    wattron(stdscr, COLOR_PAIR(1));
    refresh();
    getch();
    endwin();
    return;
  }

  showMainMenu();

  refresh();
  endwin();
}

void UI::showMainMenu(){
  WINDOW * win;
  win = createWindowMenu();

  string header = "MAIN MENU";
  printHeader(win, header);

  int choice;
  vector<string> options {"SOLVE QUIZ", "CREATE QUIZ", "IMPORT/EXPORT",
    "ABOUT", "EXIT"};
  choice = navigateOptions(win, options);

  switch(choice){
    case 1: // "SOLVE QUIZ"
      destroyWindow(win);
      solveQuizMenu.show();
      showMainMenu();
      break;
    case 2: // "CREATE QUIZ"
      destroyWindow(win);
      createQuizMenu.show();
      showMainMenu();
      break;
    case 3: // "IMPORT/EXPORT"
      destroyWindow(win);
      importExportMenu.show();
      showMainMenu();
      break;
    case 4: // "ABOUT"
      destroyWindow(win);
      showAbout();
      showMainMenu();
      break;
    case 5: // "EXIT"
      destroyWindow(win);
      return;
      break;
  }
}

void UI::showAbout(){
  WINDOW * win;
  win = createWindowMenu();

  string header = "ABOUT";
  printHeader(win, header);

  wattron(win, A_BOLD);
  printCenter(win, "PA2 Semestral Project", 2);
  wattroff(win, A_BOLD);
  printCenter(win, "Author: skluzada 2018", 1);
  wattron(win, A_UNDERLINE);
  printCenter(win, "Topic: Quizzes", 2);
  wattroff(win, A_UNDERLINE);
  printCenter(win, "Create a program to create and solve quizzes.", 1);

  printLeft(win, "Implement the following functionality:", 2);
  printLeft(win, "1. Three or more types of questions", 1);
  printLeft(win, "2. Three or more types of answers", 1);
  printLeft(win, "3. Separate quiz to multiple pages", 1);
  printLeft(win, "4. Show pages based on previous answers", 1);
  printLeft(win, "5. User interface for creating, solving and evaluating quizzes", 1);
  printLeft(win, "6. Export and import mupltiple quizzes", 1);

  int choice;
  vector<string> options {"EXIT TO MAIN MENU"};
  choice = navigateOptions(win, options, 1, 15);

  switch(choice){
    case 1:
      destroyWindow(win);
      break;
  }
}
