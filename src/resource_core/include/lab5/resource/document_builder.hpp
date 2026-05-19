#pragma once
#include "document.hpp"
#include <string>
namespace lab5::resource
{

class DocumentBuilder
{
  public:
    DocumentBuilder& SetId(int id)
    {
        id_ = id;
        return *this;
    }
    DocumentBuilder& SetName(const std::string& name)
    {
        name_ = name;
        return *this;
    }
    DocumentBuilder& SetText(const std::string& text)
    {
        text_ = text;
        return *this;
    }
    Document Build()
    {
        return Document(id_, std::move(name_), std::move(text_));
    }

  private:
    int id_ = 0;
    std::string name_;
    std::string text_;
};

} // namespace lab5::resource