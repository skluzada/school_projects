#include "uiCreateQuiz.h"
#include <cstdlib>
using namespace std;

UICreateQuiz::UICreateQuiz(QuizHandler & handler)
: handlerPtr(&handler), quiz(NULL){}

void UICreateQuiz::show(){
  quiz = NULL; // just to make sure
  createQuiz();
  quiz = NULL; // just to make sure
}

void UICreateQuiz::createQuiz(){
  WINDOW * win;
  win = createWindowMenu();

  string header = "CREATE QUIZ";
  printHeader(win, header);

  int choice;
  vector<string> inputs = {"", ""};
  // if quiz is set use it's title and topic instead of empty inputs
  if (quiz != NULL)
    inputs = {quiz->getTitle(), quiz->getTopic()};

  vector<int> isInputBox = {1, 1, 0, 0}; // first two options are input boxes
  vector<string> options = {"Title:", "Topic:", "NEXT", "EXIT TO MAIN MENU"};
  choice = navigateInput(win, options, inputs, isInputBox);

  int err = 1;
  while(err){
    switch(choice){
      case 3: // "NEXT"
        // if the title input box is empty
        if (!(inputs[0].size() > 0))
          choice = navigateInput(win, options, inputs, isInputBox, 3, 0, "Title not filled", 9);

        // if the quiz handler already contains a quiz with the given title
        else if (!handlerPtr->isUniqueTitle(inputs[0]))
          choice = navigateInput(win, options, inputs, isInputBox, 3, 0, "Title not unique", 9);

        // if the topic input box is empty
        else if (!(inputs[1].size() > 0))
          choice = navigateInput(win, options, inputs, isInputBox, 3, 0,  "Topic not filled", 9);

        else {
          err = 0;
          destroyWindow(win);
          // if the quiz is NULL create it
          if (quiz == NULL)
            quiz = new Quiz(inputs[0], inputs[1]);
          // else just change the title and topic
          else{
            quiz->setTitle(inputs[0]);
            quiz->setTopic(inputs[1]);
          }
          createQuizMenu();
        }
        break;
      case 4: // "EXIT TO MAIN MENU"
        err = 0;
        delete quiz;
        quiz = NULL;
        destroyWindow(win);
        return;
    }
  }
  return;
}

void UICreateQuiz::createQuizMenu(){
  WINDOW * win;
  win = createWindowMenu();

  string header = "CREATE QUIZ";
  printHeader(win, header);

  wattron(win, A_BOLD);
  printCenter(win, quiz->getTitle(), 2);
  wattroff(win, A_BOLD);

  printCenter(win, "Topic: " + quiz->getTopic(), 1);
  printCenter(win, "Number of questions: " + to_string(quiz->getMaxNumberOfQuestions()), 1);

  int choice;
  vector<string> options {"ADD QUESTION", "NEXT", "BACK", "EXIT TO MAIN MENU"};
  choice = navigateOptions(win, options, 1, 4);

  int err = 1;
  while (err){
    switch(choice){
      case 1: // "ADD QUESTIOn"
        err = 0;
        destroyWindow(win);
        selectQuestion();
        break;
      case 2: // "NEXT"
        // if there are no questions in the quiz
        if (!quiz->getMaxNumberOfQuestions()){
          printError(win, "No questions", 7);
          choice = navigateOptions(win, options, 2, 4);
        }
        else {
          err = 0;
          destroyWindow(win);
          chooseNumberOfQuestions();
        }
        break;
      case 3: // "BACK"
        err = 0;
        destroyWindow(win);
        createQuiz();
        break;
      case 4: // "EXIT TO MAIN MENU"
        err = 0;
        delete quiz;
        quiz = NULL;
        destroyWindow(win);
        return;
    }
  }
}

