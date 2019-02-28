#include "preprocessor.h"
#include <iostream>
#include <fstream>
#include <algorithm>
#include <sstream>
#include <cctype>
#include <dirent.h>

const std::string datasetPath = "/home/skluzada/Downloads/BI-VWM/Project/dataset/";

Preprocessor::Preprocessor(){
  getStopwords("stopwords");
  DIR * dir;
  struct dirent * ent;
  if ((dir = opendir (datasetPath.c_str())) != NULL){
    while((ent = readdir(dir)) != NULL){
      preprocess(ent->d_name);
    }
  }
}

void Preprocessor::preprocess(const std::string & fileName){
  std::string filePath = datasetPath + fileName;
  std::cout << filePath << std::endl;

  std::ifstream inFile(filePath, std::ios::in);
  std::stringstream buffer;
  buffer << inFile.rdbuf();
  std::string text = buffer.str();
  inFile.close();

  // preprocesedFlag indicates that the file has already been preprocessed
  std::string preprocessedFlag = "xxxpreprocessedxxx\n";
  std::string firstWord = "";
  for(size_t i = 0; i < preprocessedFlag.size(); i++){
    firstWord += text[i];
  }
  if (firstWord == preprocessedFlag)
    return;

  std::ofstream outFile(filePath, std::ios::out | std::ios::trunc);
  outFile << preprocessedFlag;

  size_t textLen = text.size();
  size_t i = 0;
  while (i < textLen){
    if(isPunctuation(text[i])){
      i++;
    }
    else if(isWhiteSpace(text[i])){
      outFile << text[i];
      i++;
      while (isWhiteSpace(text[i])){
        if (text[i] == '\n')
          outFile << text[i];
        i++;
      }
    }
    else {
      std::string word = "";
      while (!isWhiteSpace(text[i])){
        if (!isPunctuation(text[i]))
          word += tolower(text[i]);
        i++;
      }
      if (!isStopword(word)){
        word = s.stemmer(word);
        outFile << word;
      }
    }
  }
  outFile.close();
}

//
void Preprocessor::getStopwords(const std::string & stopwordsFileName){
  std::ifstream stopwordsFile;
  stopwordsFile.open(stopwordsFileName);
  std::string word;
  while(getline(stopwordsFile, word)){
    stopwords.push_back(word);
  }
  stopwordsFile.close();
}

bool Preprocessor::isWhiteSpace(const char ch){
 if(ch == ' ' || ch == '\t' || ch == '\r' ||
    ch == '\n' || ch == '\v' || ch == '\f'){
      return true;
    }
  return false;
}

bool Preprocessor::isPunctuation(const char ch){
  if (isWhiteSpace(ch))
    return false;

  if((int)ch <= 0){
    return true;
  }
  if((int)ch > 0 && (int)ch < 65){
    return true;
  }
  if((int)ch > 90 && (int)ch < 97){
    return true;
  }
  if((int)ch > 122 && (int)ch < 256){
    return true;
  }
  return false;
}

bool Preprocessor::isStopword(const std::string & word){
  return std::binary_search(stopwords.begin(), stopwords.end(), word);
}

int main(){

  Preprocessor a;

  return 0;
}
