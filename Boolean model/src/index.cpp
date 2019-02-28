#include "index.h"
#include "preprocessor.h"
#include <dirent.h>
#include <fstream>
#include <sstream>
#include <iostream>
#include <stdexcept>

const std::string projectPath = "/home/skluzada/Downloads/BI-VWM/Project/";
const std::string datasetPath = "/home/skluzada/Downloads/BI-VWM/Project/dataset/";

Index::Index(){
  readInvertedIndex();
  readPositionalIndex();
}

bool Index::isWhiteSpace(const char ch) const{
 if(ch == ' ' || ch == '\t' || ch == '\r' ||
    ch == '\n' || ch == '\v' || ch == '\f'){
      return true;
    }
  return false;
}

void Index::writeInvertedIndex() const{
  std::ofstream outFile;
  outFile.open(projectPath + "invertedIndex.txt", std::ios::trunc);
  for(auto const & it : invertedIndex){
    outFile << it.first << " " << it.second.size() << " -> ";
    for(auto const & it2 : it.second){
      outFile << it2 << " ";
    }
    outFile << "\n";
  }
  outFile.close();
}

void Index::writePositionalIndex() const{
  std::ofstream outFile;
  outFile.open(projectPath + "positionalIndex.txt", std::ios::trunc);
  for(auto const & it : positionalIndex){
    outFile << it.first << " " << it.second.size() << " -> ";
    for(auto const & it2 : it.second){
      outFile << it2.first << " " << it2.second.size() << " : ";
      for(auto const & it3: it2.second){
        outFile << it3 << " ";
      }
    }
    outFile << "\n";
  }
  outFile.close();
}

void Index::readInvertedIndex(){
  std::ifstream inFile;
  inFile.open(projectPath + "invertedIndex.txt");
  std::string line, word, freq_s, fileIndex_s;
  std::size_t i;
  int freq, fileIndex;
  while(std::getline(inFile, line)){
    if (line[0] == ' ')
      continue;
    i = 0;
    word = "";
    freq_s = "";

    while(!isWhiteSpace(line[i])){
      word += line[i];
      i++;
    }
    i++;
    if (word == "a")
      continue;

    while(!isWhiteSpace(line[i])){
      freq_s += line[i];
      i++;
    }
    freq = std::stoi(freq_s);

    i += 4;
    for(int j = 0; j < freq; j++){
      fileIndex_s = "";
      while(!isWhiteSpace(line[i])){
        fileIndex_s += line[i];
        i++;
      }
      fileIndex = std::stoi(fileIndex_s);
      invertedIndex[word].insert(fileIndex);
      i++;
    }
  }
  inFile.close();
}

void Index::readPositionalIndex(){
  std::ifstream inFile;
  inFile.open(projectPath + "positionalIndex.txt");
  std::string line, word, freq_s, freq2_s, fileIndex_s, wordPos_s;
  std::size_t i;
  int freq, freq2, fileIndex, wordPos;
  while(std::getline(inFile, line)){
    if (line[0] == ' ')
      continue;
    i = 0;
    word = "";
    freq_s = "";
    while(!isWhiteSpace(line[i])){
      word += line[i];
      i++;
    }
    if (word == "a")
      continue;
    i++;
    while(!isWhiteSpace(line[i])){
      freq_s += line[i];
      i++;
    }
    freq = std::stoi(freq_s);
    i += 4;
    for(int j = 0; j < freq; j++){
      fileIndex_s = "";
      freq2_s = "";
      while(!isWhiteSpace(line[i])){
        fileIndex_s += line[i];
        i++;
      }
      fileIndex = std::stoi(fileIndex_s);
      i++;
      while(!isWhiteSpace(line[i])){
        freq2_s += line[i];
        i++;
      }
      freq2 = std::stoi(freq2_s);
      i += 3;
      for(int k = 0; k < freq2; k++){
        wordPos_s = "";
        while(!isWhiteSpace(line[i])){
          wordPos_s += line[i];
          i++;
        }
        wordPos = std::stoi(wordPos_s);
        (positionalIndex[word])[fileIndex].insert(wordPos);
        i++;
      }
    }
  }
  inFile.close();
}

bool Index::searchInvertedIndex(const std::string & word, const int fileIndex) const{
  try {
    invertedIndex.at(word);
  }
  catch (const std::out_of_range & oor){
    return false;
  }
  for(auto const & it : invertedIndex.at(word)){
    if (it == fileIndex)
      return true;
  }
  return false;
}

const std::map<std::string, std::set<int>> & Index::getInvertedIndex() const{
  return invertedIndex;
}

const std::map<std::string, std::map<int, std::set<int>>> & Index::getPositionalIndex() const{
  return positionalIndex;
}

void Index::create(){
  std::string filePath, word, text;
  std::ifstream infile;
  int fileIndex = -2;
  int wordPos = 0;
  std::size_t textLen, i;
  DIR * dir;
  struct dirent * ent;
  if ((dir = opendir (datasetPath.c_str())) != NULL){
    while((ent = readdir(dir)) != NULL){
      wordPos = 0;
      filePath = datasetPath + ent->d_name;
      std::cout << filePath << std::endl;
      std::ifstream inFile(filePath, std::ios::in);
      std::stringstream buffer;
      buffer << inFile.rdbuf();
      std::string text = buffer.str();
      inFile.close();
      textLen = text.size();
      i = 19;
      while (i < textLen){
        word = "";
        while((i < textLen) && isWhiteSpace(text[i])){
          i++;
        }
        while((i < textLen) && !isWhiteSpace(text[i])){
          word = word + text[i];
          i++;
        }
        invertedIndex[word].insert(fileIndex);
        (positionalIndex[word])[fileIndex].insert(wordPos);
        wordPos++;
      }
      fileIndex++;
    }
  }
}