void UICreateQuiz::selectQuestion(){
  WINDOW * win;
  win = createWindowMenu();

  string header = "CREATE QUESTION";
  printHeader(win, header);

  wattron(win, A_UNDERLINE);
  printCenter(win, "Select a type of question:", 2);
  wattroff(win, A_UNDERLINE);

  int choice;
  vector<string> options {"Single Choice Question", "Multiple Choice Question",
    "Numeric Question", "Text Question", "Ordering Question", "BACK", "EXIT TO MAIN MENU"};
  choice = navigateOptions(win, options, 1, 2);

  // based on the chosen type of question an appropriate "createAnswer" function is passed
  switch(choice){
    case 1: // "Single Choice Question"
      destroyWindow(win);
      createQuestion([this](std::string & question){this->createASingleChoice(question);});
      break;
    case 2: // "Multiple Choice Question"
      destroyWindow(win);
      createQuestion([this](std::string & question){this->createAMultipleChoice(question);});
      break;
    case 3: // "Numeric Question"
      destroyWindow(win);
      createQuestion([this](std::string & question){this->createANumeric(question);});
      break;
    case 4: // "Text Question"
      destroyWindow(win);
      createQuestion([this](std::string & question){this->createAText(question);});
      break;
    case 5: // "Ordering Question"
      destroyWindow(win);
      createQuestion([this](std::string & question){this->createAOrder(question);});
      break;
    case 6: // "BACK"
      destroyWindow(win);
      createQuizMenu();
      break;
    case 7: // "EXIT TO MAIN MENU"
      delete quiz;
      quiz = NULL;
      destroyWindow(win);
      return;
  }
}

void UICreateQuiz::createQuestion(std::function<void(std::string&)> createAnswerFunction, const std::vector<std::string> & input){
  WINDOW * win;
  win = createWindowMenu();

  string header = "CREATE QUESTION";
  printHeader(win, header);

  int choice;
  vector<string> inputs = input;
  vector<int> isInputBox = {1, 0, 0, 0}; // first option is an input box
  vector<string> options = {"Question:", "NEXT", "BACK", "EXIT TO MAIN MENU"};
  choice = navigateInput(win, options, inputs, isInputBox);

  int err = 1;
  while(err){ // while err is set to true we can't proceed
    switch(choice){
      case 2: // "NEXT"
        // if the question input box is empty
        if (!inputs[0].size())
          choice = navigateInput(win, options, inputs, isInputBox, 2, 0, "Question not filled");

        else {
          err = 0;
          destroyWindow(win);
          // passed function that creates an answer appropriate to the type of question
          createAnswerFunction(inputs[0]);
        }
        break;
      case 3: // "BACK"
        err = 0;
        destroyWindow(win);
        selectQuestion();
        break;
      case 4: // "EXIT TO MAIN MENU"
        err = 0;
        delete quiz;
        quiz = NULL;
        destroyWindow(win);
        return;
    }
  }
}

void UICreateQuiz::finishQuestion(Question * question){
  WINDOW * win;
  win = createWindowMenu();

  string header = "CREATE QUESTION";
  printHeader(win, header);

  int choice;
  vector<string> inputs = {"1", "0"};
  vector<int> isInputBox = {1, 1, 0, 0}; // first two options are input boxes
  vector<string> options = {"Points for correct answer: (default = 1)" ,"Difficulty of question: (default = 0, min = 0, max = 10)", "NEXT", "EXIT TO MAIN MENU"};
  choice = navigateInput(win, options, inputs, isInputBox);

  double points = 1;
  int difficulty = 0;
  switch(choice){
    case 3: // "NEXT"
      points = atof(inputs[0].c_str()); //convert string to double
      // if the atof() function fails, points = 1
      if (points == 0)
        points = 1;
      question->setMaxPoints(points);

      difficulty = atoi(inputs[1].c_str()); // convert string to int
      // if the atof() function fails, difficulty = 0
      if (difficulty > 10)
        difficulty = 10;
      if (difficulty < 0)
        difficulty = 0;

      question->setDifficutly(difficulty);
      quiz->addQuestion(question); // add question to the quiz
      destroyWindow(win);
      createQuizMenu();
      break;
    case 4: // "EXIT TO MAIN MENU"
      delete quiz;
      quiz = NULL;
      delete question;
      destroyWindow(win);
      return;
  }
}

