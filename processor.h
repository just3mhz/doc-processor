#pragma once

#include "document.h"
#include "storage.h"

class DocumentProcessor {
public:
    DocumentProcessor(std::unique_ptr<Storage> storage);

    std::shared_ptr<Document> Process(std::shared_ptr<Document> input);

private:
    std::mutex mutex_;
    std::unique_ptr<Storage> storage_;
};
