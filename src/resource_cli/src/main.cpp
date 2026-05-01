#include "../../resourse_core/include/lab5/resource/document.h"
#include "../../resourse_core/include/lab5/resource/document_builder.h"
#include "../../resourse_core/include/lab5/resource/inverted_index.h"

#include <iostream>
using namespace lab5::resource;
int main()
{
    InvertedIndex index;

    Document doc1 = DocumentBuilder().SetId(1).SetName("doc1.txt").SetText("Hello world hello").Build();

    Document doc2 = DocumentBuilder().SetId(2).SetName("doc2.txt").SetText("C++ programming is fun").Build();

    index.AddDocument(std::move(doc1));
    index.AddDocument(std::move(doc2));

    auto docs = index.SearchWord("hello");
    std::cout << "Word 'hello' found in docs: ";
    for (int id : docs)
        std::cout << id << " ";
    std::cout << std::endl;

    std::cout << "Count of 'hello' in doc1: " << index.GetWordCountInDocument("hello", 1) << std::endl;

    index.RemoveDocument(1);
    docs = index.SearchWord("hello");
    std::cout << "After removal, docs with 'hello': " << docs.size() << std::endl;

    return 0;
}