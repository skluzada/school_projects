#include "WebApp.h"
#include "kmp.h"
#include "boolExp.h"
#include <Wt/WBreak.h>
#include <Wt/WFlags.h>
#include <Wt/WAny.h>
#include <Wt/WTableCell.h>
#include <memory>
#include <sstream>
#include <fstream>
#include <dirent.h>
using namespace Wt;
using namespace std;

double getTimeStamp(){
  struct timeval tv;
  gettimeofday (&tv, NULL);
  return tv.tv_sec + tv.tv_usec/1000000.0;
}

WebApp::WebApp(const WEnvironment & env)
: WApplication(env){
  setTitle("Boolean model");
  // this->root()->setContentAlignment(AlignmentFlag::Center);
  text = root()->addWidget(make_unique<WText>("Enter boolean expression (&=AND, |=OR, !=NEG):"));
  root()->addWidget(make_unique<WBreak>());
  input = root()->addWidget(make_unique<WLineEdit>());
  input->setFocus();
  input->setTextSize(50);
  input->setMargin(3, Side::Top);
  button = root()->addWidget(make_unique<WPushButton>("Search"));
  button->setMargin(3, Side::Left);
  container = root()->addWidget(make_unique<WContainerWidget>());
  button->clicked().connect(this, &WebApp::search);
}

void WebApp::search(){
  container->clear();
  container->addWidget(make_unique<WBreak>());
  double timeStamp = getTimeStamp();
  ifstream infile;
  string boolExpString = input->text().narrow();
  booleanExpression boolExp(boolExpString);
  string datasetPath = "/home/skluzada/Downloads/BI-VWM/Project/dataset/";
  string filePath, out, line;
  bool flag = 0;
  int fileIndex = -2;
  DIR * dir;
  struct dirent * ent;
  if ((dir = opendir (datasetPath.c_str())) != NULL){
    while((ent = readdir(dir)) != NULL){
      filePath = datasetPath + ent->d_name;
      infile.open(filePath);
      std::stringstream buffer;
      buffer << infile.rdbuf();
      bool res = boolExp.evaluate_KMP(buffer.str());
      // bool res = boolExp.evaluate_invertedIndex(index, fileIndex);
      if (res == 1){
        if (flag == 0){
          container->addWidget(make_unique<WBreak>());
          container->addWidget(make_unique<WText>(boolExpString + " found at:"));
          flag = 1;
        }
        container->addWidget(make_unique<WBreak>());
        container->addWidget(make_unique<WText>(std::string("File: ") + ent->d_name));
      }
      infile.close();
      fileIndex++;
    }
  }

  if (flag == 0){
    container->addWidget(make_unique<WBreak>());
    container->addWidget(make_unique<WText>(boolExpString + " not found."));
  }

  double time = getTimeStamp() - timeStamp;
  container->addWidget(make_unique<WBreak>());
  container->addWidget(make_unique<WText>("Execution time = " + to_string(time)));

}
