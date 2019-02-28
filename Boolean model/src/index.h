#pragma once
#include <string>
#include <map>
#include <set>

class Index{
public:
  Index();
  // create inverted index and positional index from dataset
  void create();
  // write inverted index into a file
  void writeInvertedIndex() const;
  // write positional index into a file
  void writePositionalIndex() const;
  // read inverted index from a file
  void readInvertedIndex();
  // read positional index from a file
  void readPositionalIndex();
  // return refrence to inverted index
  const std::map<std::string, std::set<int>> & getInvertedIndex() const;
  // return refrence to positional index
  const std::map<std::string, std::map<int, std::set<int>>> & getPositionalIndex() const;
  // return true if file contains the given word
  bool searchInvertedIndex(const std::string & word, const int fileIndex) const;
private:
  bool isWhiteSpace(const char ch) const;
  std::map<std::string, std::set<int>> invertedIndex;
  std::map<std::string, std::map<int, std::set<int>>> positionalIndex;
};
