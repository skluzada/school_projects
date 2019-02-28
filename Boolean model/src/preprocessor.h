#pragma once
#include "stemmer.h"
#include <string>
#include <vector>

class Preprocessor{
public:
  Preprocessor();
private:
  void preprocess(const std::string & fileName);
  // load stopwords from a file
  void getStopwords(const std::string & stopwordsFileName);
  bool isStopword(const std::string & word);
  bool isWhiteSpace(const char ch);
  bool isPunctuation(const char ch);
  std::vector<std::string> stopwords;
  Stemmer s;
};