void UICreateQuiz::createASingleChoice(std::string & question){
  // create choices for single choice question
  createAChoices(question, 1);
}

void UICreateQuiz::createAMultipleChoice(std::string & question){
  // create choices for multiple choice question
  createAChoices(question, 0);
}

void UICreateQuiz::createAChoices(std::string & question, int singleChoice, int ordering){
  WINDOW * win;
  win = createWindowMenu();

  string header = "CREATE QUESTION";
  printHeader(win, header);

  int choice;
  vector<string> inputs = {""};
  vector<int> isInputBox = {1, 0, 0, 0, 0}; // first option is an input box
  vector<string> options = {"Choice: (min = 2, max = 8)" , "ADD CHOICE", "NEXT", "BACK", "EXIT TO MAIN MENU"};
  choice = navigateInput(win, options, inputs, isInputBox);

  vector<string> choices;
  int err = 1;
  while(err){ // while err is set to true we can't proceed
    switch(choice){
      case 2: // "ADD CHOICE"
      // if the choice input box is empty
      if (!inputs[0].size())
        choice = navigateInput(win, options, inputs, isInputBox, 2, 0, "Choice not entered");
      // if max number of choices have been added already
      else if(choices.size() == 8)
        choice = navigateInput(win, options, inputs, isInputBox, 2, 0, "Maximum number of choices entered");
      else{
        choices.push_back(inputs[0]);
        inputs[0] = "";
        printGreen(win, "Choice added", 4);
        choice = navigateInput(win, options, inputs, isInputBox);
      }
      break;
      case 3: // "NEXT"
        // if min number of choices have't been added yet
        if (choices.size() < 2)
          choice = navigateInput(win, options, inputs, isInputBox, 3, 0, "Two or more choices not entered");
        // if an ordering question is supposed to be created
        else if (ordering){
          err = 0;
          destroyWindow(win);
          createAOrderChooseOrder(question, choices);
        }
        // if single choice or multiple choice question is supposed to be created
        else{
          err = 0;
          destroyWindow(win);
          createAChoicesChooseCorrect(question, singleChoice, choices);
        }
        break;
      case 4: // "BACK"
        err = 0;
        destroyWindow(win);
        {vector<string> input = {question};
        // pass appropriate create answer function
        if (ordering)
          createQuestion([this](std::string & question){this->createAOrder(question);}, input);
        else if (singleChoice)
          createQuestion([this](std::string & question){this->createASingleChoice(question);}, input);
        else
          createQuestion([this](std::string & question){this->createAMultipleChoice(question);}, input);}
        break;
      case 5: // "EXIT TO MAIN MENU"
        err = 0;
        delete quiz;
        quiz = NULL;
        destroyWindow(win);
        return;
    }
  }
}

void UICreateQuiz::createAChoicesChooseCorrect(std::string & question, int singleChoice, std::vector<std::string> & choices){
  WINDOW * win;
  win = createWindowMenu();

  string header = "CREATE QUESTION";
  printHeader(win, header);

  wattron(win, A_UNDERLINE);
  if (singleChoice)
    printCenter(win, "Choose correct choice:", 2);
  else
    printCenter(win, "Choose correct choices:", 2);
  wattroff(win, A_UNDERLINE);

  int choice;
  vector<int> chosen (choices.size(), 0);
  vector<string> options = {"NEXT", "BACK", "EXIT TO MAIN MENU"};
  choice = navigateChoices(win, choices, chosen, options, singleChoice, 1, 3);

  int err = 1;
  Answer * answer;
  Question * quest;
  while(err){ // while err is set to true we can't proceed
    switch(choice){
      case 1: // "NEXT"
        // if single choice answer exactly one choice must be set to true
        if (singleChoice){
          for(int i = 0; i < (int)chosen.size(); i++){
            if (chosen[i] == 1)
              err = 0;
          }
          // if one exactly choice is se to true
          if (err == 0){
            answer = new AChoices(chosen);
            quest = new QSingleChoice(question, choices, answer);
            destroyWindow(win);
            finishQuestion(quest);
          }
          // if no choice is set to true
          else {
            printError(win, "No choice selected", 4 + choices.size());
            choice = navigateChoices(win, choices, chosen, options, singleChoice, 1+choices.size(), 3);
          }
        }
        // if multiple choice answer any number of choices can be set to true
        else {
          err = 0;
          answer = new AChoices(chosen);
          quest = new QMultipleChoice(question, choices, answer);
          destroyWindow(win);
          finishQuestion(quest);
        }
        break;
      case 2: // "BACK"
        err = 0;
        destroyWindow(win);
        createAChoices(question, singleChoice);
        break;
      case 3: // "EXIT TO MAIN MENU"
        err = 0;
        delete quiz;
        quiz = NULL;
        destroyWindow(win);
        return;
    }
  }
}

