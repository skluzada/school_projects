#ifndef __UIPrinter_h__Quizzes_skluzada__
#define __UIPrinter_h__Quizzes_skluzada__
#include <ncurses.h>
#include <string>
#include <vector>

/** Window width ratio relative to the source terminal */
const double Window_WidthRatio = 1.15;
/** Window height ratio relative to the source terminal */
const double Window_HeightRatio = 1.15;

/** Auxiliary function that decides whether the passed char is printable into the input box
*
* @param[in] c Char that is or is not printable
*
* @return True if the char is printable, else return false
*/
bool isCharacter(int c);

/** Auxiliary function that deletes all the whitespaces from the passed string
*
* @param[in] text String with or without whitespaces
*
* @return String without whitespaces
*/
std::string deleteWhitespaces(const std::string & text);


/**
* Class UIPrinter provides methods that are used by the UI elements
*/
class UIPrinter{
protected:
  /**
  * Method that creates a new window with borders and size relative to the source terminal
  *
  * @return Pointer to a newly created window
  */
  WINDOW * createWindowMenu();
  /**
  * Method that delets a window with  it's borders
  *
  * @param[in] win Pointer to a window that will be deleted
  */
  void destroyWindow(WINDOW * win);
  /**
  * Method to print a text in the center of the window
  *
  * @param[in] win Pointer to a window where the text will be printed
  * @param[in] text Text that will be printed
  * @param[in] posY Specifies the Y position where the text will be printed
  */
  void printCenter(WINDOW * win, const std::string & text, int posY);
  /**
  * Method to print a text in the center of the window and delete the previous text printed on the same position
  *
  * @param[in] win Pointer to a window where the text will be printed
  * @param[in] text Text that will be printed
  * @param[in] posY Specifies the Y position where the text will be printed
  * @param[in] highlight Specifies whether the text shoudl be highlighted (reversed) or not
  */
  void printCenterClear(WINDOW * win, const std::string & text, int posY, int highlight = 0);
  /**
  * Method to print a text in the left
  *
  * @param[in] win Pointer to a window where the text will be printed
  * @param[in] text Text that will be printed
  * @param[in] posY Specifies the Y position where the text will be printed
  */
  void printLeft(WINDOW * win, const std::string & text, int posY);
  /**
  * Method to print a header
  *
  * @param[in] win Pointer to a window where the header will be printed
  * @param[in] text Text that will be printed in the header
  */
  void printHeader(WINDOW * win, const std::string & text);
  /**
  * Method to print a footer
  *
  * @param[in] win Pointer to a window where the footer will be printed
  * @param[in] text Text that will be printed in the footer
  */
  void printFooter(WINDOW * win, const std::string & text);
  /**
  * Method to print a line that acts as an input box
  *
  * @param[in] win Pointer to a window where the inputbox will be printed
  * @param[in] input Text that are currently shown in the input box
  */
  void printInputBox(WINDOW * win, std::string & input);
  /**
  * Method to print an error
  *
  * @param[in] win Pointer to a window where the error will be printed
  * @param[in] text Text that will be printed in the error
  * @param[in] posY Specifies the Y position where the error will be printed
  */
  void printError(WINDOW * win, const std::string & text, int posY);
  /**
  * Method to print a green text in the center
  *
  * @param[in] win Pointer to a window where the green text will be printed
  * @param[in] text Text that will be printed green
  * @param[in] posY Specifies the Y position where the green text will be printed
  */
  void printGreen(WINDOW * win, const std::string & text, int posY);
  /**
  * Method to print choice in the format: text true/false
  *
  * @param[in] win Pointer to a window where the choice will be printed
  * @param[in] text Text that will be printed in the choice
  * @param[in] chosen If true print true next to the text, else print false
  * @param[in] posY Specifies the Y position where the green text will be printed
  */
  void printChoice(WINDOW * win, const std::string & text, int chosen, int posY = 1);
  /**
  * Method to print options that act as a simple menu
  *
  * @param[in] win Pointer to a window where the options will be printed
  * @param[in] options Vector of options that will be printed
  * @param[in] highlight Index of option that will be highlighted
  * @param[in] startY Y position where the printing will start from
  * @param[in] startX X position where the printing will start from
  */
  void printOptions(WINDOW * win, const std::vector<std::string> & options, int highlight, int startY = 1, int startX = 0);
  /**
  * Method to navigate options that act as a simple menu
  *
  * @param[in] win Pointer to a window where the options will be printed
  * @param[in] options Vector of options that will be printed
  * @param[in] highlight Index of option that will be highlighted
  * @param[in] startY Y position where the printing will start from
  * @param[in] startX X position where the printing will start from
  *
  * @return Number of option that was chosen by the user
  */
  int navigateOptions(WINDOW * win, const std::vector<std::string> & options, int highlight = 1, int startY = 1, int startX = 0);
  /**
  * Overloaded method to print one page of options that act as a menu that can be scrolled up and down
  *
  * @param[in] win Pointer to a window where the options will be printed
  * @param[in] options Vector of options that will be printed
  * @param[in] highlight Index of option that will be highlighted
  * @param[in] startY Y position where the printing will start from
  * @param[in] startX X position where the printing will start from
  * @param[in] page Number of page that will be printed
  * @param[in] optionsPerPage Number of options per page
  */
  void printScrollingOptions(WINDOW * win, const std::vector<std::string> & options, int highlight, int startY, int startX, int page, int optionsPerPage);
  /**
  * Overloaded method to navigate options that act as a menu that can be scrolled up and down
  *
  * @param[in] win Pointer to a window where the options will be printed
  * @param[in] options Vector of options that will be printed
  * @param[in] highlight Index of option that will be highlighted
  * @param[in] startY Y position where the printing will start from
  * @param[in] startX X position where the printing will start from
  * @param[in] page Number of page that will be printed
  * @param[in] optionsPerPage Number of options per page
  *
  * @return Number of option that was chosen by the user
  */
  int navigateScrollingOptions(WINDOW * win, const std::vector<std::string> & options, int highlight = 1, int startY = 1, int startX = 0);
  /**
  * Overloaded method to print one page of options that act as a menu that can be scrolled up and down and multiple options can be selected
  *
  * @param[in] win Pointer to a window where the options will be printed
  * @param[in] options Vector of options that will be printed
  * @param[in] isHighlighted Vector of options that are highlighted
  * @param[in] highlight Index of option that will be highlighted
  * @param[in] startY Y position where the printing will start from
  * @param[in] startX X position where the printing will start from
  * @param[in] page Number of page that will be printed
  * @param[in] optionsPerPage Number of options per page
  */
  void printScrollingOptions(WINDOW * win, const std::vector<std::string> & options, std::vector<int> & isHighlighted, int highlight, int startY, int startX, int page, int optionsPerPage);
  /**
  * Overloaded method to print one page of options that act as a menu that can be scrolled up and down and multiple options can be selected
  *
  * @param[in] win Pointer to a window where the options will be printed
  * @param[in] options Vector of options that will be printed
  * @param[in] isHighlighted Vector of options that are highlighted
  * @param[in] highlight Index of option that will be highlighted
  * @param[in] startY Y position where the printing will start from
  * @param[in] startX X position where the printing will start from
  * @param[in] page Number of page that will be printed
  * @param[in] optionsPerPage Number of options per page
  *
  * @return Vector that specify what options were chosen by the user
  */
  int navigateScrollingOptions(WINDOW * win, const std::vector<std::string> & options, std::vector<int> & isHighlighted, int highlight = 1, int startY = 1, int startX = 0);
  /**
  * Method to print options that act as a window with one or more input boxes
  *
  * @param[in] win Pointer to a window where the options and input boxes will be printed
  * @param[in] options Vector of options that will be printed
  * @param[in] inputs Vector of inputs that will be printed inside of the input boxes
  * @param[in] isInputBox Specifies on what positions the input boxes are
  * @param[in] highlight Index of option that will be highlighted
  * @param[in] startY Y position where the printing will start from
  */
  void printInput(WINDOW * win, const std::vector<std::string> & options, std::vector<std::string> & inputs, const std::vector<int> & isInputBox, int highlight, int startY);
  /**
  * Method to navigate options that act as a window with one or more input boxes
  *
  * @param[in] win Pointer to a window where the options and input boxes will be printed
  * @param[in] options Vector of options that will be printed
  * @param[in] inputs Vector of inputs that will be printed inside of the input boxes
  * @param[in] isInputBox Specifies on what positions the input boxes are
  * @param[in] highlight Index of option that will be highlighted
  * @param[in] startY Y position where the printing will start from
  *
  * @return Number of the option choosen by the user
  */
  int navigateInput(WINDOW * win, const std::vector<std::string> & options, std::vector<std::string> & inputs, const std::vector<int> & isInputBox, int highlight = 1, int startY = 0, const std::string & error = "", int errPos = 4);
  /**
  * Method to print options and choices
  *
  * @param[in] win Pointer to a window where the options and choices will be printed
  * @param[in] choices Vector of choices that will be printed
  * @param[in] chosen Specify what choices are true or false
  * @param[in] options Vector of options that will be printed
  * @param[in] highlight Index of option that will be highlighted
  * @param[in] startY Y position where the printing will start from
  */
  void printChoices(WINDOW * win, const std::vector<std::string> & choices, const std::vector<int> & chosen, const std::vector<std::string> & options, int highlight = 1, int startY = 1);
  /**
  * Method to navigate options and choices
  *
  * @param[in] win Pointer to a window where the options and choices will be printed
  * @param[in] choices Vector of choices that will be printed
  * @param[in] chosen Specify what choices are true or false
  * @param[in] options Vector of options that will be printed
  * @param[in] highlight Index of option that will be highlighted
  * @param[in] startY Y position where the printing will start from
  *
  * @return Number of the option choosen by the user
  */
  int navigateChoices(WINDOW * win, const std::vector<std::string> & choices, std::vector<int> & chosen, const std::vector<std::string> & options, int singleChoice, int highlight = 1, int startY = 1);
  /**
  * Method to print options and choices that can be ordered
  *
  * @param[in] win Pointer to a window where the options and choices will be printed
  * @param[in] choices Vector of choices that will be printed
  * @param[in] order Specify what order int which the choices will be printed
  * @param[in] options Vector of options that will be printed
  * @param[in] highlight Index of option that will be highlighted
  * @param[in] startY Y position where the printing will start from
  * @param[in] chosen Specifies whether the highlighted choice is chosen, if it is it can be moved up and down to change the order of choices
  */
  void printOrder(WINDOW * win, const std::vector<std::string> & choices, const std::vector<int> & order, const std::vector<std::string> & options, int highlight = 1, int startY = 1, int chosen = 0);
  /**
  * Method to print options and choices that can be ordered
  *
  * @param[in] win Pointer to a window where the options and choices will be printed
  * @param[in] choices Vector of choices that will be printed
  * @param[in] order Specify what order int which the choices will be printed
  * @param[in] options Vector of options that will be printed
  * @param[in] highlight Index of option that will be highlighted
  * @param[in] startY Y position where the printing will start from
  *
  * @return Number of the option choosen by the user
  */
  int navigateOrder(WINDOW * win, const std::vector<std::string> & choices, std::vector<int> & order, const std::vector<std::string> & options, int highlight = 1, int startY = 1);
};

#endif //__uiPrinter_h__Quizzes_skluzada__
