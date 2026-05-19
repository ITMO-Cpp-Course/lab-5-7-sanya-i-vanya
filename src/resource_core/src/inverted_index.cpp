#include <algorithm>
#include <cctype>
#include <lab5/resource/inverted_index.hpp>
#include <sstream>
#include <vector>
namespace lab5::resource
{
static std::string normalizeWord(const std::string& word)
{
    std::string result;
    for (char ch : word)
    {
        result.push_back(static_cast<char>(std::tolower(static_cast<unsigned char>(ch))));
    }
    return result;
}

static bool isPunctuation(char ch)
{
    return ch == '.' || ch == ',' || ch == '!' || ch == '?' || ch == ';' || ch == ':' || ch == '"' || ch == '(' ||
           ch == ')' || ch == '[' || ch == ']' || ch == '{' || ch == '}' || ch == '<' || ch == '>' || ch == '/' ||
           ch == '|' || ch == '@' || ch == '#' || ch == '$' || ch == '%' || ch == '^' || ch == '&' || ch == '*' ||
           ch == '+' || ch == '=' || ch == '~' || ch == '`';
}

static std::vector<std::string> splitIntoWords(const std::string& text)
{
    std::vector<std::string> words;
    std::string currentWord;

    for (char ch : text)
    {
        if (std::isspace(static_cast<unsigned char>(ch)) || isPunctuation(ch))
        {
            if (!currentWord.empty())
            {
                words.push_back(currentWord);
                currentWord.clear();
            }
        }
        else
        {
            currentWord.push_back(ch);
        }
    }

    if (!currentWord.empty())
    {
        words.push_back(currentWord);
    }

    return words;
}

void InvertedIndex::AddDocument(Document&& doc)
{
    int docId = doc.GetId();

    documents_.emplace(docId, std::move(doc));
    const Document& storedDoc = documents_.at(docId);

    std::vector<std::string> words = splitIntoWords(storedDoc.GetText());

    for (const std::string& rawWord : words)
    {
        std::string word = normalizeWord(rawWord);
        invertedIndex_[word][docId]++;
    }
}

void InvertedIndex::RemoveDocument(int docId)
{
    for (auto it = invertedIndex_.begin(); it != invertedIndex_.end();)
    {
        it->second.erase(docId);

        if (it->second.empty())
        {
            it = invertedIndex_.erase(it);
        }
        else
        {
            ++it;
        }
    }

    documents_.erase(docId);
}

std::vector<int> InvertedIndex::SearchWord(const std::string& word) const
{
    std::string norm = normalizeWord(word);
    auto it = invertedIndex_.find(norm);
    if (it == invertedIndex_.end())
    {
        return {};
    }

    std::vector<int> result;
    for (const auto& [docId, _] : it->second)
    {
        result.push_back(docId);
    }
    return result;
}

int InvertedIndex::GetWordCountInDocument(const std::string& word, int docId) const
{
    std::string norm = normalizeWord(word);
    auto wordIt = invertedIndex_.find(norm);
    if (wordIt == invertedIndex_.end())
    {
        return 0;
    }
    auto docIt = wordIt->second.find(docId);
    if (docIt == wordIt->second.end())
    {
        return 0;
    }
    return docIt->second;
}
} // namespace lab5::resource
//
