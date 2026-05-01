#include "../src/resource_core/include/lab5/resource/document.h"
#include "../src/resource_core/include/lab5/resource/document_builder.h"
#include "../src/resource_core/include/lab5/resource/inverted_index.h"
#include <catch2/catch_test_macros.hpp>
#include <vector>

using namespace lab5::resource;

TEST_CASE("Document creation and getters", "[document]")
{
    Document doc(100, "test_document.txt", "This is a test document content");

    SECTION("Document ID getter")
    {
        REQUIRE(doc.GetId() == 100);
    }

    SECTION("Document name getter")
    {
        REQUIRE(std::string(doc.GetName()) == "test_document.txt");
    }

    SECTION("Document text getter")
    {
        REQUIRE(std::string(doc.GetText()) == "This is a test document content");
    }
}

TEST_CASE("Document with empty text", "[document]")
{
    Document doc(200, "empty.txt", "");

    REQUIRE(doc.GetId() == 200);
    REQUIRE(doc.GetName() == "empty.txt");
    REQUIRE(doc.GetText().empty());
}

TEST_CASE("Document with long content", "[document]")
{
    std::string longText(1000, 'A'); // 1000 символов 'A'
    Document doc(300, "long.txt", longText);

    REQUIRE(doc.GetText().length() == 1000);
    REQUIRE(doc.GetText()[0] == 'A');
}

TEST_CASE("Add single document and search", "[inverted_index]")
{
    InvertedIndex index;

    Document doc =
        DocumentBuilder().SetId(42).SetName("sample.txt").SetText("quick brown fox jumps over lazy dog").Build();

    index.AddDocument(std::move(doc));

    SECTION("Search for existing word")
    {
        std::vector<int> results = index.SearchWord("fox");
        REQUIRE(results.size() == 1);
        REQUIRE(results[0] == 42);
    }

    SECTION("Search for word that appears once")
    {
        std::vector<int> results = index.SearchWord("quick");
        REQUIRE(results.size() == 1);
        REQUIRE(results[0] == 42);
    }

    SECTION("Search for non-existent word")
    {
        std::vector<int> results = index.SearchWord("elephant");
        REQUIRE(results.empty());
    }
}

TEST_CASE("Word frequency counting", "[inverted_index]")
{
    InvertedIndex index;

    Document doc =
        DocumentBuilder().SetId(10).SetName("repeat.txt").SetText("apple banana apple cherry apple banana").Build();

    index.AddDocument(std::move(doc));

    SECTION("Count of 'apple' (appears 3 times)")
    {
        int count = index.GetWordCountInDocument("apple", 10);
        REQUIRE(count == 3);
    }

    SECTION("Count of 'banana' (appears 2 times)")
    {
        int count = index.GetWordCountInDocument("banana", 10);
        REQUIRE(count == 2);
    }

    SECTION("Count of 'cherry' (appears 1 time)")
    {
        int count = index.GetWordCountInDocument("cherry", 10);
        REQUIRE(count == 1);
    }

    SECTION("Count of non-existent word")
    {
        int count = index.GetWordCountInDocument("grape", 10);
        REQUIRE(count == 0);
    }
}

TEST_CASE("Case insensitive search", "[inverted_index]")
{
    InvertedIndex index;

    Document doc = DocumentBuilder().SetId(5).SetName("mixed_case.txt").SetText("Hello HELLO hello HeLlO").Build();

    index.AddDocument(std::move(doc));

    SECTION("Search with lowercase")
    {
        int count = index.GetWordCountInDocument("hello", 5);
        REQUIRE(count == 4);
    }

    SECTION("Search with UPPERCASE")
    {
        int count = index.GetWordCountInDocument("HELLO", 5);
        REQUIRE(count == 4);
    }

    SECTION("Search with MixedCase")
    {
        int count = index.GetWordCountInDocument("HeLlO", 5);
        REQUIRE(count == 4);
    }

    SECTION("Search results for different casings")
    {
        std::vector<int> resultsLower = index.SearchWord("hello");
        std::vector<int> resultsUpper = index.SearchWord("HELLO");

        REQUIRE(resultsLower == resultsUpper);
    }
}

TEST_CASE("Multiple documents in index", "[inverted_index]")
{
    InvertedIndex index;

    Document doc1 = DocumentBuilder().SetId(1).SetName("doc1.txt").SetText("python java cpp").Build();

    Document doc2 = DocumentBuilder().SetId(2).SetName("doc2.txt").SetText("python python ruby").Build();

    Document doc3 = DocumentBuilder().SetId(3).SetName("doc3.txt").SetText("cpp cpp cpp java").Build();

    index.AddDocument(std::move(doc1));
    index.AddDocument(std::move(doc2));
    index.AddDocument(std::move(doc3));

    SECTION("Search 'python' - appears in docs 1 and 2")
    {
        std::vector<int> results = index.SearchWord("python");
        REQUIRE(results.size() == 2);
        REQUIRE(std::find(results.begin(), results.end(), 1) != results.end());
        REQUIRE(std::find(results.begin(), results.end(), 2) != results.end());
    }

    SECTION("Search 'cpp' - appears in docs 1 and 3")
    {
        std::vector<int> results = index.SearchWord("cpp");
        REQUIRE(results.size() == 2);
        REQUIRE(std::find(results.begin(), results.end(), 1) != results.end());
        REQUIRE(std::find(results.begin(), results.end(), 3) != results.end());
    }

    SECTION("Search 'ruby' - appears only in doc 2")
    {
        std::vector<int> results = index.SearchWord("ruby");
        REQUIRE(results.size() == 1);
        REQUIRE(results[0] == 2);
    }

    SECTION("Search 'java' - appears in docs 1 and 3, different frequencies")
    {
        REQUIRE(index.GetWordCountInDocument("java", 1) == 1);
        REQUIRE(index.GetWordCountInDocument("java", 3) == 1);
        REQUIRE(index.GetWordCountInDocument("java", 2) == 0);
    }
}

