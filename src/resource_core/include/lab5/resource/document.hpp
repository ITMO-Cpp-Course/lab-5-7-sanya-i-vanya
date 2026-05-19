#pragma once
#include <string>
#include <utility>
namespace lab5::resource
{

class Document
{
  public:
    Document(int id, std::string name, std::string text) : id_(id), name_(std::move(name)), text_(std::move(text)) {}
    int GetId() const
    {
        return id_;
    }
    const std::string& GetName() const
    {
        return name_;
    }
    const std::string& GetText() const
    {
        return text_;
    }

  private:
    int id_;
    std::string name_;
    std::string text_;
};

} // namespace lab5::resource
//