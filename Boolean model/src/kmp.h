#pragma once
#include <vector>
#include <string>

// my implementation of Knuth-Morris-Pratt algorithm
// https://en.wikipedia.org/wiki/Knuth%E2%80%93Morris%E2%80%93Pratt_algorithm

void KMP_table(const std::string & word, std::vector<int> & table);
bool KMP_search(const std::string & text, const std::string & word);
