#include "uiSolveQuiz.h"
#include <typeinfo>
using namespace std;

UISolveQuiz::UISolveQuiz(QuizHandler & handler)
: handlerPtr(&handler){}

void UISolveQuiz::show(){
  chooseQuiz();
}

void UISolveQuiz::chooseQuiz(){
  WINDOW * win;
  win = createWindowMenu();

  string header = "CHOOSE QUIZ";
  printHeader(win, header);

  int choice;
  vector<string> options = handlerPtr->getTitles();
  options.push_back("EXIT TO MAIN MENU");
  choice = navigateScrollingOptions(win, options);

  if (choice == (int)options.size()){
    destroyWindow(win);
    return;
  }
  else{
    destroyWindow(win);
    overviewQuiz(options[choice-1]);
    return;
  }
}

void UISolveQuiz::overviewQuiz(const std::string & title){
  WINDOW * win;
  win = createWindowMenu();

  string header = "QUIZ OVERVIEW";
  printHeader(win, header);

  const Quiz * quiz = handlerPtr->getQuizPtr(title);

  wattron(win, A_BOLD);
  printCenter(win, quiz->getTitle(), 2);
  wattroff(win, A_BOLD);

  printCenter(win, "Topic: " + quiz->getTopic(), 1);
  printCenter(win, "Number of questions in the pool: " + to_string(quiz->getMaxNumberOfQuestions()), 1);
  printCenter(win, "Number of questions in the quiz: " + to_string(quiz->getNumberOfQuestions()), 1);
  printCenter(win, "Average question difficulty: " + to_string(quiz->getAverageDifficulty()), 1);

  int choice;
  vector<string> options = {"SOLVE QUIZ", "BACK", "EXIT TO MAIN MENU"};
  choice = navigateOptions(win, options, 1, 6);

  switch(choice){
    case 1: // "SOLVE QUIZ"
      destroyWindow(win);
      solveQuiz(handlerPtr->getQuizPtr(title));
      break;
    case 2: // "BACK"
      destroyWindow(win);
      chooseQuiz();
      break;
    case 3: // "EXIT TO MAIN MENU"
      destroyWindow(win);
      return;
  }
}

