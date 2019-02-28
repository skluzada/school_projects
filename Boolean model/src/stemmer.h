#pragma once
#include <string>
#include <utility>
#include <regex>
#include <vector>
#include <string>

class Stemmer{
public:
  Stemmer(){
    setRules();
    setRegex();
  }
  // my implementation of Porter Stemmer based on the original article
  // URL: https://tartarus.org/martin/PorterStemmer/def.txt
  std::string stemmer(const std::string & w);
private:
  // set all the rules used in the Stemmer
  void setRules();
  // set all the regex patterns used in the Stemmer
  void setRegex();
  // returns word with transformed suffix based on the passed rule
  std::string transform(const std::string & word, const std::pair<std::string, std::string> & rule);
  // returns true if suffix of the word matches the passed regex
  bool regRule(const std::string & word, const std::string & regex);
  // returns true if the word matches the passed regex
  bool regCond(const std::string & word, const std::string & regex);
  // returns true if the word matches the passed regex after transforming it's suffix
  bool regCond(const std::string & word, const std::string & regex, const std::pair<std::string, std::string> & rule);
  std::vector<std::pair<std::string, std::string>> rules_1a;
  std::vector<std::pair<std::string, std::string>> rules_1b;
  std::vector<std::pair<std::string, std::string>> rules_2;
  std::vector<std::pair<std::string, std::string>> rules_3;
  std::vector<std::pair<std::string, std::string>> rules_4;
  std::vector<std::pair<std::string, std::string>> rules_5a;
  std::vector<std::pair<std::string, std::string>> rules_5b;
  std::string con; // consontant
  std::string vow; // vowel
  std::string C; // consontants
  std::string V; // vowels
  std::string mgr0; // [C](VC)^m, m>0
  std::string meq1; // [C](VC)^m[V], m=1
  std::string mgr1; // [C](VC)^m, m>1
  std::string hasvow; // contains a vowel
  std::string doucon; // ends with a double consontant
  std::string cvc; // ends cvc, where the second c is not W, X or Y
};
