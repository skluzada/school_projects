#pragma once
#include "index.h"
#include <Wt/WApplication.h>
#include <Wt/WContainerWidget.h>
#include <Wt/WText.h>
#include <Wt/WLineEdit.h>
#include <Wt/WPushButton.h>
#include <sys/time.h>

double getTimeStamp();

class WebApp : public Wt::WApplication{
public:
  WebApp(const Wt::WEnvironment & env);
private:
  Wt::WText * text;
  Wt::WLineEdit * input;
  Wt::WPushButton * button;
  Wt::WContainerWidget * container;
  Index index;
  void search();
};