void UISolveQuiz::solveQuiz(const Quiz * quiz){
  double points = 0;
  double pointsMax = 0;
  double successRate = -1;
  vector<int> questionPool;
  // fill the question pool with all the question indexes
  for(int i = 0; i < quiz->getMaxNumberOfQuestions(); i++){
    questionPool.push_back(i);
  }

  // if the quiz is based on difficulty, the quiz is separated to parts
  if (quiz->getOrder() == "difficulty" && quiz->getMaxNumberOfQuestions() > 2){
    int pages = quiz->getNumberOfQuestions()/questionsPerPage;
    int lastPage = quiz->getNumberOfQuestions()%questionsPerPage;
    if (lastPage == 0){
      pages--;
      lastPage = questionsPerPage;
    }

    vector<int> questionsPos;
    for(int p = 0; p < pages; p++){
      questionsPos = quiz->getQuestionsPos(questionPool, successRate);
      for(int i = 0; i < questionsPerPage; i++){
        points+=solveQuestion(quiz->getQuestion(questionsPos[i]), quiz->getTitle(), i+1, questionsPerPage, p+1, pages+1);
        pointsMax+=quiz->getQuestion(questionsPos[i])->getMaxPoints();
      }
      successRate = points/pointsMax;
    }

    // print last page
    questionsPos = quiz->getQuestionsPos(questionPool, successRate, lastPage);
    for(int i = 0; i < lastPage; i++){
      points+=solveQuestion(quiz->getQuestion(questionsPos[i]), quiz->getTitle(), i+1, lastPage, pages+1, pages+1);
      pointsMax+=quiz->getQuestion(questionsPos[i])->getMaxPoints();
    }
    successRate = points/pointsMax;
  }
  // else the quiz is not seperated into parts
  else {
    // get the poisitions in which the questions will be showed
    vector<int> questionsPos = quiz->getQuestionsPos(questionPool);
    // show all the questions and count the points
    for(int i = 0; i < quiz->getNumberOfQuestions(); i++){
      points+=solveQuestion(quiz->getQuestion(questionsPos[i]), quiz->getTitle(), i+1, quiz->getNumberOfQuestions());
      pointsMax+=quiz->getQuestion(questionsPos[i])->getMaxPoints();
    }
  }
  // after all the questions were shown, create a window to show the result
  WINDOW * win;
  win = createWindowMenu();

  string header = "RESULT";
  printHeader(win, header);

  int successRatePercent = points/pointsMax*100;
  wattron(win, A_BOLD);
  // if successRate is lower than 33% print it red
  if (successRatePercent < 33){
    wattron(win, COLOR_PAIR(1));
    printCenter(win, to_string(successRatePercent) + "%%", 2);
    wattroff(win, COLOR_PAIR(1));
  }
  // if successRate is higher than 66% print it green
  else if (successRatePercent > 66){
    wattron(win, COLOR_PAIR(2));
    printCenter(win, to_string(successRatePercent) + "%%", 2);
    wattroff(win, COLOR_PAIR(2));
  }
  // if successRate is higher than 33% and lower than 67% print it yellow
  else {
    wattron(win, COLOR_PAIR(3));
    printCenter(win, to_string(successRatePercent) + "%%", 2);
    wattroff(win, COLOR_PAIR(3));
  }
  wattroff(win, A_BOLD);
  string pointsS = to_string(points);
  // cut the last 5 decimal places
  pointsS = pointsS.substr(0, pointsS.size()-5);
  string maxPointsS = to_string(pointsMax);
  // cut the last 5 decimal places
  maxPointsS = maxPointsS.substr(0, maxPointsS.size()-5);
  printCenter(win, pointsS + '/' + maxPointsS + " points" , 1);

  vector<string> options {"EXIT TO MAIN MENU"};
  navigateOptions(win, options, 1, 4);

  destroyWindow(win);
  return;
}

double UISolveQuiz::solveQuestion(const Question * question, const std::string & title, int nQuestion, int nQuestions, int nPage, int nPages){
  WINDOW * win;
  win = createWindowMenu();

  string header = title;
  printHeader(win, header);
  string footer = "";
  if (nPages != -1)
    footer += "part " + to_string(nPage) + "/" + to_string(nPages) + " ";
  footer += "question " + to_string(nQuestion) + "/" + to_string(nQuestions);
  printFooter(win, footer);

  // get the type of the question to call the right show function
  string questionType = question->getQuestionType();
  if (questionType == "QSingleChoice"){
    double points = solveQSingleChoice(win, question);
    destroyWindow(win);
    return points;
  }
  else if (questionType == "QMultipleChoice"){
    double points = solveQMultipleChoice(win, question);
    destroyWindow(win);
    return points;
  }
  else if (questionType == "QNumeric"){
    double points = solveQNumeric(win, question);
    destroyWindow(win);
    return points;
  }
  else if (questionType == "QText"){
    double points = solveQText(win, question);
    destroyWindow(win);
    return points;
  }
  else if (questionType == "QOrdering"){
    double points = solveQOrdering(win, question);
    destroyWindow(win);
    return points;
  }

  // the function should never get here
  destroyWindow(win);
  return 0;
}

double UISolveQuiz::solveQSingleChoice(WINDOW * win, const Question * question){
  // question elements contains question and choices
  vector<string> questionElements = question->getQuestion();
  wmove(win, 0, 0);
  wattron(win, A_BOLD);
  printCenter(win, questionElements[0], 2);
  wattroff(win, A_BOLD);

  printCenter(win, "Select one:", 2);

  auto begIt = questionElements.begin();
  auto endIt = questionElements.end();
  vector<string> choices(begIt+1, endIt);
  // set all the options to false
  vector<int> chosen(choices.size(), 0);
  vector<string> options = {"NEXT"};
  navigateChoices(win, choices, chosen, options, 1, 1, 4);

  bool trueIsSet = 0;
  while (!trueIsSet){
    for(int i = 0; i < (int)chosen.size(); i++){
      if (chosen[i] == 1)
        trueIsSet = 1;
    }
    if (!trueIsSet){
      printError(win, "Nothing selected", 5 + chosen.size());
      navigateChoices(win, choices, chosen, options, 1, 1, 4);
    }
  }

  UserAnswer answer;
  answer.vec = chosen;

  return question->getPoints(answer);
}

