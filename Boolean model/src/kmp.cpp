#include "kmp.h"

void KMP_table(const std::string & word, std::vector<int> & table){
  size_t wordLen = word.length();
  int cnd = 0;
  for(size_t pos = 1; pos < wordLen; ++pos){
    if (word[pos] == word[cnd]){
      table[pos] = table[cnd];
      cnd++;
    }
    else {
      table[pos] = cnd;
      cnd = table[cnd];
      while (cnd >= 0 && word[pos] != word[cnd])
        cnd = table[cnd];
      cnd++;
    }
  }
}

bool KMP_search(const std::string & text, const std::string & word){
  size_t wordLen = word.length();
  size_t textLen = text.length();
  if (wordLen == 0 || textLen == 0)
    return false;
  std::vector<int> table(wordLen, -1);
  KMP_table(word, table);

  int k = 0;
  for(size_t i = 0; i < textLen; ++i){
    if (word[k] == text[i]){
      k++;
      if (k == (int)wordLen)
        return true;
    }
    else {
      k = table[k];
      if (k < 0)
        k++;
    }
  }
  return false;
}
