#pragma once

#include "document.h"

#include <memory>

class Storage {
public:
    virtual std::shared_ptr<Document> get(const std::string& url) = 0;
    virtual void update(const std::shared_ptr<Document>& document) = 0;
    virtual ~Storage() = default;
};

struct DatabaseSettings {
    std::string host;
    std::string username;
    std::string password;
    std::string databaseName;
};

std::unique_ptr<Storage> createDbStorage(const DatabaseSettings&);
