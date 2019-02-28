#include "stemmer.h"
#include <iostream>
using namespace std;

void Stemmer::setRegex(){
  con = "[^aeiou]";
  vow = "[aeiouy]";
  C = con + "[^aeiouy]*";
  V = vow + "[aeiou]*";
  mgr0 = "^(" + C + ")?" + V + C + ".*";
  meq1 = "^(" + C + ")?" + V + C + "(" + V + ")?$";
  mgr1 = "^(" + C + ")?" + V + C + V + C + ".*";
  hasvow = ".*" + vow + ".*";
  doucon = ".*(" + con + ")\\1$";
  cvc = ".*(" + con + vow + "[^aeiouywx])$";
}

void Stemmer::setRules(){
  rules_1a.push_back(std::make_pair("sses", "ss"));
  rules_1a.push_back(std::make_pair("ies", "i"));
  rules_1a.push_back(std::make_pair("ss", "ss"));
  rules_1a.push_back(std::make_pair("s", ""));

  rules_1b.push_back(std::make_pair("eed", "ee"));
  rules_1b.push_back(std::make_pair("ed", ""));
  rules_1b.push_back(std::make_pair("ing", ""));
  rules_1b.push_back(std::make_pair("at", "ate"));
  rules_1b.push_back(std::make_pair("bl", "ble"));
  rules_1b.push_back(std::make_pair("iz", "ize"));
  rules_1b.push_back(std::make_pair("", "e"));

  rules_2.push_back(std::make_pair("ational", "ate"));
  rules_2.push_back(std::make_pair("tional", "tion"));
  rules_2.push_back(std::make_pair("enci", "ence"));
  rules_2.push_back(std::make_pair("anci", "ance"));
  rules_2.push_back(std::make_pair("izer", "ize"));
  rules_2.push_back(std::make_pair("abli", "able"));
  rules_2.push_back(std::make_pair("alli", "al"));
  rules_2.push_back(std::make_pair("entli", "ent"));
  rules_2.push_back(std::make_pair("eli", "e"));
  rules_2.push_back(std::make_pair("ousli", "ous"));
  rules_2.push_back(std::make_pair("ization", "ize"));
  rules_2.push_back(std::make_pair("ation", "ate"));
  rules_2.push_back(std::make_pair("ator", "ate"));
  rules_2.push_back(std::make_pair("alism", "al"));
  rules_2.push_back(std::make_pair("iveness", "ive"));
  rules_2.push_back(std::make_pair("fulness", "ful"));
  rules_2.push_back(std::make_pair("ousness", "ous"));
  rules_2.push_back(std::make_pair("aliti", "al"));
  rules_2.push_back(std::make_pair("iviti", "ive"));
  rules_2.push_back(std::make_pair("biliti", "ble"));

  rules_3.push_back(std::make_pair("icate", "ic"));
  rules_3.push_back(std::make_pair("ative", ""));
  rules_3.push_back(std::make_pair("alize", "al"));
  rules_3.push_back(std::make_pair("iciti", "ic"));
  rules_3.push_back(std::make_pair("ical", "ic"));
  rules_3.push_back(std::make_pair("ful", ""));
  rules_3.push_back(std::make_pair("ness", ""));

  rules_4.push_back(std::make_pair("al", ""));
  rules_4.push_back(std::make_pair("ance", ""));
  rules_4.push_back(std::make_pair("ence", ""));
  rules_4.push_back(std::make_pair("er", ""));
  rules_4.push_back(std::make_pair("ic", ""));
  rules_4.push_back(std::make_pair("able", ""));
  rules_4.push_back(std::make_pair("ible", ""));
  rules_4.push_back(std::make_pair("ant", ""));
  rules_4.push_back(std::make_pair("ement", ""));
  rules_4.push_back(std::make_pair("ment", ""));
  rules_4.push_back(std::make_pair("ent", ""));
  rules_4.push_back(std::make_pair("ion", ""));
  rules_4.push_back(std::make_pair("ou", ""));
  rules_4.push_back(std::make_pair("ism", ""));
  rules_4.push_back(std::make_pair("ate", ""));
  rules_4.push_back(std::make_pair("iti", ""));
  rules_4.push_back(std::make_pair("ous", ""));
  rules_4.push_back(std::make_pair("ive", ""));
  rules_4.push_back(std::make_pair("ize", ""));

  rules_5a.push_back(std::make_pair("e", ""));

  rules_5b.push_back(std::make_pair("ll", "l"));
}