TEST_CASE("Remove document from index", "[inverted_index]")
{
    InvertedIndex index;

    Document doc1 = DocumentBuilder().SetId(7).SetName("first.txt").SetText("unique_special_word content").Build();

    Document doc2 =
        DocumentBuilder().SetId(8).SetName("second.txt").SetText("another unique_special_word here").Build();

    index.AddDocument(std::move(doc1));
    index.AddDocument(std::move(doc2));

    SECTION("Before removal - both documents contain 'unique_special_word'")
    {
        std::vector<int> results = index.SearchWord("unique_special_word");
        REQUIRE(results.size() == 2);
    }

    SECTION("After removing first document")
    {
        index.RemoveDocument(7);

        std::vector<int> results = index.SearchWord("unique_special_word");
        REQUIRE(results.size() == 1);
        REQUIRE(results[0] == 8);

        REQUIRE(index.GetWordCountInDocument("unique_special_word", 7) == 0);
    }
}

TEST_CASE("Remove non-existent document", "[inverted_index]")
{
    InvertedIndex index;

    Document doc = DocumentBuilder().SetId(15).SetText("some text").Build();

    index.AddDocument(std::move(doc));

    SECTION("Removing non-existent ID should not affect index")
    {
        index.RemoveDocument(999);

        REQUIRE(index.SearchWord("some").size() == 1);
        REQUIRE(index.GetWordCountInDocument("text", 15) == 1);
    }
}

TEST_CASE("Empty document handling", "[inverted_index]")
{
    InvertedIndex index;

    Document emptyDoc = DocumentBuilder().SetId(99).SetName("empty.txt").SetText("").Build();

    index.AddDocument(std::move(emptyDoc));

    SECTION("Search should return no results")
    {
        std::vector<int> results = index.SearchWord("anything");
        REQUIRE(results.empty());
    }

    SECTION("Word count should be zero")
    {
        int count = index.GetWordCountInDocument("anything", 99);
        REQUIRE(count == 0);
    }
}

TEST_CASE("Document with special characters and numbers", "[inverted_index]")
{
    InvertedIndex index;

    Document doc = DocumentBuilder().SetId(50).SetName("special.txt").SetText("c++ python3 c# java_8 test123").Build();

    index.AddDocument(std::move(doc));

    SECTION("Search with special characters")
    {
        std::vector<int> results = index.SearchWord("c++");
        REQUIRE(results.size() == 1);
        REQUIRE(results[0] == 50);
    }

    SECTION("Search with numbers in word")
    {
        std::vector<int> results = index.SearchWord("python3");
        REQUIRE(results.size() == 1);
    }

    SECTION("Search with underscore")
    {
        std::vector<int> results = index.SearchWord("java_8");
        REQUIRE(results.size() == 1);
    }

    SECTION("Mixed alphanumeric")
    {
        std::vector<int> results = index.SearchWord("test123");
        REQUIRE(results.size() == 1);
    }
}

TEST_CASE("Large document performance", "[inverted_index]")
{
    InvertedIndex index;

    std::string largeText;
    for (int i = 0; i < 1000; ++i)
    {
        largeText += "performance test benchmark ";
    }

    Document largeDoc = DocumentBuilder().SetId(1000).SetName("large.txt").SetText(largeText).Build();

    index.AddDocument(std::move(largeDoc));

    SECTION("Word frequency should be correct for repeated words")
    {
        int count = index.GetWordCountInDocument("performance", 1000);
        REQUIRE(count == 1000);
    }

    SECTION("Multiple words frequency")
    {
        REQUIRE(index.GetWordCountInDocument("test", 1000) == 1000);
        REQUIRE(index.GetWordCountInDocument("benchmark", 1000) == 1000);
    }
}

TEST_CASE("DocumentBuilder chain operations", "[document_builder]")
{
    SECTION("Build document with all fields")
    {
        Document doc =
            DocumentBuilder().SetId(777).SetName("chain_test.txt").SetText("built by document builder").Build();

        REQUIRE(doc.GetId() == 777);
        REQUIRE(std::string(doc.GetName()) == "chain_test.txt");
        REQUIRE(std::string(doc.GetText()) == "built by document builder");
    }

    SECTION("Build document with only ID and text")
    {
        Document doc = DocumentBuilder().SetId(888).SetText("only id and text").Build();

        REQUIRE(doc.GetId() == 888);
        REQUIRE(std::string(doc.GetName()).empty());
        REQUIRE(std::string(doc.GetText()) == "only id and text");
    }

    SECTION("Build document with only text")
    {
        Document doc = DocumentBuilder().SetText("just text").Build();

        REQUIRE(doc.GetId() == 0);
        REQUIRE(std::string(doc.GetText()) == "just text");
    }
}

TEST_CASE("Overwrite existing document", "[inverted_index]")
{
    InvertedIndex index;

    Document firstVersion = DocumentBuilder().SetId(555).SetText("first version content").Build();

    Document secondVersion = DocumentBuilder().SetId(555).SetText("second version updated content").Build();

    index.AddDocument(std::move(firstVersion));

    SECTION("Before overwrite")
    {
        REQUIRE(index.GetWordCountInDocument("first", 555) == 1);
        REQUIRE(index.GetWordCountInDocument("version", 555) == 1);
    }
}