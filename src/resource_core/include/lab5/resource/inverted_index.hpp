#pragma once

#include "document.hpp"

#include <string>
#include <unordered_map>
#include <vector>

namespace lab5::resource
{

class InvertedIndex
{
  public:
    void AddDocument(Document&& doc);
    void RemoveDocument(int docId);
    std::vector<int> SearchWord(const std::string& word) const;
    int GetWordCountInDocument(const std::string& word, int docId) const;

  private:
    std::unordered_map<int, Document> documents_;
    std::unordered_map<std::string, std::unordered_map<int, int>> invertedIndex_;
};

} // namespace lab5::resource
//