std::string Stemmer::transform(const std::string & word, const std::pair<std::string, std::string> & rule){
  return word.substr(0, word.size() - rule.first.size()) + rule.second;
}

bool Stemmer::regRule(const std::string & word, const std::string & regex){
  return std::regex_search(word, std::regex("(" + regex + ")$"));
}

bool Stemmer::regCond(const std::string & word, const std::string & regex){
  return std::regex_match(word, std::regex(regex));
}

bool Stemmer::regCond(const std::string & word, const std::string & regex, const std::pair<std::string, std::string> & rule){
  if(!regRule(word, rule.first))
    return false;
  if(!regCond(word.substr(0, word.size() - rule.first.size()), regex))
    return false;
  return true;
}

std::string Stemmer::stemmer(const std::string & w){
  std::string word = w;
  if (word.size() < 3)
    return word;

  for (size_t i = 0; i < rules_1a.size(); i++){
    if (regRule(word, rules_1a[i].first)){
      word = transform(word, rules_1a[i]);
      break;
    }
  }

  bool flag = 0;
  if (regCond(word, mgr0, rules_1b[0])){
    word = transform(word, rules_1b[0]);
  }
  else if (regCond(word, hasvow)){
    if (regCond(word, mgr0, rules_1b[1])){
      word = transform(word, rules_1b[1]);
      flag = 1;
    }
    else if (regCond(word, mgr0, rules_1b[2])){
      word = transform(word, rules_1b[2]);
      flag = 1;
    }
  }

  if (flag == 1){
    if (regRule(word, rules_1b[3].first))
      word = transform(word, rules_1b[3]);
    else if (regRule(word, rules_1b[4].first))
      word = transform(word, rules_1b[4]);
    else if (regRule(word, rules_1b[5].first))
      word = transform(word, rules_1b[5]);
    else if (regCond(word, doucon) && !(regCond(word, ".*l$") ||
             regCond(word, ".*s$") || regCond(word, ".*z$")))
      word = word.substr(0, word.size()-1);
    else if (regCond(word, meq1) && regCond(word, cvc))
      word = transform(word, rules_1b[6]);
  }

  std::vector<std::pair<std::string, std::string>> rules_1c;
  rules_1c.push_back(std::make_pair("y", "i"));

  if (regCond(word, hasvow, rules_1c[0]))
    word = transform(word, rules_1c[0]);

  if(regCond(word, mgr0)){
    for (size_t i = 0; i < rules_2.size(); i++){
      if (regCond(word, mgr0, rules_2[i])){
        word = transform(word, rules_2[i]);
        break;
      }
    }
  }

  if(regCond(word, mgr0)){
    for (size_t i = 0; i < rules_3.size(); i++){
      if (regCond(word, mgr0, rules_3[i])){
        word = transform(word, rules_3[i]);
        break;
      }
    }
  }

  if(regCond(word, mgr1)){
    for (size_t i = 0; i < rules_4.size(); i++){
      if(!regCond(word, mgr1))
        break;
      if ((i == 11) && regCond(word, mgr1) && (regCond(word, ".*s$") || regCond(word, ".*t$"))){
        word = transform(word, rules_4[i]);
        break;
      }
      else if (regCond(word, mgr1, rules_4[i])){
        word = transform(word, rules_4[i]);
        break;
      }
    }
  }

  if (regCond(word, mgr1)){
    if (regCond(word, mgr1, rules_5a[0]))
      word = transform(word, rules_5a[0]);
  }
  else if (regCond(word, meq1, rules_5a[0]) && !regCond(word, ".*o$"))
    word = transform(word, rules_5a[0]);

  if (regCond(word, mgr1) && regCond(word, ".*ll$"))
    word = transform(word, rules_5b[0]);

  return word;
}