void UICreateQuiz::createAOrder(std::string & question){
  // create choices for ordering question
  createAChoices(question, 0, 1);
}

void UICreateQuiz::createAOrderChooseOrder(std::string & question, std::vector<std::string> & choices){
  WINDOW * win;
  win = createWindowMenu();

  string header = "CREATE QUESTION";
  printHeader(win, header);

  wattron(win, A_UNDERLINE);
  printCenter(win, "Choose correct order:", 2);
  wattroff(win, A_UNDERLINE);

  int choice;
  vector<int> order;
  // create default order based on the number of choices
  for(int i = 0; i < (int)choices.size(); i++){
    order.push_back(i);
  }
  vector<string> options = {"NEXT", "BACK", "EXIT TO MAIN MENU"};
  choice = navigateOrder(win, choices, order, options, 1, 3);

  Answer * answer;
  Question * quest;
  switch(choice){
    case 1: // "NEXT"
      answer = new AOrder(order);
      quest = new QOrdering(question, choices, answer);
      destroyWindow(win);
      finishQuestion(quest);
      break;
    case 2: // "BACK"
      destroyWindow(win);
      createAChoices(question, 0, 1);
      break;
    case 3: // "EXIT TO MAIN MENU"
      delete quiz;
      quiz = NULL;
      destroyWindow(win);
      return;
  }
}

void UICreateQuiz::createANumeric(std::string & question){
  WINDOW * win;
  win = createWindowMenu();

  string header = "CREATE QUESTION";
  printHeader(win, header);

  wattron(win, A_BOLD);
  printCenter(win, question, 2);
  wattroff(win, A_BOLD);
  wattron(win, A_UNDERLINE);
  printCenter(win, "Select a type of numeric answer:", 2);
  wattroff(win, A_UNDERLINE);

  int choice;
  vector<string> options {"Number", "Interval", "Residue class",
    "BACK", "EXIT TO MAIN MENU"};
  choice = navigateOptions(win, options, 1, 4);

  switch(choice){
    case 1: // "Number answer"
      destroyWindow(win);
      createANumber(question);
      break;
    case 2: // "Interval answer"
      destroyWindow(win);
      createAInterval(question);
      break;
    case 3: // "Residue Class answer"
      destroyWindow(win);
      createAResidueClass(question);
      break;
    case 4: // "BACK"
      destroyWindow(win);
      {vector<string> input = {question};
      createQuestion([this](std::string & question){this->createANumeric(question);}, input);}
      break;
    case 5: // "EXIT TO MAIN MENU"
      delete quiz;
      quiz = NULL;
      destroyWindow(win);
      return;
  }
}