double UISolveQuiz::solveQMultipleChoice(WINDOW * win, const Question * question){
  // question elements contains question and choices
  vector<string> questionElements = question->getQuestion();
  wmove(win, 0, 0);
  wattron(win, A_BOLD);
  printCenter(win, questionElements[0], 2);
  wattroff(win, A_BOLD);

  printCenter(win, "Select correct:", 2);

  auto begIt = questionElements.begin();
  auto endIt = questionElements.end();
  vector<string> choices(begIt+1, endIt);
  // set all the options to false
  vector<int> chosen(choices.size(), 0);
  vector<string> options = {"NEXT"};
  navigateChoices(win, choices, chosen, options, 0, 1, 4);

  UserAnswer answer;
  answer.vec = chosen;

  return question->getPoints(answer);
}

double UISolveQuiz::solveQNumeric(WINDOW * win, const Question * question){
  vector<string> questionElements = question->getQuestion();
  wmove(win, 0, 0);
  wattron(win, A_BOLD);
  printCenter(win, questionElements[0], 2);
  wattroff(win, A_BOLD);

  vector<string> inputs = {""};
  vector<int> isInputBox = {1, 0}; // first option is an output box
  vector<string> options = {"Number answer:" ,"NEXT"};
  navigateInput(win, options, inputs, isInputBox, 1, 2);

  // while the input is empty
  while(!inputs[0].size())
    navigateInput(win, options, inputs, isInputBox, 1, 2, "Answer not entered", 6);

  // while a number is not entered
  while(atof(inputs[0].c_str()) == 0 && inputs[0] != "0")
    navigateInput(win, options, inputs, isInputBox, 1, 2, "Number not entered", 6);


  UserAnswer answer;
  answer.number = atof(inputs[0].c_str());

  return question->getPoints(answer);
}

double UISolveQuiz::solveQText(WINDOW * win, const Question * question){
  vector<string> questionElements = question->getQuestion();
  wmove(win, 0, 0);
  wattron(win, A_BOLD);
  printCenter(win, questionElements[0], 2);
  wattroff(win, A_BOLD);

  vector<string> inputs = {""};
  vector<int> isInputBox = {1, 0}; // first option is an output box
  vector<string> options = {"Text answer:" ,"NEXT"};
  navigateInput(win, options, inputs, isInputBox, 1, 2);

  // while the input is empty
  while(!inputs[0].size())
    navigateInput(win, options, inputs, isInputBox, 1, 2, "Answer not entered", 6);

  UserAnswer answer;
  answer.text = inputs[0];

  return question->getPoints(answer);
}

double UISolveQuiz::solveQOrdering(WINDOW * win, const Question * question){
  // question elements contains question and choices
  vector<string> questionElements = question->getQuestion();
  wmove(win, 0, 0);
  wattron(win, A_BOLD);
  printCenter(win, questionElements[0], 2);
  wattroff(win, A_BOLD);

  printCenter(win, "Select order:", 2);

  auto begIt = questionElements.begin();
  auto endIt = questionElements.end();
  vector<string> choices(begIt+1, endIt);
  vector<int> order;
  // create default order based on the number of choices
  for(int i = 0; i < (int)choices.size(); i++){
    order.push_back(i);
  }
  vector<string> options = {"NEXT", "BACK", "EXIT TO MAIN MENU"};
  navigateOrder(win, choices, order, options, 1, 4);

  UserAnswer answer;
  answer.vec = order;

  return question->getPoints(answer);
}
