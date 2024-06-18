#include "storage.h"
#include "document.h"

#include <cstdint>
#include <memory>
#include <pqxx/pqxx>

class DbStorage final: public Storage {
public:
    DbStorage(std::string connectionUri)
        : connection_(connectionUri)
    {
    }

    std::shared_ptr<Document> get(const std::string& url)
    {
        pqxx::work transaction{connection_};
        pqxx::result result = transaction.exec(
            "SELECT * FROM documents WHERE url = " + transaction.quote(url));

        if (result.empty()) {
            return nullptr;
        }
        
        pqxx::row row = result.front();
        Document document{
            .url = row["url"].as<std::string>(),
            .pubDate = row["pubDate"].as<uint64_t>(),
            .fetchTime = row["fetchTime"].as<uint64_t>(),
            .text = row["text"].as<std::string>(),
            .firstFetchTime = row["firstFetchTime"].as<uint64_t>()
        };
        transaction.commit();
        return std::make_shared<Document>(std::move(document));
    }

    void update(const std::shared_ptr<Document>& document)
    {
        pqxx::work transaction{connection_};
        transaction.exec0(
            "INSERT INTO documents "
            "VALUES (" + transaction.quote(document->url)
                       + ", " + pqxx::to_string(document->pubDate)
                       + ", " + pqxx::to_string(document->fetchTime)
                       + ", " + transaction.quote(document->text)
                       + ", " + pqxx::to_string(*document->firstFetchTime)
                       + ") "
            "ON CONFLICT (url) DO UPDATE SET"
            " pubDate = EXCLUDED.pubDate,"
            " fetchTime = EXCLUDED.fetchTime,"
            " text = EXCLUDED.text,"
            " firstFetchTime = EXCLUDED.firstFetchTime"
        );
        transaction.commit();
    }

private:
    pqxx::connection connection_;
};


std::unique_ptr<Storage> createDbStorage(const DatabaseSettings& settings)
{
    std::stringstream stream;
    stream << "postgresql://" << settings.username << ":" << settings.password
           << "@" << settings.host << "/" << settings.databaseName;

    return std::make_unique<DbStorage>(stream.str());
}