void UICreateQuiz::createANumber(std::string & question){
  WINDOW * win;
  win = createWindowMenu();

  string header = "CREATE QUESTION";
  printHeader(win, header);

  int choice;
  vector<string> inputs = {""};
  vector<int> isInputBox = {1, 0, 0, 0}; // first option is an output box
  vector<string> options = {"Number:" ,"NEXT", "BACK", "EXIT TO MAIN MENU"};
  choice = navigateInput(win, options, inputs, isInputBox);

  Answer * answer;
  Question * quest;
  double number = 0.0;
  int err = 1;
  while(err){ // while err is set to true we can't proceed
    switch(choice){
      case 2: // "NEXT"
        number = atof(inputs[0].c_str()); // convert string to double
        // if atof() fails (returns 0) and user input was not a 0
        if (number == 0 && inputs[0] != "0")
          choice = navigateInput(win, options, inputs, isInputBox, 2, 0, "Number not entered");
        else{
          err = 0;
          answer = new ANumber(number);
          quest = new QNumeric(question, answer);
          destroyWindow(win);
          finishQuestion(quest);
        }
        break;
      case 3: // "BACK"
        err = 0;
        destroyWindow(win);
        createANumeric(question);
        break;
      case 4: // "EXIT TO MAIN MENU"
        err = 0;
        delete quiz;
        quiz = NULL;
        destroyWindow(win);
        return;
    }
  }
}

void UICreateQuiz::createAInterval(std::string & question){
  WINDOW * win;
  win = createWindowMenu();

  string header = "CREATE QUESTION";
  printHeader(win, header);

  int choice;
  vector<string> inputs = {""};
  vector<int> isInputBox = {1, 0, 0, 0};
  vector<string> options = {"Interval: (open = (.,.), bounded = <.,.>)" ,"NEXT", "BACK", "EXIT TO MAIN MENU"};
  choice = navigateInput(win, options, inputs, isInputBox);

  Answer * answer;
  Question * quest;
  string input;
  string leftBoundString = "";
  string rightBoundString = "";
  double leftBound = 0.0;
  double rightBound = 0.0;
  bool leftBounded, rightBounded, comma = false;
  int err = 1;
  while(err){ // while err is set to true we can't proceed
    switch(choice){
      case 2:
        input = deleteWhitespaces(inputs[0]); // delete white spaces from the user input

        // if input size is lower than 5
        if (input.size() < 5){
          choice = navigateInput(win, options, inputs, isInputBox, 2, 0, "Interval not entered");
          break;
        }

        // first char of user input must be '(' or '<'
        if (input[0] == '(')
          leftBounded = false;
        else if (input[0] == '<')
          leftBounded = true;
        else{
          choice = navigateInput(win, options, inputs, isInputBox, 2, 0, "Left bracket not entered");
          break;
        }

        // last char of user input must be ')' or '<'
        if (input[input.size()-1] == ')')
          rightBounded = false;
        else if (input[input.size()-1] == '>')
          rightBounded = true;
        else{
          choice = navigateInput(win, options, inputs, isInputBox, 2, 0, "Right bracket not entered");
          break;
        }

        // erase first and last char from user input
        input = input.substr(1, input.size()-2);
        leftBoundString = "";
        rightBoundString = "";
        comma = false;
        // before comma add char to leftBoundString, after comma is read add char to rightBoundString
        for (size_t i = 0; i < input.size(); i++){
          if (input[i] == ','){
            comma = true;
          }
          else if(comma){
            rightBoundString+=input[i];
          }
          else{
            leftBoundString+=input[i];
          }
        }

        // if user input didn't contain a comma
        if(comma == false){
          choice = navigateInput(win, options, inputs, isInputBox, 2, 0, "Comma not entered");
          break;
        }

        leftBound = atof(leftBoundString.c_str()); // convert string to double
        // if atof() fails (returns 0) and user input was not a 0
        if (leftBound == 0 && leftBoundString != "0"){
          choice = navigateInput(win, options, inputs, isInputBox, 2, 0, "Left bound not entered");
          break;
        }

        rightBound = atof(rightBoundString.c_str()); // convert string to double
        // if atof() fails (returns 0) and user input was not a 0
        if (rightBound == 0 && rightBoundString != "0"){
          choice = navigateInput(win, options, inputs, isInputBox, 2, 0, "Right bound not entered");
          break;
        }

        // if the left bound number is greater thatn the right bound number
        if (leftBound > rightBound){
          choice = navigateInput(win, options, inputs, isInputBox, 2, 0, "Left bound greater than right bound");
          break;
        }

        // if none of the previous if statements were true, we can create an interval answer
        err = 0;
        answer = new AInterval(leftBound, rightBound, leftBounded, rightBounded);
        quest = new QNumeric(question, answer);
        destroyWindow(win);
        finishQuestion(quest);
        break;
      case 3: // "BACK"
        err = 0;
        destroyWindow(win);
        createANumeric(question);
        break;
      case 4: // "EXIT TO MAIN MENU"
        err = 0;
        delete quiz;
        quiz = NULL;
        destroyWindow(win);
        return;
    }
  }
}

