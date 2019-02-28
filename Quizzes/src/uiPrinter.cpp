#include "uiPrinter.h"
using namespace std;

std::string deleteWhitespaces(const std::string & text){
  string out = "";
  for(size_t i = 0; i < text.size(); i++){
    if (text[i] != ' ')
      out+=text[i];
  }

  return out;
}

bool isCharacter(int c){
  if (c > 31 && c < 126)
    return true;

  return false;
}

WINDOW * UIPrinter::createWindowMenu(){
  WINDOW * win;
  int maxY, maxX, width, height, startY, startX;

  // calculate window size and position based on terminal size
  getmaxyx(stdscr, maxY, maxX);
  width = maxX/Window_WidthRatio;
  height = maxY/Window_HeightRatio;
  startY = (maxY-height)/2;
  startX = (maxX-width)/2;

  win = newwin(height, width, startY, startX);
  box(win, 0, 0);
  keypad(win, TRUE);

  return win;
}

void UIPrinter::destroyWindow(WINDOW * win){
  box(win, ' ', ' ');
  wborder(win, ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ');
  wrefresh(win);
  delwin(win);
}

void UIPrinter::printCenter(WINDOW * win, const std::string & text, int posY){
  int maxX, y, x;
  maxX = getmaxx(win);
  getyx(win, y, x);
  y+=posY;
  x = (maxX-text.size())/2; // calculate x position to print text in the center
  mvwprintw(win, y, x, text.c_str());
}

void UIPrinter::printCenterClear(WINDOW * win, const std::string & text, int posY, int highlight){
  int maxX, y, x;
  maxX = getmaxx(win);
  getyx(win, y, x);
  y+=posY;
  wmove(win, y, 0);
  wclrtoeol(win); // clear the whole line
  mvwaddch(win, y, 0, ACS_VLINE); // add vertical frame line on the left
  mvwaddch(win, y, maxX-1, ACS_VLINE); // add vertical frame line on the right
  x = (maxX-text.size())/2; // calculate x position to print text in the center

  // if highlight print as reversed
  if(highlight){
    wattron(win, A_REVERSE);
    mvwprintw(win, y, x, text.c_str());
    wattroff(win, A_REVERSE);
  }
  else
    mvwprintw(win, y, x, text.c_str());
}

void UIPrinter::printLeft(WINDOW * win, const std::string & text, int posY){
  int maxX, y, x;
  maxX = getmaxx(win);
  getyx(win, y, x);
  y+=posY;
  x = maxX-(maxX/1.25);
  mvwprintw(win, y, x, text.c_str());
}

void UIPrinter::printHeader(WINDOW * win, const std::string & text){
  string header = "| ";
  header += text;
  header += " |";
  int maxX, y, x;
  maxX = getmaxx(win);
  y = 0;
  x = (maxX-header.size())/2;
  mvwprintw(win, y, x, header.c_str());
}

void UIPrinter::printFooter(WINDOW * win, const std::string & text){
  int maxX, maxY, y, x;
  maxX = getmaxx(win);
  maxY = getmaxy(win);
  y = maxY-1;
  // add spaces to cover a possibly longer previous footer
  string footer = "  " + text + " ";
  if (footer.size()%2 == 1)
    footer += " ";
  x = (maxX-footer.size())/2;
  mvwprintw(win, y, x, footer.c_str());
}

void UIPrinter::printInputBox(WINDOW * win, string & input){
  int maxX, y, x;
  maxX = getmaxx(win);
  // set input so that it's not longer than input box
  input = input.substr(0, maxX/1.5);
  string box = input;
  // append spaces to the input (reversed spaces act as an input box)
  box.append(maxX/1.5, ' ');
  // set input box size
  box = box.substr(0, maxX/1.5);
  getyx(win, y, x);
  y+=1;
  x = (maxX-box.size())/2;
  wattron(win, A_REVERSE);
  mvwprintw(win, y, x, box.c_str());
  wattroff(win, A_REVERSE);
}

void UIPrinter::printError(WINDOW * win, const std::string & text, int posY){
  string err = "ERROR: ";
  err += text;
  int maxX, y, x;
  maxX = getmaxx(win);
  y = posY;

  // add padding to cover a possibly longer error that was printed previously
  string padding((((maxX-err.size())/4)-1), ' ');
  string error = padding + err + padding;
  x = (maxX-error.size())/2;
  wattron(win, COLOR_PAIR(1)); // red color on
  mvwprintw(win, y, x, error.c_str());
  wattroff(win, COLOR_PAIR(1)); // red color off
}

void UIPrinter::printGreen(WINDOW * win, const std::string & text, int posY){
  int maxX, y, x;
  maxX = getmaxx(win);
  y = posY;

  // add padding to cover a possibly longer green text that was printed previously
  string padding((((maxX-text.size())/4)-1), ' ');
  string messg = padding + text + padding;
  x = (maxX-messg.size())/2;
  wattron(win, COLOR_PAIR(2)); // green color on
  mvwprintw(win, y, x, messg.c_str());
  wattroff(win, COLOR_PAIR(2)); // green color off
}

void UIPrinter::printChoice(WINDOW * win, const std::string & text, int chosen, int posY){
  int maxX, y, x;
  maxX = getmaxx(win);
  getyx(win, y, x);
  y+=posY;
  x = (maxX-(text.size()+6))/2;
  mvwprintw(win, y, x, text.c_str());
  getyx(win, y, x);

  // if the choice is chosen print green true beside the choice, else print red false
  if (chosen){
    wattron(win, COLOR_PAIR(2));
    mvwprintw(win, y, x, " true ");
    wattroff(win, COLOR_PAIR(2));
  }
  else{
    wattron(win, COLOR_PAIR(1));
    mvwprintw(win, y, x, " false");
    wattroff(win, COLOR_PAIR(1));
  }
}

void UIPrinter::printOptions(WINDOW * win, const std::vector<std::string> & options, int highlight, int startY, int startX){
  wmove(win, startY, startX);
  // print all the options, reverse the highlighted one
  for(int i = 0; i < (int)options.size(); i++){
    if (i+1 == highlight){
      wattron(win, A_REVERSE);
      printCenter(win, options[i], 2);
      wattroff(win, A_REVERSE);
    }
    else {
      printCenter(win, options[i], 2);
    }
  }
  wrefresh(win);
}

int UIPrinter::navigateOptions(WINDOW * win, const std::vector<std::string> & options, int highlight, int startY, int startX){
  int c;
  int choice = 0;

  while(true){
    printOptions(win, options, highlight, startY, startX);
    c = wgetch(win);
    switch(c){
      case KEY_DOWN:
        // if last option is highlighted
        if (highlight >= (int)options.size())
          highlight = 1;
        else
          highlight++;
        break;
      case KEY_UP:
        // if first option is highlighted
        if (highlight <= 1)
          highlight = (int)options.size();
        else
          highlight--;
        break;
      case 10: // ENTER
        choice = highlight;
        break;
      default:
        break;
    }
    if (choice != 0)
      break;
  }

  return highlight;
}

void UIPrinter::printScrollingOptions(WINDOW * win, const std::vector<std::string> & options, int highlight, int startY, int startX, int page, int optionsPerPage){
  wmove(win, startY, startX);
  // print all the options on the given page and reverse the highlighted one
  for(int i = 0; i < optionsPerPage; i++){
    // if the page is not complete, fill it with empty lines
    if (i+(page-1)*optionsPerPage == (int)options.size()){
      for( ; i < optionsPerPage; i++){
        printCenterClear(win, " ", 2, 0);
      }
      break;
    }

    if ((page-1)*optionsPerPage+i+1 == highlight){
      printCenterClear(win, options[(page-1)*optionsPerPage + i], 2, 1);
    }
    else {
      printCenterClear(win, options[(page-1)*optionsPerPage + i], 2, 0);
    }
  }
  wrefresh(win);
}

int UIPrinter::navigateScrollingOptions(WINDOW * win, const std::vector<std::string> & options, int highlight, int startY, int startX){
  // calculate options per page and number of pages
  int maxY = getmaxy(win) - startY -3;
  int optionsPerPage = maxY/2;
  int pages = options.size()/optionsPerPage;
  if (options.size()%optionsPerPage != 0)
    pages++;

  int c;
  int choice = 0;
  int page = 1;
  while(true){
    printScrollingOptions(win, options, highlight, startY, startX, page, optionsPerPage);
    // print what page the user is on
    printFooter(win, to_string(page) + "/" + to_string(pages));
    c = wgetch(win);
    switch(c){
      case KEY_DOWN:
        // if last option is highlighted
        if (highlight >= (int)options.size()){
          highlight = 1;
          page = 1;
        }
        // if the end of the page is reached
        else if (highlight%optionsPerPage == 0){
          highlight++;
          page++;
        }
        else
          highlight++;
        break;
      case KEY_UP:
        // if first option is highlighted
        if (highlight <= 1){
          highlight = (int)options.size();
          page = pages;
        }
        // if the beg of the page is reached
        else if (highlight%optionsPerPage == 1){
          highlight--;
          page--;
        }
        else
          highlight--;
        break;
      case 10:
        choice = highlight;
        break;
      default:
        break;
    }
    if (choice != 0)
      break;
  }

  return highlight;
}

void UIPrinter::printScrollingOptions(WINDOW * win, const std::vector<std::string> & options, std::vector<int> & isHighlighted, int highlight, int startY, int startX, int page, int optionsPerPage){
  wmove(win, startY, startX);
  // print all the options on the given page and reverse the highlighted ones
  for(int i = 0; i < optionsPerPage; i++){
    // if the page is not complete, fill it with empty lines
    if (i+(page-1)*optionsPerPage == (int)options.size()){
      for( ; i < optionsPerPage; i++){
        printCenterClear(win, " ", 2, 0);
      }
      break;
    }

    if (isHighlighted[(page-1)*optionsPerPage + i] ||
        highlight == (page-1)*optionsPerPage + i+1){
      printCenterClear(win, options[(page-1)*optionsPerPage + i], 2, 1);
    }
    else {
      printCenterClear(win, options[(page-1)*optionsPerPage + i], 2, 0);
    }
  }
  wrefresh(win);
}

int UIPrinter::navigateScrollingOptions(WINDOW * win, const std::vector<std::string> & options, std::vector<int> & isHighlighted, int highlight, int startY, int startX){
  // calculate options per page and number of pages
  int maxY = getmaxy(win) - startY -3;
  int optionsPerPage = maxY/2;
  int pages = options.size()/optionsPerPage;
  if (options.size()%optionsPerPage != 0)
    pages++;

  int c;
  int choice = 0;
  int page = 1;
  while(true){
    printScrollingOptions(win, options, isHighlighted, highlight, startY, startX, page, optionsPerPage);
    // print what page the user is on
    printFooter(win, to_string(page) + "/" + to_string(pages));
    c = wgetch(win);
    switch(c){
      case KEY_DOWN:
        // if last option is highlighted
        if (highlight >= (int)options.size()){
          highlight = 1;
          page = 1;
        }
        // if the end of the page is reached
        else if (highlight%optionsPerPage == 0){
          highlight++;
          page++;
        }
        else
          highlight++;
        break;
      case KEY_UP:
        // if last option is highlighted
        if (highlight <= 1){
          highlight = (int)options.size();
          page = pages;
        }
        // if the beg of the page is reached
        else if (highlight%optionsPerPage == 1){
          highlight--;
          page--;
        }
        else
          highlight--;
        break;
      case 10: // ENTER
        // the last three options acts as buttons
        if (highlight > (int)isHighlighted.size()-3){
          choice = highlight - (int)isHighlighted.size()+3;
        }
        else{ // reverse options highlight
          isHighlighted[highlight-1] = !(isHighlighted[highlight-1]);
        }
        break;
      default:
        break;
    }
    if (choice != 0)
      break;
  }

  return choice;
}

void UIPrinter::printInput(WINDOW * win, const std::vector<std::string> & options, std::vector<std::string> & inputs, const std::vector<int> & isInputBox, int highlight, int startY){
  curs_set(0);
  wmove(win, startY, 0);
  int highlightIndex, highlightY;
  int inputIndex = 0;
  // input indexes are needed to know which input is associated with input box
  vector<int> inputIndexes (options.size(), 0);
  for(int i = 0; i < (int)options.size(); i++){
    // if the option is an input box
    if (isInputBox[i]){
      printCenter(win, options[i], 2);
      printInputBox(win, inputs[inputIndex]);
      inputIndexes[i] = inputIndex;
      // if input box is highlighted get it's index and y pos
      if (highlight == i+1){
        highlightIndex = inputIndex;
        highlightY = getcury(win);
      }
      inputIndex++;
    }
    // if the option is not an input box
    else {
      if (highlight == i+1){
        wattron(win, A_REVERSE);
        printCenter(win, options[i], 2);
        wattroff(win, A_REVERSE);
      }
      else{
        printCenter(win, options[i], 2);
      }
    }
  }

  // if the input box is highlighted show cursor
  if(isInputBox[highlight-1]){
    curs_set(1);
    int maxX;
    maxX = getmaxx(win);
    wmove(win, highlightY, inputs[highlightIndex].size()+(maxX-maxX/1.5)/2);
  }
}

int UIPrinter::navigateInput(WINDOW * win, const std::vector<std::string> & options, std::vector<std::string> & inputs, const std::vector<int> & isInputBox, int highlight, int startY, const std::string & error, int errPos){
  int c;
  int choice = 0;

  // if an error string is passed, print it
  if (error.size() > 0)
    printError(win, error, errPos);
  // else print spaces to hide an error that could've been printed previously
  else{
    int maxX;
    maxX = getmaxx(win);
    string s (maxX/1.25, ' ');
    wattron(win, COLOR_PAIR(1));
    printCenter(win, s, errPos);
    wattroff(win, COLOR_PAIR(1));
  }

  while(true){
    printInput(win, options, inputs, isInputBox, highlight, startY);
    c = wgetch(win);
    switch(c){
      case KEY_DOWN:
        // if the last option is highlighted
        if (highlight >= (int)options.size())
          highlight = 1;
        else
          highlight++;
        break;

      case KEY_UP:
        // if the first option is highlighted
        if (highlight <= 1)
          highlight = (int)options.size();
        else
          highlight--;
        break;

      case 10: // ENTER
        // if an input box is not selected => button was selected
        if (!isInputBox[highlight-1])
          choice = highlight;
        break;

      case 263: // BACKSPACE
        // if an input box is highlighted, delete last char
        if (isInputBox[highlight-1])
          inputs[highlight-1] = inputs[highlight-1].substr(0, inputs[highlight-1].size()-1);

      default:
        // if the pressed key is an printable char
        if (isCharacter(c)){
          // if an input box is highlighted, add the char
          if (isInputBox[highlight-1])
            inputs[highlight-1]+=c;
        }
        break;
    }
    if (choice != 0)
      break;
  }

  return choice;
}

void UIPrinter::printChoices(WINDOW * win, const std::vector<std::string> & choices, const std::vector<int> & chosen, const std::vector<std::string> & options, int highlight, int startY){
  wmove(win, startY, 0);
  // print choices, reverse the highlighted one
  for(int i = 0; i < (int)choices.size(); i++){
    if (highlight == i+1){
      wattron(win, A_REVERSE);
      printChoice(win, choices[i], chosen[i]);
      wattroff(win, A_REVERSE);
    }
    else {
      printChoice(win, choices[i], chosen[i]);
    }
  }

  // options are printed below choices
  for(int i = 0; i < (int)options.size(); i++){
    if (highlight == i+(int)choices.size()+1){
      wattron(win, A_REVERSE);
      printCenter(win, options[i], 2);
      wattroff(win, A_REVERSE);
    }
    else {
      printCenter(win, options[i], 2);
    }
  }
}

int UIPrinter::navigateChoices(WINDOW * win, const std::vector<std::string> & choices, std::vector<int> & chosen, const std::vector<std::string> & options, int singleChoice, int highlight, int startY){
  int c;
  int choice = 0;

  while(true){
    printChoices(win, choices, chosen, options, highlight, startY);
    c = wgetch(win);
    switch(c){
      case KEY_DOWN:
        // if the last element is highlighted
        if (highlight >= (int)options.size()+(int)choices.size())
          highlight = 1;
        else
          highlight++;
        break;
      case KEY_UP:
        // if the first element is highlighted
        if (highlight <= 1)
          highlight = (int)options.size()+(int)choices.size();
        else
          highlight--;
        break;
      case 10: // ENTER
        // if an option is highlighted, we can set choice and continue
        if (highlight > (int)choices.size())
          choice = highlight-choices.size();
        // if a choice is highlighted
        else {
          if (singleChoice){
            // if single choice, set all other choices to false first
            for(int i = 0; i < (int)choices.size(); i++){
              if (i != highlight-1)
                chosen[i] = 0;
            }
          }
          // if choice was true set to false and vice versa
          chosen[highlight-1] = !chosen[highlight-1];
        }
        break;
      default:
        break;
    }
    if (choice != 0)
      break;
  }

  return choice;
}

void UIPrinter::printOrder(WINDOW * win, const std::vector<std::string> & choices, const std::vector<int> & order, const std::vector<std::string> & options, int highlight, int startY, int chosen){
  wmove(win, startY, 0);
  // print order choices
  for(int i = 0; i < (int)choices.size(); i++){
    if (highlight == i+1){
      // print arrows next to chosen choice
      if (chosen){
        printCenterClear(win, "->" + choices[order[i]] + "<-", 1, 1);
      }
      // if the choice is highlighted but not chosen
      else{
        printCenterClear(win, choices[order[i]], 1, 1);
      }
    }
    else {
      printCenterClear(win, choices[order[i]], 1, 0);
    }
  }

  // options are printed after the order choices
  for(int i = 0; i < (int)options.size(); i++){
    if (highlight == i+(int)choices.size()+1){
      wattron(win, A_REVERSE);
      printCenter(win, options[i], 2);
      wattroff(win, A_REVERSE);
    }
    else {
      printCenter(win, options[i], 2);
    }
  }
}

int UIPrinter::navigateOrder(WINDOW * win, const std::vector<std::string> & choices, std::vector<int> & order, const std::vector<std::string> & options, int highlight, int startY){
  int c;
  int choice = 0;
  // when choice is chosen, user can move up and down with it to set order
  int chosen = 0;

  while(true){
    printOrder(win, choices, order, options, highlight, startY, chosen);
    c = wgetch(win);
    switch(c){
      case KEY_DOWN:
        if (chosen){
          // if the choice is chosen and not on the bottom yet, move it down
          if (!(highlight >= (int)choices.size())){
            highlight++;
            int tmp = order[chosen-1];
            order[chosen-1] = order[chosen];
            order[chosen] = tmp;
            chosen++;
          }
        }
        else{
          // if the last element is highlighted
          if (highlight >= (int)options.size()+(int)choices.size())
            highlight = 1;
          else
            highlight++;
        }
        break;
      case KEY_UP:
        if(chosen){
          // if the choice is chosen and not on the top yet, move it up
          if (!(highlight <= 1)){
            highlight--;
            int tmp = order[chosen-1];
            order[chosen-1] = order[chosen-2];
            order[chosen-2] = tmp;
            chosen--;
          }
        }
        else{
          // if the first element is highlighted
          if (highlight <= 1)
            highlight = (int)options.size()+(int)choices.size();
          else
            highlight--;
        }
        break;
      case 10:
        if (highlight > (int)choices.size())
          choice = highlight;
        else if (chosen){
          chosen = 0;
        }
        else {
          chosen = highlight;
          choice = highlight;
        }
        break;
      default:
        break;
    }
    if (choice > (int)choices.size())
      break;
  }

  return choice-choices.size();
}
