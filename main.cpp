#include "document.h"
#include "processor.h"
#include "storage.h"

#include <iostream>
#include <ostream>

#include <pqxx/pqxx>

std::ostream& operator<<(std::ostream& os, const Document& document)
{
    os << "("
        << document.url
        << " " << document.pubDate
        << " " << document.fetchTime
        << " '" << document.text << "' ";
    
    if (document.firstFetchTime) {
        os << *document.firstFetchTime;
    }

    os << ")";
    return os;
}

std::istream& operator>>(std::istream& is, Document& document)
{
    is >> document.url
       >> document.pubDate
       >> document.fetchTime
       >> document.text;
    return is;
}

int main() {
    DatabaseSettings dbSettings{
        .host = "",
        .username = "",
        .password = "",
        .databaseName = ""
    };

    auto processor = DocumentProcessor(createDbStorage(dbSettings));

    Document document;
    while (std::cin >> document) {
        auto processed = processor.Process(
            std::make_shared<Document>(std::move(document)));
        std::cout << *processed << std::endl;
    }
}