void UICreateQuiz::createAResidueClass(std::string & question){
  WINDOW * win;
  win = createWindowMenu();

  string header = "CREATE QUESTION";
  printHeader(win, header);

  int choice;
  vector<string> inputs = {""};
  vector<int> isInputBox = {1, 0, 0, 0}; // first option is an input box
  vector<string> options = {"Modulus:" ,"NEXT", "BACK", "EXIT TO MAIN MENU"};
  choice = navigateInput(win, options, inputs, isInputBox);

  Answer * answer;
  Question * quest;
  int modulus = 0;
  int err = 1;
  while(err){ // while err is set to true we can't proceed
    switch(choice){
      case 2: // "NEXT"
        modulus = atoi(inputs[0].c_str()); // convert string to int
        // if atoi() failed and user input was not a 0
        if (modulus == 0 && inputs[0] != "0")
          choice = navigateInput(win, options, inputs, isInputBox, 2, 0, "Number not entered");
        else{
          err = 0;
          answer = new AResidueClass(modulus);
          quest = new QNumeric(question, answer);
          destroyWindow(win);
          finishQuestion(quest);
        }
        break;
      case 3: // "BACK"
        err = 0;
        destroyWindow(win);
        createANumeric(question);
        break;
      case 4: // "EXIT TO MAIN MENU"
        err = 0;
        delete quiz;
        quiz = NULL;
        destroyWindow(win);
        return;
    }
  }
}

void UICreateQuiz::createAText(std::string & question){
  WINDOW * win;
  win = createWindowMenu();

  string header = "CREATE QUESTION";
  printHeader(win, header);

  int choice;
  vector<string> inputs = {""};
  vector<int> isInputBox = {1, 0, 0, 0, 0};
  vector<string> options = {"Text:" , "ADD CORRECT OPTION", "NEXT", "BACK", "EXIT TO MAIN MENU"};
  choice = navigateInput(win, options, inputs, isInputBox);

  vector<string> correctAnswers;
  Answer * answer;
  Question * quest;
  int err = 1;
  while(err){ // while err is set to true we can't proceed
    switch(choice){
      case 2: // "ADD CORRECT OPTION"
      // is the text input box is empty
      if (!inputs[0].size())
        choice = navigateInput(win, options, inputs, isInputBox, 2, 0, "Text not entered");
      else{
        correctAnswers.push_back(inputs[0]); // add text to answers vector
        inputs[0] = "";
        printGreen(win, "      Correct option added       ", 4);
        choice = navigateInput(win, options, inputs, isInputBox);
      }
      break;
      case 3: // "NEXT"
        // if the text input box and answers vector are both empty
        if (!inputs[0].size() && !correctAnswers.size())
          choice = navigateInput(win, options, inputs, isInputBox, 3, 0, "Text not entered");
        else{
          err = 0;
          if (inputs[0].size()) // if input not empty insert it
            correctAnswers.push_back(inputs[0]);
          answer = new AText(correctAnswers);
          quest = new QText(question, answer);
          destroyWindow(win);
          finishQuestion(quest);
        }
        break;
      case 4: // "BACK"
        err = 0;
        destroyWindow(win);
        {vector<string> input = {question};
        createQuestion([this](std::string & question){this->createAText(question);}, input);}
        break;
      case 5: // "EXIT TO MAIN MENU"
        err = 0;
        delete quiz;
        quiz = NULL;
        destroyWindow(win);
        return;
    }
  }
}

void UICreateQuiz::chooseNumberOfQuestions(){
  WINDOW * win;
  win = createWindowMenu();

  string header = "CREATE QUIZ";
  printHeader(win, header);

  wattron(win, A_BOLD);
  printCenter(win, quiz->getTitle(), 2);
  wattroff(win, A_BOLD);
  printCenter(win, "Topic: " + quiz->getTopic(), 1);
  printCenter(win, "Number of questions: " + to_string(quiz->getMaxNumberOfQuestions()), 1);

  int choice;
  vector<string> inputs = {to_string(quiz->getNumberOfQuestions())};
  vector<int> isInputBox = {1, 0, 0, 0};
  vector<string> options = {"Number of questions shown in each quiz:" ,"NEXT", "BACK", "EXIT TO MAIN MENU"};
  choice = navigateInput(win, options, inputs, isInputBox, 1, 4);

  int numberOfQuestions = 0;
  int err = 1;
  while(err){ // while err is set to true we can't proceed
    switch(choice){
      case 2: // "NEXT"
        numberOfQuestions = atoi(inputs[0].c_str()); // convert string to int
        // if atoi() failed or user input is 0
        if (numberOfQuestions <= 0){
          choice = navigateInput(win, options, inputs, isInputBox, 2, 4, "Number not entered", 8);
          break;
        }
        // if the entered number is higher than the number of questions in the quiz
        if (numberOfQuestions > quiz->getMaxNumberOfQuestions()){
          choice = navigateInput(win, options, inputs, isInputBox, 2, 4, "Not enough questions in the quiz", 8);
          break;
        }
        err = 0;
        quiz->setNumberOfQuestions(numberOfQuestions);
        destroyWindow(win);
        // if the entered number is equal to the number of questions in the quiz
        if (numberOfQuestions == quiz->getMaxNumberOfQuestions())
          chooseQuestionsOrder(true);
        // if the entered number is lower than the number of questions in the quiz
        else
          chooseQuestionsOrder(false);

        break;
      case 3: // "BACK"
        err = 0;
        destroyWindow(win);
        createQuizMenu();
        break;
      case 4: // "EXIT TO MAIN MENU"
        err = 0;
        delete quiz;
        quiz = NULL;
        destroyWindow(win);
        return;
    }
  }
}

void UICreateQuiz::chooseQuestionsOrder(int maxQuestions){
  WINDOW * win;
  win = createWindowMenu();

  string header = "CREATE QUIZ";
  printHeader(win, header);

  wattron(win, A_BOLD);
  printCenter(win, quiz->getTitle(), 2);
  wattroff(win, A_BOLD);
  printCenter(win, "Topic: " + quiz->getTopic(), 1);
  printCenter(win, "Number of questions: " + to_string(quiz->getMaxNumberOfQuestions()), 1);
  printCenter(win, "Number of questions in each quiz: " + to_string(quiz->getNumberOfQuestions()), 1);

  wattron(win, A_UNDERLINE);
  if (maxQuestions)
    printCenter(win, "Select the order of questions:", 2);
  else
    printCenter(win, "Select how are the questions selected:", 2);
  wattroff(win, A_UNDERLINE);

  int choice;
  vector<string> options;
  if (maxQuestions)
    options.push_back("Default");
  else
    options.push_back("Based on difficulty");

  options.push_back("Random");
  options.push_back("BACK");
  options.push_back("EXIT TO MAIN MENU");

  choice = navigateOptions(win, options, 1, 7);

  switch(choice){
    case 1: // "Default" or "Based on difficulty"
      if (maxQuestions)
        quiz->setOrder("default");
      else
        quiz->setOrder("difficulty");
      handlerPtr->addQuiz(quiz);
      quiz = NULL;
      destroyWindow(win);
      return;
    case 2: // "Random"
      quiz->setOrder("random");
      handlerPtr->addQuiz(quiz);
      quiz = NULL;
      destroyWindow(win);
      return;
    case 3: // "BACK"
      destroyWindow(win);
      chooseNumberOfQuestions();
      break;
    case 4: // "EXIT TO MAIN MENU"
      delete quiz;
      quiz = NULL;
      destroyWindow(win);
      return;
  }
}
